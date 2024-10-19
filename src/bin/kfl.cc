#include "CoreExprBuilder.h"
#include "Parser.h"
#include "Pprint.h"
#include "TiState.h"

#include <boost/range/numeric.hpp>

#include <exception>
#include <iostream>
#include <iterator>
#include <vector>

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

  auto allocateSc(std::tuple<TiHeap, TiGlobals> acc, std::shared_ptr<CoreScDefn> defn)
  {
    auto [heap, globals] = acc;
    const auto name = defn->getName();
    const auto binders = defn->getBinders();
    globals[name] = heap.allocSupercomb(name, (binders ? *binders : std::vector<Name>()), defn->getBody());
    return std::tuple(heap, globals);
  }

  auto buildInitialHeap(const CoreProgram& program)
  {
    return boost::accumulate(program.getDefns(), std::tuple(TiHeap(), TiGlobals()), &allocateSc);
  }

  TiState compile(const CoreProgram& program)
  {
    const auto [heap, globals] = buildInitialHeap(program);

    const auto address_of_main = globals.find("main");
    if (address_of_main == globals.end()) {
      throw std::runtime_error("main is not defined");
    }

    TiStack initial_stack = {address_of_main->second};

    return TiState(initial_stack, TiDump(), heap, globals, TiStats());
  }

  TiState applyToStats(TiState state)
  {
    state.stats.incSteps();
    return state;
  }

  TiState step(const TiState& state)
  {
    if (state.stack.empty()) {
      throw std::runtime_error("Stack empty");
    }

    return applyToStats(state.heap.lookup(state.stack.back()).dispatch(state));
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

    auto s = compile(*p);

    std::vector<TiState> states;
    while (!tiFinal(s)) {
      std::cout << s << std::endl;
      s = step(s);
      states.push_back(s);
    }

    std::cout << std::endl << "Trace:" << std::endl << states << std::endl;

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
