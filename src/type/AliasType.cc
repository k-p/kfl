#include "AliasType.h"

kfl::AliasType::AliasType(const std::string & name, const Type & type)
  : super(name), m_type(type)
{
}

bool kfl::AliasType::equals(const Type & other) const
{
  return (static_cast<const Type*>(this) == &other) || m_type.equals(other);
}

const kfl::Type & kfl::AliasType::type() const
{
  return m_type;
}
