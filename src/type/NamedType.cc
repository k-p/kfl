#include "NamedType.h"

kfl::NamedType::NamedType(const std::string & name)
  : m_name(name)
{
}

const std::string & kfl::NamedType::name() const
{
  return m_name;
}
