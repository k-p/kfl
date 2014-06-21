#include "TupleType.h"

kfl::TupleType::TupleType()
  : super(), m_types()
{
}

kfl::TupleType::TupleType(const TypeVector & types)
  : super(), m_types(types)
{
}

kfl::TupleType::~TupleType()
{
}

const std::string & kfl::TupleType::name() const
{
  if (m_cachedName.empty()) {
    m_cachedName = computeName();
  }
  return m_cachedName;
}

std::string kfl::TupleType::computeName() const
{
  const char open = '(';
  const char close = ')';
  const std::string separator = " * ";

  std::string name;
  name += open;
  TypeVector::const_iterator t = m_types.begin();
  if (t != m_types.end()) {
    if (*t) {
      name += (*t)->name();
    }
    ++t;
  }
  while (t != m_types.end()) {
    if (*t) {
      name += separator;
      name += (*t)->name();
    }
    ++t;
  }
  name += close;
  return name;
}
