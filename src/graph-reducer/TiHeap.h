/* -*-c++-*-
 *
 *  Created by Keith Dennison on 18/01/2014.
 *  Copyright (c) 2014 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Heap.h"

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace kfl {

  class TiState;

  class TiHeap : public Heap<unsigned, std::string>
  {
  public:

    class TiNode : public Node {
    public:
      virtual TiState step(TiState state) const = 0;
      virtual bool isDataNode() const { return false; }
      virtual Addr getArg() const { throw std::runtime_error("Tried to get arg of node that is not an application"); }
    };

    Addr allocAp(Addr fn, Addr arg) override;
    Addr allocNum(int n) override;
    Addr allocSupercomb(const Name& name, const ArgList& args, const CoreExpr& body) override;

    Addr alloc();
    Addr updateAp(Addr addr, Addr fn, Addr arg);
    Addr updateNum(Addr addr, int n);
    Addr updateSupercomb(Addr addr, const Name& name, const ArgList& args, const CoreExpr& body);
    Addr updateInd(Addr addr, Addr target);

    const TiNode& lookup(Addr addr) const override;

    unsigned getAllocs() const { return allocs_; }
    unsigned getUpdates() const { return updates_; }

  private:
    template<typename T, typename... Args>
    inline Addr allocate(Args&&... args) {
      heap_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
      ++allocs_;
      return Addr(heap_.size() - 1);
    }

    template<typename T, typename... Args>
    inline TiHeap::Addr update(const Addr addr, Args&&... args) {
      heap_[addr] = std::make_unique<T>(std::forward<Args>(args)...);
      ++updates_;
      return addr;
    }

    using TiNodeHeap = std::vector<std::shared_ptr<TiNode>>;
    TiNodeHeap heap_;
    unsigned allocs_ = 0;
    unsigned updates_ = 0;
  };

  inline std::ostream& operator<<(std::ostream& os, const TiHeap::TiNode& node) {
    return node.print(os);
  }

} /* end namespace kfl */
