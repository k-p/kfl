/* @(#)Heap.h
 */

#pragma once

#include "CoreExpr.h"

#include <iostream>
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
      using Addr = Heap::Addr;
      virtual ~Node() { }
      virtual void print(std::ostream& os) const = 0;
    };

    virtual Addr allocAp(Addr fn, Addr arg) = 0;
    virtual Addr allocNum(int) = 0;
    virtual Addr allocSupercomb(const Name& name, const ArgList& args,
                                const CoreExpr& body) = 0;

    virtual const Node& lookup(Addr addr) const = 0;
  };

  inline std::ostream& operator<<(std::ostream& os, const Heap::Node& node) {
    node.print(os);
    return os;
  }

}
