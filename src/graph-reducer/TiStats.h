/* @(#)TiStats.h
 */

#pragma once

#include <ostream>

namespace kfl {

  class TiStats final {
  public:
    inline void incSteps() { s_++; }
    inline unsigned getSteps() const { return s_; }

  private:
    unsigned s_ = 0;
  };

  std::ostream& operator<<(std::ostream& os, const TiStats& stats);

} // end namespace kfl
