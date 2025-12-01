/* @(#)Heap.h
 */

#pragma once

#include "CoreExpr.h"

#include <iostream>
#include <vector>

namespace kfl {

  template<typename A, typename N>
  class Heap
  {
  public:
    using Addr = A;
    using Name = N;
    using ArgList = std::vector<Name>;

    class Node
    {
    public:
      using Addr = Heap::Addr;
      virtual ~Node() = default;
      virtual std::ostream& print(std::ostream& os) const = 0;
    };

    virtual ~Heap() = default;
    virtual Addr allocAp(Addr fn, Addr arg) = 0;
    virtual Addr allocNum(int) = 0;
    virtual Addr allocSupercomb(const Name& name, const ArgList& args,
                                const CoreExpr& body) = 0;
    virtual Addr allocUnaryPrim(const Name& name, std::function<int(int)> fn) = 0;
    virtual Addr allocBinaryPrim(const Name& name, std::function<int(int, int)> fn) = 0;

    virtual const Node& lookup(Addr addr) const = 0;
  };

  //template<typename A, typename N>
  //inline std::ostream& operator<<(std::ostream& os, const typename Heap<A, N>::Node& node) {
  //  return node.print(os);
  //}

}
