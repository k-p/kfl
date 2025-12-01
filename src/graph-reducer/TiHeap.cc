/* @(#)TiHeap.cc
 */

#include "TiHeap.h"
#include "TiState.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <exception>
#include <optional>

namespace {

  using namespace kfl;
  using ArgList = TiHeap::ArgList;
  using Name = TiHeap::Name;
  using TiNode = TiHeap::TiNode;

  class Instantiate : public CoreFnExprVisitor<TiHeap::Addr, Instantiate>
  {
    using super = CoreFnExprVisitor<TiHeap::Addr, Instantiate>;

    TiHeap& heap_;
    const TiGlobals& env_;

  protected:
    TiHeap& heap() { return heap_; }
    const TiGlobals& env() const { return env_; }

  public:
    virtual TiHeap::Addr visit(const CoreVar& e)
    {
      const auto itr = env().find(e.getId());
      if (itr == env().end()) {
        throw std::runtime_error("Undefined name " + e.getId());
      }
      return itr->second;
    }

    virtual TiHeap::Addr visit(const CoreNum& e)
    {
      return heap().allocNum(e.getNum());
    }

    virtual TiHeap::Addr visit(const CoreConstr& e)
    { 
      throw std::runtime_error("Can't instantiate constructors yet");
    }

    virtual TiHeap::Addr visit(const CoreAp& e)
    {
      return heap().allocAp(Instantiate(heap(), env())(e.getFn()), Instantiate(heap(), env())(e.getArg()));
    }

    virtual TiHeap::Addr visit(const CoreLet& e)
    {
      TiGlobals localEnv = env();
      for (const auto& d : e.getDefns()) {
        localEnv[d.first] = heap().alloc();
      }
      for (const auto& d : e.getDefns()) {
        Instantiate(heap(), localEnv)(*d.second);
      }
      return Instantiate(heap(), localEnv)(e.getBody());
    }

    virtual TiHeap::Addr visit(const CoreCase& e)
    {
      throw std::runtime_error("Can't instantiate case exprs");
    }

    virtual TiHeap::Addr visit(const CoreLam& e)
    {
      throw std::runtime_error("Can't instantiate lambda exprs");
    }

    Instantiate(TiHeap& heap, const TiGlobals& env) : super(), heap_(heap), env_(env) { }
  };

  class InstantiateAndUpdate : public Instantiate
  {
    using super = Instantiate;

    const TiHeap::Addr addr_;

  protected:
    TiHeap::Addr addr() const { return addr_; }

  public:
    TiHeap::Addr visit(const CoreNum& e) override
    {
      return heap().updateNum(addr(), e.getNum());
    }

    TiHeap::Addr visit(const CoreAp& e) override
    {
      return heap().updateAp(addr(), Instantiate(heap(), env())(e.getFn()), Instantiate(heap(), env())(e.getArg()));
    }

    TiHeap::Addr visit(const CoreLet& e) override
    {
      TiGlobals localEnv = env();
      for (const auto& d : e.getDefns()) {
        localEnv[d.first] = heap().alloc();
      }
      for (const auto& d : e.getDefns()) {
        InstantiateAndUpdate(heap(), localEnv, localEnv[d.first])(*d.second);
      }
      return Instantiate(heap(), localEnv)(e.getBody());
    }

    InstantiateAndUpdate(TiHeap& heap, const TiGlobals& env, const TiHeap::Addr addr) : super(heap, env), addr_(addr) { }
  };

  class NNum : public TiNode {
    public:
      NNum(int n) : num_(n) { }

      std::ostream& print(std::ostream& os) const override {
        return os << "NNum " << num_;
      }

      TiState step(TiState state) const override {
        if (state.dump.empty()) {
          throw std::runtime_error("Number applied as function");
        }
        state.stack = state.dump.back();
        state.dump.pop_back();
        return state;
      }

      bool isDataNode() const override { return true; }
      std::optional<int> tryGetNum() const override { return num_; }

    private:
      const int num_;
    };

  class NAp : public TiNode {
    public:
      NAp(Addr fn, Addr arg) : fn_(fn), arg_(arg) { }

      std::ostream& print(std::ostream& os) const override {
        return os << "NAp " << fn_ << ' ' << arg_;
      }

      TiState step(TiState state) const override {
        auto& arg = state.heap.lookup(getArg());
        if (arg.isIndNode()) {
          arg_ = arg.getTarget();
        }
        state.stack.push_back(fn_);
        state.stats.incPrimitive();
        return state;
      }

      Addr getArg() const {
        return arg_;
      }

    private:
      const Addr fn_;
      mutable Addr arg_;
    };

    class NFn : public TiNode {
    public:
      NFn(const Name& name) : name_(name) { }

      std::ostream& print(std::ostream& os) const override {
        return os << getDescription() << ' ' << getName();
      }

    protected:
      const Name& getName() const { return name_; }
      TiStack getArgs(const TiHeap& heap, const TiStack& stack, size_t n) const {
        if (stack.size() - 1 < n) {
          throw std::runtime_error("Not enough arguments to apply " + std::string(getDescription()) + " '" + getName() + "'. "
          + std::to_string(n) + " required, but only " + std::to_string(stack.size()-1) + " supplied");
        }
        TiStack args;
        std::transform(stack.crbegin() + 1, stack.crend(), std::back_inserter(args),
                        [&](const Addr addr){
                          return heap.lookup(addr).getArg();
                        });
        return args;
      }

    private:
      virtual const char * const getDescription() const = 0;

      const Name name_;
    };

    class NSupercomb : public NFn {
      using super = NFn;
    public:
      NSupercomb(const Name& name, const ArgList& args, const CoreExpr& body)
        : super(name), args_(args), body_(body) { }

      TiState step(TiState state) const override {
        const TiStack values = getArgs(state.heap, state.stack, args_.size());
        std::map<Name, Addr> env(state.globals);
        auto v = values.cbegin();
        for (const auto& name : args_) {
          assert(v != values.cend());
          env[name] = *v;
          ++v;
        }
        const auto root = state.stack.end() - args_.size() - 1;
        const auto addr = InstantiateAndUpdate(state.heap, env, *root)(body_);
        if (addr != *root) {
          state.heap.updateInd(*root, addr);
        }
        state.stack.erase(root + 1, state.stack.end());
        state.stats.incSupercomb();
        return state;
      }

    private:
      const char * const getDescription() const override { return "NSupercomb"; }

      const ArgList args_;
      const CoreExpr& body_;
    };

    class NPrim : public NFn
    {
      using super = NFn;
    public:
      NPrim(Name name) : super(name) { }

      TiState step(TiState state) const override {
        const auto args = getArgs(state.heap, state.stack, arity());
        unsigned i = 0;
        for (const auto addr : args) {
          ++i;
          const auto& value = state.heap.lookup(addr);
          if (!value.isDataNode()) {
            // argument not yet a number - undo the last unwind and save the stack
            state.stack.erase(state.stack.end() - i, state.stack.end());
            state.dump.push_back(state.stack);
            // create a new stack to evaluate the argument
            state.stack.clear();
            state.stack.push_back(addr);
            return state;
          }
        }
        // All arguments are evaluated; perform the primitive operation
        const auto root = state.stack.end() - arity() - 1;
        std::vector<int> argValues;
        std::transform(args.cbegin(), args.cend(), std::back_inserter(argValues),
                       [&](const Addr addr){
                         return state.heap.lookup(addr).getNum();
                       });
        state.heap.updateNum(*root, apply(argValues));
        state.stack.erase(root + 1, state.stack.end());
        return state;
      }

    private:
      const char * const getDescription() const override { return "NPrim"; }
      virtual size_t arity() const = 0;
      virtual int apply(std::vector<int> args) const = 0;
    };

    class NUnaryPrim : public NPrim
    {
      using super = NPrim;
    public:
      using Fn = std::function<int(int)>;
      NUnaryPrim(Name name, Fn fn) : super(name), fn_(std::move(fn)) { }
      size_t arity() const override { return 1; }
      int apply(std::vector<int> args) const override { return fn_(args[0]); }

    private:
      const Fn fn_;
    };

    class NBinaryPrim : public NPrim
    {
      using super = NPrim;
    public:
      using Fn = std::function<int(int, int)>;
      NBinaryPrim(Name name, Fn fn) : super(name), fn_(std::move(fn)) { }
      size_t arity() const override { return 2; }
      int apply(std::vector<int> args) const override { return fn_(args[0], args[1]); }

    private:
      const Fn fn_;
    };

    class NInd : public TiNode
    {
    public:
      NInd(Addr addr) : addr_(addr) { }

      std::ostream& print(std::ostream& os) const override {
        return os << "NInd " << addr_;
      }

      TiState step(TiState state) const override {
        state.stack.back() = addr_;
        state.stats.incInd();
        return state;
      }

      bool isIndNode() const override { return true; }
      Addr getTarget() const override { return addr_; }

    private:
      const Addr addr_;
    };

}

TiHeap::Addr
TiHeap::allocAp(const Addr fn, const Addr arg)
{
  return allocate<NAp>(fn, arg);
}

TiHeap::Addr
TiHeap::allocNum(const int n)
{
  return allocate<NNum>(n);
}

TiHeap::Addr
TiHeap::allocSupercomb(const Name& name, const std::vector<Name>& args, const CoreExpr& body)
{
  return allocate<NSupercomb>(name, args, body);
}

TiHeap::Addr
TiHeap::allocUnaryPrim(const Name& name, std::function<int(int)> fn)
{
  return allocate<NUnaryPrim>(name, fn);
}

TiHeap::Addr
TiHeap::allocBinaryPrim(const Name& name, std::function<int(int, int)> fn)
{
  return allocate<NBinaryPrim>(name, fn);
}

TiHeap::Addr
TiHeap::alloc()
{
  heap_.emplace_back(nullptr);
  ++allocs_;
  return Addr(heap_.size() - 1);
}

TiHeap::Addr
TiHeap::updateAp(const Addr addr, const Addr fn, const Addr arg)
{
  return update<NAp>(addr, fn, arg);
}

TiHeap::Addr
TiHeap::updateNum(const Addr addr, const int n)
{
  return update<NNum>(addr, n);
}

TiHeap::Addr
TiHeap::updateInd(const Addr addr, const Addr target)
{
  return update<NInd>(addr, target);
}

const TiNode&
TiHeap::lookup(Addr addr) const
{
  if (addr >= heap_.size()) {
    throw std::runtime_error("Heap lookup failed: address out of range - " + std::to_string(addr));
  }
  if (heap_[addr].get() == nullptr) {
    throw std::runtime_error("Heap lookup failed: null node - " + std::to_string(addr));
  }
  return *heap_[addr];
}
