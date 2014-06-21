#include "gtest/gtest.h"
#include "ListType.h"

TEST(ListTypeTest, ListTypeName)
{
  const kfl::ListType type;
  ASSERT_TRUE(type.name() == kfl::ListType::cName);
}

TEST(ListTypeTest, ListTypeEquality)
{
  const kfl::ListType type;
  ASSERT_TRUE(type.equals(type));

  const kfl::ListType othertype;
  ASSERT_TRUE(!type.equals(othertype));
}
