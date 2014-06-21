#include <string>

#include "gtest/gtest.h"
#include "AliasType.h"

namespace {

  class SomeType : public kfl::Type {
    typedef kfl::Type super;
  public:
    SomeType() : super(), m_name("someName") { }
    virtual const std::string & name() const { return m_name; }
  private:
    const std::string m_name;
  };

  const SomeType type;

  const std::string aliasName = "aliasName";
  const kfl::AliasType alias(aliasName, type);
};

TEST(AliasTypeTest, Name)
{
  ASSERT_TRUE(alias.name() == aliasName);
}

TEST(AliasTypeTest, Equals)
{
  ASSERT_TRUE(alias.equals(alias));
  ASSERT_TRUE(alias.equals(type));
  //ASSERT_TRUE(type.equals(alias));
}

TEST(AliasTypeTest, Type)
{
  ASSERT_TRUE(&(alias.type()) == static_cast<const kfl::Type *>(&type));
}
