#include "CoreExprBuilder.h"
#include "GraphReducer.h"
#include "Parser.h"
#include "Pprint.h"

#include <exception>
#include <iostream>

using namespace kfl;
using namespace kfl::Parser;

namespace {

  template<typename Iterator>
  auto parse(Iterator begin, Iterator end)
  {
    AstProgram ast;

    if (!parseProgram(begin, end, ast)) {
      throw std::runtime_error("Parsing error");
    }

    CoreFactory factory;
    CoreProgramBuilder builder(factory);
    return builder(ast);
  }

}

int main()
{
  try {
    std::string storage;
    std::cin.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(std::istream_iterator<char>(std::cin),
              std::istream_iterator<char>(),
              std::back_inserter(storage));

    auto p = parse(storage.begin(), storage.end());

    Pprint<CoreId> pprint(std::cout);
    pprint.visit(*p);
    std::cout << std::endl;

    GraphReducer::run(*p, std::cout);
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << '.' << std::endl;
    return EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "ERROR: Caught unkown exception." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
