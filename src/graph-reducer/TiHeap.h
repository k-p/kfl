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
      virtual TiState dispatch(TiState state) const = 0;
      virtual bool isDataNode() const { return false; }
      virtual Addr getArg() const { throw std::runtime_error("Tried to get arg of node that is not an application"); }
    };

    Addr allocAp(Addr fn, Addr arg) override;
    Addr allocNum(int n) override;
    Addr allocSupercomb(const Name& name, const ArgList& args, const CoreExpr& body) override;

    const TiNode& lookup(Addr addr) const override;

  private:
    template<typename T, typename... Args>
    inline Addr allocate(Args&&... args)
    {
      heap_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
      return Addr(heap_.size() - 1);
    }

  private:
    using TiNodeHeap = std::vector<std::shared_ptr<TiNode>>;
    TiNodeHeap heap_;
  };

  inline std::ostream& operator<<(std::ostream& os, const TiHeap::TiNode& node) {
    return node.print(os);
  }

} /* end namespace kfl */
