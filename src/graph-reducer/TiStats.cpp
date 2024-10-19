#include "TiStats.h"

std::ostream& kfl::operator<<(std::ostream& os, const TiStats& stats)
{
    return os << "Total number of steps = " << stats.getSteps(); 
}
