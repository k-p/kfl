/* @(#)TiHeap.cc
 */

#include "TiHeap.h"
#include "TiState.h"

#include <algorithm>
#include <iterator>
#include <exception>

namespace {

  using namespace kfl;
  using ArgList = TiHeap::ArgList;
  using Name = TiHeap::Name;
  using TiNode = TiHeap::TiNode;
  
  class Instantiate : private CoreDefaultVisitor
  {
    Heap& heap_;
    const TiGlobals& env_;
    Addr addr_ = 0;

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
      addr_ = heap_.allocNum(e.getNum());
    }

    void visitConstr(const CoreConstr& e) override
    { 
      throw std::runtime_error("Can't instantiate constructors yet");
    }

    void visitAp(const CoreAp& e) override
    {
      addr_ = heap_.allocAp(Instantiate(heap_, env_)(e.getFn()),
                            Instantiate(heap_, env_)(e.getArg()));
    }

    void visitLet(const CoreLet& e) override
    {
      throw std::runtime_error("Can't instantiate let(rec)s yet");
    }

    void visitCase(const CoreCase& e) override
    {
      throw std::runtime_error("Can't instantiate case exprs");
    }

  public:
    Instantiate(Heap& heap, const TiGlobals& env) : heap_(heap), env_(env) { }
    Addr operator()(const CoreExpr& e) { e.visit(*this); return addr_; }
  };

  class NNum : public TiNode {
    public:
      NNum(int n) : num_(n) { }

      void print(std::ostream& os) const override {
        os << "NNum " << num_;
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

      void print(std::ostream& os) const override {
        os << "NAp " << fn_ << ' ' << arg_;
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

      void print(std::ostream& os) const override {
        os << "NSupercomb " << name_;
      }

      TiState dispatch(TiState state) const override {
        const TiStack values = getArgs(state.heap, state.stack);
        if (values.size() < args_.size()) {
          throw std::runtime_error("Not enough arguments to apply supercombinator '" + name_ + "'. "
          + std::to_string(args_.size()) + " required, but only " + std::to_string(values.size()) + " supplied");
        }
        std::map<Name, Addr> env(state.globals);
        std::transform(args_.begin(), args_.end(), values.rbegin(), std::inserter(env, env.end()),
                        [](const Name& name, const Addr value) { return std::make_pair(name, value); });
        state.stack.erase(state.stack.end() - args_.size() - 1, state.stack.end());
        state.stack.push_back(Instantiate(state.heap, env)(body_));
        return state;
      }

    private:
      TiStack getArgs(const TiHeap& heap, const TiStack& stack) const {
        TiStack args(&heap);
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

Addr
TiHeap::allocAp(const Addr fn, const Addr arg)
{
  return allocate<NAp>(fn, arg);
}

Addr
TiHeap::allocNum(const int n)
{
  return allocate<NNum>(n);
}

Addr
TiHeap::allocSupercomb(const Name& name, const std::vector<Name>& args, const CoreExpr& body)
{
  return allocate<NSupercomb>(name, args, body);
}

const TiNode&
TiHeap::lookup(Addr addr) const
{
  if (addr >= heap_.size()) {
    throw std::runtime_error("Heap lookup failed");
  }
  return *heap_[addr];
}
