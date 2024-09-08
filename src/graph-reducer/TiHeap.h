/* -*-c++-*-
 *
 *  Created by Keith Dennison on 18/01/2014.
 *  Copyright (c) 2014 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Heap.h"

#include <memory>
#include <vector>

namespace kfl {

  class TiState;

  class TiHeap : public Heap
  {
  public:

    class TiNode : public Node {
    public:
      virtual TiState dispatch(TiState state) const = 0;
      virtual bool isDataNode() const { return false; }
    };

    Addr allocAp(Addr fn, Addr arg) override;
    Addr allocNum(int n) override;
    Addr allocSupercomb(const Name& name, const ArgList& args, const CoreExpr& body) override;

    const TiNode& lookup(Addr addr) const override;

  private:
    using TiNodeHeap = std::vector<std::shared_ptr<TiNode>>;
    TiNodeHeap heap_;
  };

} /* end namespace kfl */
