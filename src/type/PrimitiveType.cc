#include "PrimitiveType.h"

kfl::PrimitiveType::PrimitiveType(const std::string & name)
  : super(name)
{
}

const std::string kfl::IntType::typeName("Integer");

kfl::IntType::IntType()
  : super(typeName)
{
}
