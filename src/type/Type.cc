#include "Type.h"

kfl::Type::~Type()
{
}

bool kfl::Type::equals(const Type & other) const
{
  return this == &other;
}
