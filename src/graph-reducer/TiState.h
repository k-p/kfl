/* @(#)TiState.h
 */

#pragma once

#include "TiDump.h"
#include "TiGlobals.h"
#include "TiHeap.h"
#include "TiStack.h"
#include "TiStats.h"

namespace kfl {

  template<typename Stack, typename Dump, typename Heap, typename Globals, typename Stats>
  struct State
  {
  public:
    inline State(const Stack& s, const Dump& d, const Heap& h, const Globals& g, const Stats& t)
      : stack(s), dump(d), heap(h), globals(g), stats(t) { }
    Stack stack;
    Dump dump;
    Heap heap;
    Globals globals;
    Stats stats;
  };

  class TiState : public State<TiStack, TiDump, TiHeap, TiGlobals, TiStats>
  {
    using super = State<TiStack, TiDump, TiHeap, TiGlobals, TiStats>;
  public:
    inline TiState(const TiStack& s, const TiDump& d, const TiHeap& h, const TiGlobals& g, const TiStats& t)
      : super(s, d, h, g, t) { }
  };

  inline bool tiFinal(const TiState& state)
  {
    return state.stack.size() == 1
      && state.heap.lookup(state.stack.back()).isDataNode();
  }

} /* end namespace kfl */
