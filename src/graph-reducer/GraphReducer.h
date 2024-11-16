/* -*-c++-*-
 *
 *  Created by Keith Dennison on 18/01/2014.
 *  Copyright (c) 2014 Keith Dennison. All rights reserved.
 */

#pragma once

#include "CoreExpr.h"

#include <iostream>

namespace kfl {

  class GraphReducer {
  public:
      static void run(const CoreProgram& p, std::ostream& os);
  };

} /* end namespace kfl */
