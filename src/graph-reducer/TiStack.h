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

    inline Addr peek() const {
      if (empty()) {
        throw std::runtime_error("Stack underflow");
      }
      return back();
    }
    inline void pop(size_t n = 1) {
      if (size() < n) {
        throw std::runtime_error("Stack underflow");
      }
      erase(end() - n, end());
    }
    inline void push(Addr addr) { push_back(addr); }

    using std::vector<Addr>::empty;
    using std::vector<Addr>::size;

    using std::vector<Addr>::clear;

    using std::vector<Addr>::crbegin;
    using std::vector<Addr>::crend;

    inline std::ostream& print(std::ostream& os, const TiHeap& heap) const {
      os << "Stck [";
      for (const auto& a : *this) {
        os << a << ": " << heap.lookup(a) << ", ";
      }
      os << ']';
      return os;
    }
  };

} /* end namespace kfl */
