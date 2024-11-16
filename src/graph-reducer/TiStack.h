/* @(#)TiStack.h
 */

#pragma once

#include "TiHeap.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

namespace kfl {

  class TiStack : private std::vector<TiHeap::Addr> {
  public:
    using Addr = TiHeap::Addr;

    inline explicit TiStack() : std::vector<Addr>() { }

    using std::vector<Addr>::value_type;

    using std::vector<Addr>::back;
    using std::vector<Addr>::empty;
    using std::vector<Addr>::size;

    using std::vector<Addr>::erase;
    using std::vector<Addr>::push_back;

    using std::vector<Addr>::begin;
    using std::vector<Addr>::end;
    using std::vector<Addr>::cbegin;
    using std::vector<Addr>::cend;
    using std::vector<Addr>::rbegin;

    inline std::ostream& print(std::ostream& os, const TiHeap& heap) const {
      os << "Stck [";
      for (const auto& a : *this) {
        os << a << ": " << heap.lookup(a) << ", ";
      }
      os << ']';
      return os;
    }
  };

  inline std::ostream& operator<<(std::ostream& os, const TiStack& stack) {
    os << "Stck [";
    std::copy(stack.cbegin(), stack.cend(), std::ostream_iterator<TiStack::value_type>(os, ", "));
    os << ']';
    return os;
  }

} /* end namespace kfl */
