/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include "CoreExprBuilder.h"
#include "Parser.h"
#include "Pprint.h"

#include <iostream>
#include <iterator>
#include <string>

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
    CoreFactory factory;
    CoreProgramBuilder builder(factory);
    auto p = builder(ast);

    Pprint<CoreId> pprinter(std::cout);
    pprinter.visit(*p);
  }
  else {
    std::cerr << "Parsing error\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
