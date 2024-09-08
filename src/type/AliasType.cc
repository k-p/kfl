#include "AliasType.h"

kfl::AliasType::AliasType(const std::string & name, const Type & type)
  : super(name), m_type(type)
{
}

bool kfl::AliasType::equals(const Type & other) const
{
  return super::equals(other) || (type() == other);
}

const kfl::Type & kfl::AliasType::type() const
{
  return m_type;
}
