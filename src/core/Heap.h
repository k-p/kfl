/* @(#)Heap.h
 */

#pragma once

#include "CoreExpr.h"

#include <string>
#include <vector>

namespace kfl {

  class Heap
  {
  public:

    virtual ~Heap() { }

    using Addr = unsigned;
    using Name = std::string;
    using ArgList = std::vector<Name>;

    class Node
    {
    public:
      virtual ~Node() { }
    };

    virtual Addr allocAp(Addr fn, Addr arg) = 0;
    virtual Addr allocNum(int) = 0;
    virtual Addr allocSupercomb(const Name& name, const ArgList& args,
                                   const CoreExpr& body) = 0;

    virtual const Node& lookup(Addr addr) const = 0;

  };

}
