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
    const std::optional<TiHeap::Addr> addr_;

  public:
    TiHeap::Addr visit(const CoreVar& e)
    {
      const auto itr = env_.find(e.getId());
      if (itr == env_.end()) {
        throw std::runtime_error("Undefined name " + e.getId());
      }
      return itr->second;
    }

    TiHeap::Addr visit(const CoreNum& e)
    {
      return (addr_ ? heap_.updateNum(*addr_, e.getNum()) : heap_.allocNum(e.getNum()));
    }

    TiHeap::Addr visit(const CoreConstr& e)
    { 
      throw std::runtime_error("Can't instantiate constructors yet");
    }

    TiHeap::Addr visit(const CoreAp& e)
    {
      return (addr_
                ? heap_.updateAp(*addr_,
                                 Instantiate(heap_, env_)(e.getFn()),
                                 Instantiate(heap_, env_)(e.getArg()))
                : heap_.allocAp(Instantiate(heap_, env_)(e.getFn()),
                                Instantiate(heap_, env_)(e.getArg())));
    }

    TiHeap::Addr visit(const CoreLet& e)
    {
      TiGlobals localEnv = env_;
      for (const auto& d : e.getDefns()) {
        localEnv[d.first] = heap_.alloc();
      }
      for (const auto& d : e.getDefns()) {
        Instantiate(heap_, localEnv, localEnv[d.first])(*d.second);
      }
      return Instantiate(heap_, localEnv)(e.getBody());
    }

    TiHeap::Addr visit(const CoreCase& e)
    {
      throw std::runtime_error("Can't instantiate case exprs");
    }

    TiHeap::Addr visit(const CoreLam& e)
    {
      throw std::runtime_error("Can't instantiate lambda exprs");
    }

  public:
    Instantiate(TiHeap& heap, const TiGlobals& env) : super(), heap_(heap), env_(env) { }
    Instantiate(TiHeap& heap, const TiGlobals& env, const TiHeap::Addr addr) : super(), heap_(heap), env_(env), addr_(addr) { }
  };

  class NNum : public TiNode {
    public:
      NNum(int n) : num_(n) { }

      std::ostream& print(std::ostream& os) const override {
        return os << "NNum " << num_;
      }

      TiState dispatch(TiState state) const override {
        throw std::runtime_error("Number applied as function");
      }

      bool isDataNode() const override { return true; }

    private:
      const int num_;
    };

  class NAp : public TiNode {
    public:
      NAp(Addr fn, Addr arg) : fn_(fn), arg_(arg) { }

      std::ostream& print(std::ostream& os) const override {
        return os << "NAp " << fn_ << ' ' << arg_;
      }

      TiState dispatch(TiState state) const override {
        state.stack.push_back(fn_);
        return state;
      }

      Addr getArg() const {
        return arg_;
      }

    private:
      const Addr fn_;
      const Addr arg_;
    };

    class NSupercomb : public TiNode {
    public:
      NSupercomb(const Name& name, const ArgList& args, const CoreExpr& body)
        : name_(name), args_(args), body_(body) { }

      std::ostream& print(std::ostream& os) const override {
        return os << "NSupercomb " << name_;
      }

      TiState dispatch(TiState state) const override {
        const TiStack values = getArgs(state.heap, state.stack);
        if (values.size() < args_.size()) {
          throw std::runtime_error("Not enough arguments to apply supercombinator '" + name_ + "'. "
          + std::to_string(args_.size()) + " required, but only " + std::to_string(values.size()) + " supplied");
        }
        std::map<Name, Addr> env(state.globals);
        auto v = values.rbegin();
        for (const auto& name : args_) {
          assert(v != values.rend());
          env[name] = *v;
          ++v;
        }
        //std::transform(args_.begin(), args_.end(), values.rbegin(), std::inserter(env, env.end()),
        //                [](const Name& name, const Addr value) { return std::make_pair(name, value); });
        const auto root = state.stack.end() - args_.size() - 1;
        const auto addr = Instantiate(state.heap, env)(body_);
        state.heap.updateInd(*root, addr);
        state.stack.erase(root, state.stack.end());
        state.stack.push_back(addr);
        return state;
      }

    private:
      TiStack getArgs(const TiHeap& heap, const TiStack& stack) const {
        TiStack args;
        std::transform(stack.cbegin(), stack.cend() - 1, std::back_inserter(args),
                        [&](const Addr addr){
                          return heap.lookup(addr).getArg();
                        });
        return args;
      }

      const Name name_;
      const ArgList args_;
      const CoreExpr& body_;
    };

    class NInd : public TiNode
    {
    public:
      NInd(Addr addr) : addr_(addr) { }

      std::ostream& print(std::ostream& os) const override {
        return os << "NInd " << addr_;
      }

      TiState dispatch(TiState state) const override {
        state.stack.back() = addr_;
        return state;
      }

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
TiHeap::alloc()
{
  heap_.emplace_back(std::shared_ptr<TiNode>(nullptr));
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
TiHeap::updateSupercomb(const Addr addr, const Name& name, const std::vector<Name>& args, const CoreExpr& body)
{
  return update<NSupercomb>(addr, name, args, body);
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
