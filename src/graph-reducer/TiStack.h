/* @(#)TiStack.h
 */

#pragma once

#include "TiTypes.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

namespace kfl {

  class TiHeap;

  class TiStack : private std::vector<Addr> {
  public:
    inline explicit TiStack(const TiHeap* heap)
      : std::vector<Addr>(), m_heap(heap) { }

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

    inline const TiHeap& heap() const { return *m_heap; }
    inline std::ostream& print(std::ostream& os, const TiHeap& heap) const {
      os << "Stck [";
      for (const auto& a : *this) {
        os << a << ": " << heap.lookup(a) << ", ";
      }
      os << ']';
      return os;
    }

  private:
    const TiHeap* m_heap;
  };

  inline std::ostream& operator<<(std::ostream& os, const TiStack& stack) {
    os << "Stck [";
    std::copy(stack.cbegin(), stack.cend(), std::ostream_iterator<TiStack::value_type>(os, ", "));
    os << ']';
    return os;
  }

} /* end namespace kfl */
