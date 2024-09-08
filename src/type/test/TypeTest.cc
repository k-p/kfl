#include "Type.h"

#include <boost/test/unit_test.hpp>

using namespace kfl;

namespace {

  class TestType : public Type {
  public:
    const std::string & name() const { static std::string n = "TestName"; return n; }
  };

}

BOOST_AUTO_TEST_SUITE( TypeTest )

BOOST_AUTO_TEST_CASE( TypeEqualsItself )
{
  const TestType type;

  BOOST_CHECK(type == type);
}

BOOST_AUTO_TEST_CASE( TypeDoesNotEqualAnotherType )
{
  TestType type1;
  TestType type2;

  BOOST_CHECK(!(type1 == type2));
}

BOOST_AUTO_TEST_CASE( TypeIncludesNameWhenPrinted )
{
  std::ostringstream output;
  output << TestType();

  BOOST_CHECK_EQUAL(output.str(), "[Type: TestName]");
}

BOOST_AUTO_TEST_SUITE_END()
