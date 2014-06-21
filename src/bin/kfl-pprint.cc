/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include <iostream>
#include <iterator>
#include <string>

#include "boost/scoped_ptr.hpp"

#include "CoreExprBuilder.h"
#include "Parser.h"
#include "Pprint.h"

int main()
{
  using namespace kfl;
  using namespace kfl::Parser;

  std::string storage;
  std::cin.unsetf(std::ios::skipws); // No white space skipping!
  std::copy(std::istream_iterator<char>(std::cin),
	    std::istream_iterator<char>(),
	    std::back_inserter(storage));

  AstProgram ast;
  if (parseProgram(storage.begin(), storage.end(), ast)) {

    CoreExprFactory factory;
    CoreProgramBuilder builder(factory);
    boost::scoped_ptr<CoreProgram> p(builder(ast));

    Pprint<CoreId> pprint(std::cout);
    pprint.visit(*p);
  }
  else {
    std::cerr << "Parsing error\n";
  }

  return 0;
}
