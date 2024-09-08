/* @(#)TiStats.h
 */

#pragma once

namespace kfl {

  class TiStats {
  public:
    void incSteps() { s_++; }
    unsigned getSteps() { return s_; }

  private:
    unsigned s_ = 0;
  };

} /* end namespace */
