/* @(#)TiHeap.cc
 */

#include "TiHeap.h"
#include "TiState.h"

#include <exception>
#include <utility>

namespace {

  using namespace kfl;
  using TiNode = TiHeap::TiNode;
  using TiNodeHeap = std::vector<std::shared_ptr<TiNode>>;
  using Name = TiHeap::Name;
  using ArgList = TiHeap::ArgList;
  
  class Instantiate : private CoreDefaultVisitor
  {
    Heap& heap_;
    Addr addr_ = 0;

    void visitNum(const CoreNum& e) override
    {
      addr_ = heap_.allocNum(e.getNum());
    }

    void visitAp(const CoreAp& e) override
    {
      addr_ = heap_.allocAp(Instantiate(heap_)(e.getFn()),
                            Instantiate(heap_)(e.getArg()));
    }

  public:
    Instantiate(Heap& heap) : heap_(heap) { }
    Addr operator()(const CoreExpr& e) { e.visit(*this); return addr_; }
  };

  class NNum : public TiNode {
    public:
      NNum(int n) : num_(n) { }
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
      TiState dispatch(TiState state) const override {
        TiState newState(state);
        newState.stack.push_back(fn_);
        return newState;
      }
    private:
      const Addr fn_;
      const Addr arg_;
    };

    class NSupercomb : public TiNode {
    public:
      NSupercomb(const Name& name, const ArgList& args, const CoreExpr& body)
        : name_(name), args_(args), body_(body) { }
      TiState dispatch(TiState state) const override {
        const auto addr = Instantiate(state.heap)(body_);
        state.stack[0] = addr;
        //state.stack.erase(0, args_.size());
        return state;
      }
    private:
      const Name& name_;
      const ArgList& args_;
      const CoreExpr& body_;
    };

  template<typename T, typename... Args>
  Addr allocate(TiNodeHeap& heap, Args&&... args)
  {
    heap.push_back(std::make_shared<T>(std::forward<Args>(args)...));
    return heap.size()-1;
  }

}

Addr
TiHeap::allocAp(const Addr fn, const Addr arg)
{
  return allocate<NAp>(heap_, fn, arg);
}

Addr
TiHeap::allocNum(const int n)
{
  return allocate<NNum>(heap_, n);
}

Addr
TiHeap::allocSupercomb(const Name& name, const std::vector<Name>& args, const CoreExpr& body)
{
  return allocate<NSupercomb>(heap_, name, args, body);
}

const TiNode&
TiHeap::lookup(Addr addr) const
{
  if (addr >= heap_.size()) {
    throw std::runtime_error("Heap lookup failed");
  }
  return *heap_[addr];
}
