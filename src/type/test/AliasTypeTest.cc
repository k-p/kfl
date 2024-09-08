#include "AliasType.h"

#include <boost/test/unit_test.hpp>

using namespace kfl;

namespace {

  class SomeType : public NamedType {
  public:
    SomeType() : NamedType("someName") { }
  };

};

BOOST_AUTO_TEST_SUITE( AliasTypeTest )

BOOST_AUTO_TEST_CASE( AliasTypeEqualsItself )
{
  const SomeType type;

  const AliasType alias("aliasTypeName", type);

  BOOST_CHECK(alias == alias);
}

BOOST_AUTO_TEST_CASE( AliasTypeEqualsTypeItIsAnAliasFor )
{
  const SomeType type;

  const AliasType alias("aliasTypeName", type);

  //BOOST_CHECK(alias == type);
  BOOST_CHECK(type == alias);
}

/*
BOOST_AUTO_TEST_CASE( Equals )
{
  BOOST_CHECK(alias.equals(alias));
  BOOST_CHECK(alias.equals(type));
  //BOOST_CHECK(type.equals(alias));
}
*/
/*
BOOST_AUTO_TEST_CASE( Type )
{
  BOOST_CHECK(&(alias.type()) == static_cast<const Type *>(&type));
}
*/
BOOST_AUTO_TEST_SUITE_END()
