#include "TiStats.h"

std::ostream& kfl::operator<<(std::ostream& os, const TiStats& stats)
{
    return os << "Total number of steps = " << stats.getSteps() << std::endl
              << "Total number of supercombinator reductions = " << stats.getSupercomb() << std::endl
              << "Total number of primitive reductions = " << stats.getPrimitive() << std::endl
              << "Total number of indirections = " << stats.getInd() << std::endl
              << "Maximum stack depth = " << stats.getMaxStackDepth() << std::endl;
}
