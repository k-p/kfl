#include "NamedType.h"

#include <boost/test/unit_test.hpp>

using namespace kfl;

namespace {

  class TestType : public NamedType {
  public:
    TestType(const std::string & name) : NamedType(name) { }
  };

}

BOOST_AUTO_TEST_SUITE( NamedTypeTest )

BOOST_AUTO_TEST_CASE( NamedTypeReturnsGivenName )
{
  BOOST_CHECK_EQUAL(TestType("type_name").name(), "type_name");
}

BOOST_AUTO_TEST_SUITE_END()
