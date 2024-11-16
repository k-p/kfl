/* @(#)TiGlobals.h
 */

#pragma once

#include "TiHeap.h"

#include <map>

namespace kfl {

  using TiGlobals = std::map<TiHeap::Name, TiHeap::Addr>;

} /* end namespace kfl */
