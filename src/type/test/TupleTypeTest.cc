#include <string>

#include "gtest/gtest.h"

#include "TupleType.h"

namespace {

  class SomeType : public kfl::Type {
    typedef kfl::Type super;
  public:
    SomeType(const std::string & name) : super(), m_name(name) { }
    virtual const std::string & name() const { return m_name; }
  private:
    const std::string m_name;
  };

  const kfl::TupleType emptyTupleType;

};

TEST(TupleTypeTest, Name)
{
  ASSERT_TRUE(emptyTupleType.name() == "()");

  kfl::TupleType::TypeVector tv;
  SomeType t1("t1");
  tv.push_back(&t1);
  SomeType t2("t2");
  tv.push_back(&t2);
  const kfl::TupleType type(tv);
  ASSERT_TRUE(type.name() == "(t1 * t2)");
}

TEST(TupleTypeTest, Equals)
{
  ASSERT_TRUE(emptyTupleType.equals(emptyTupleType));
}
