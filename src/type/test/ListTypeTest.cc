#include "ListType.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( ListTypeTest )

BOOST_AUTO_TEST_CASE( ListTypeName )
{
  const kfl::ListType type;
  BOOST_CHECK_EQUAL(type.name(), kfl::ListType::cName);
}
/*
BOOST_AUTO_TEST_CASE( ListTypeEquality )
{
  const kfl::ListType type;
  BOOST_CHECK_EQUALS(type, type);

  const kfl::ListType othertype;
  BOOST_CHECK_EQUALS(type, othertype);
}
*/
BOOST_AUTO_TEST_SUITE_END()
