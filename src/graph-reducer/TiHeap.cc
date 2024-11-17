/* @(#)TiHeap.cc
 */

#include "TiHeap.h"
#include "TiState.h"

#include <algorithm>
#include <iterator>
#include <exception>
#include <optional>

namespace {

  using namespace kfl;
  using ArgList = TiHeap::ArgList;
  using Name = TiHeap::Name;
  using TiNode = TiHeap::TiNode;
  
  class Instantiate : private CoreDefaultVisitor
  {
    TiHeap& heap_;
    const TiGlobals& env_;
    std::optional<TiHeap::Addr> addr_;

    void visitVar(const CoreVar& e) override
    {
      const auto itr = env_.find(e.getId());
      if (itr == env_.end()) {
        throw std::runtime_error("Undefined name " + e.getId());
      }
      addr_ = itr->second;
    }

    void visitNum(const CoreNum& e) override
    {
      if (addr_) {
        heap_.setNum(*addr_, e.getNum());
      }
      else {
        addr_ = heap_.allocNum(e.getNum());
      }
    }

    void visitConstr(const CoreConstr& e) override
    { 
      throw std::runtime_error("Can't instantiate constructors yet");
    }

    void visitAp(const CoreAp& e) override
    {
      if (addr_) {
        heap_.setAp(*addr_,
                    Instantiate(heap_, env_)(e.getFn()),
                    Instantiate(heap_, env_)(e.getArg()));
      }
      else {
        addr_ = heap_.allocAp(Instantiate(heap_, env_)(e.getFn()),
                              Instantiate(heap_, env_)(e.getArg()));
      }
    }

    void visitLet(const CoreLet& e) override
    {
      TiGlobals localEnv = env_;
      for (const auto& d : e.getDefns()) {
        localEnv[d.first] = heap_.alloc();
      }
      for (const auto& d : e.getDefns()) {
        Instantiate(heap_, localEnv, localEnv[d.first])(*d.second);
      }
      addr_ = Instantiate(heap_, localEnv)(e.getBody());
    }

    void visitCase(const CoreCase& e) override
    {
      throw std::runtime_error("Can't instantiate case exprs");
    }

  public:
    Instantiate(TiHeap& heap, const TiGlobals& env) : heap_(heap), env_(env) { }
    Instantiate(TiHeap& heap, const TiGlobals& env, const TiHeap::Addr addr) : heap_(heap), env_(env), addr_(addr) { }
    TiHeap::Addr operator()(const CoreExpr& e) { e.visit(*this); return *addr_; }
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
      int num_;
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
          env[name] = *v;
          ++v;
        }
        //std::transform(args_.begin(), args_.end(), values.rbegin(), std::inserter(env, env.end()),
        //                [](const Name& name, const Addr value) { return std::make_pair(name, value); });
        state.stack.erase(state.stack.end() - args_.size() - 1, state.stack.end());
        state.stack.push_back(Instantiate(state.heap, env)(body_));
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

void
TiHeap::setAp(const Addr addr, const Addr fn, const Addr arg)
{
  set<NAp>(addr, fn, arg);
}

void
TiHeap::setNum(const Addr addr, const int n)
{
  set<NNum>(addr, n);
}

void
TiHeap::setSupercomb(const Addr addr, const Name& name, const std::vector<Name>& args, const CoreExpr& body)
{
  set<NSupercomb>(addr, name, args, body);
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
