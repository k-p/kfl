#include "TupleType.h"

#include <boost/test/unit_test.hpp>

#include <string>

namespace {

  class SomeType : public kfl::Type {
    typedef kfl::Type super;
  public:
    SomeType(const std::string & name) : super(), m_name(name) { }
    virtual const std::string & name() const { return m_name; }
  private:
    const std::string m_name;
  };

  std::ostream & operator<<(std::ostream & cout, const SomeType& type) {
    return cout << "[SomeType: " << &type << ']';
  }

  const kfl::TupleType emptyTupleType;

};

BOOST_AUTO_TEST_SUITE( TupleTypeTest )

BOOST_AUTO_TEST_CASE( Name )
{
  BOOST_CHECK_EQUAL(emptyTupleType.name(), "()");

  kfl::TupleType::TypeVector tv;
  SomeType t1("t1");
  tv.push_back(&t1);
  SomeType t2("t2");
  tv.push_back(&t2);
  const kfl::TupleType type(tv);
  BOOST_CHECK_EQUAL(type.name(), "(t1 * t2)");
}

BOOST_AUTO_TEST_CASE( Equals )
{
  //BOOST_CHECK_EQUAL(emptyTupleType, emptyTupleType);
}

BOOST_AUTO_TEST_SUITE_END()
