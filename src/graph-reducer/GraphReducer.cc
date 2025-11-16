/* -*-c++-*-
 *
 *  Created by Keith Dennison on 16/11/2024.
 *  Copyright (c) 2024 Keith Dennison. All rights reserved.
 */

#include "GraphReducer.h"

#include "TiState.h"

#include <boost/range/numeric.hpp>

#include <exception>
#include <iostream>

using namespace kfl;

namespace {

  auto allocateSc(std::tuple<TiHeap, TiGlobals> acc, std::shared_ptr<CoreScDefn> defn)
  {
    auto [heap, globals] = acc;
    const auto name = defn->getName();
    const auto binders = defn->getBinders();
    globals[name] = heap.allocSupercomb(name, (binders ? *binders : std::vector<TiHeap::Name>()), defn->getBody());
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

    TiStack initial_stack;
    initial_stack.push_back(address_of_main->second);

    return TiState(initial_stack, TiDump(), heap, globals, TiStats());
  }

  TiState applyToStats(TiState state)
  {
    state.stats.incSteps();
    state.stats.setMaxStackDepth(state.stack.size());
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

void GraphReducer::run(const CoreProgram& p, std::ostream& os)
{
    auto s = compile(p);

    std::vector<TiState> states = {s};
    while (!tiFinal(s)) {
      s = step(s);
      states.push_back(s);
    }

    os << "Trace:" << std::endl << states << std::endl
       << "Stats:" << std::endl << states.back().stats
       << "Total number of allocations = " << states.back().heap.getAllocs() << std::endl
       << "Total number of updates = " << states.back().heap.getUpdates() << std::endl;
}
