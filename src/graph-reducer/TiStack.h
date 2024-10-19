/* @(#)TiStack.h
 */

#pragma once

#include "TiTypes.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

namespace kfl {

  using TiStack = std::vector<Addr>;

  inline std::ostream& operator<<(std::ostream& os, const TiStack& stack) {
    os << "Stck [";
    std::copy(stack.cbegin(), stack.cend(), std::ostream_iterator<Addr>(os, ", "));
    os << ']';
    return os;
  }

} /* end namespace kfl */
