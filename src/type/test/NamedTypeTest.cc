#include "gtest/gtest.h"
#include "NamedType.h"

namespace {

  class TestSubclass : public kfl::NamedType {
    typedef kfl::NamedType super;
  public:
    TestSubclass(const std::string & name) : super(name) { }
  };

  const std::string typeName1 = "typeName1";
  const TestSubclass type1(typeName1);

  const std::string typeName2 = "typeName2";
  const TestSubclass type2(typeName2);

  const TestSubclass type3(typeName1);
};

TEST(NamedTypeTest, Name)
{
  ASSERT_TRUE(type1.name() == typeName1);
}

TEST(NamedTypeTest, Equals)
{
  ASSERT_TRUE(type1.equals(type1));
  ASSERT_TRUE(!type1.equals(type2));
  ASSERT_TRUE(!type1.equals(type3));
}
