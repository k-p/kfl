/* -*-c++-*-
 *
 *  Created by Keith Dennison on 18/01/2014.
 *  Copyright (c) 2014 Keith Dennison. All rights reserved.
 */

#pragma once

#include "CoreExpr.h"

#include <iostream>
#include <memory>
#include <string>

namespace kfl {

  class GraphReducer {
  public:
      std::string run(const std::istream& is);

  private:
      std::unique_ptr<CoreProgram> parse(const std::istream& is);
      std::unique_ptr<TiState> compile(const CoreProgram& program);
  };

} /* end namespace kfl */
