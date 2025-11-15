/* @(#)TiStats.h
 */

#pragma once

#include <algorithm>
#include <ostream>

namespace kfl {

  class TiStats final {
  public:
    inline void incSteps() { ++s_; }
    inline unsigned getSteps() const { return s_; }

    inline void incSupercomb() { ++sc_; }
    inline unsigned getSupercomb() const { return sc_; }

    inline void incPrimitive() { ++p_; }
    inline unsigned getPrimitive() const { return p_; }

    inline void setMaxStackDepth(unsigned sd) { sd_ = std::max(sd_, sd); }
    inline unsigned getMaxStackDepth() const { return sd_; }

  private:
    unsigned s_ = 0;
    unsigned sc_ = 0;
    unsigned p_ = 0;
    unsigned sd_ = 0;
  };

  std::ostream& operator<<(std::ostream& os, const TiStats& stats);

} // end namespace kfl
