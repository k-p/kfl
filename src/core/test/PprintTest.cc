/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include "Pprint.h"

#include "CoreExpr.h"

#include <boost/test/unit_test.hpp>
#include <sstream>

using namespace kfl;
using TestPprint = Pprint<std::string>;
using TestPprintA = PprintA<std::string>;
using std::make_shared;
using std::make_unique;

BOOST_AUTO_TEST_SUITE( PprintTest )

BOOST_AUTO_TEST_CASE( EVar )
{
  std::ostringstream output;

  TestPprint(output)
    .visit(CoreVar("variable_name"));

  BOOST_CHECK_EQUAL(output.str(), "variable_name");
}

BOOST_AUTO_TEST_CASE( ENum )
{
  std::ostringstream output;
 
  TestPprint(output)
    .visit(CoreNum(42));

  BOOST_CHECK_EQUAL(output.str(), "42");
}

BOOST_AUTO_TEST_CASE( EConstr )
{
  std::ostringstream output;
  const int cTag = 1;
  const int cArity = 2;

  TestPprint(output)
    .visit(CoreConstr(cTag,cArity));

  BOOST_CHECK_EQUAL(output.str(), "Pack{1,2}");
}

BOOST_AUTO_TEST_CASE( EAp )
{
  std::ostringstream output;

  TestPprint(output)
    .visit(CoreAp(make_unique<CoreVar>("fn_name"),
                  make_unique<CoreNum>(42)));

  BOOST_CHECK_EQUAL(output.str(), "fn_name 42");
}

BOOST_AUTO_TEST_CASE( ELet )
{
  std::ostringstream output;

  CoreLet::Defn d1("x", new CoreNum(0));
  CoreLet::Defn d2("y", new CoreNum(1));
  auto defns = make_unique<CoreLet::DefnVec>();
  defns->push_back(d1);
  defns->push_back(d2);

  TestPprint(output)
    .visit(CoreLet(std::move(defns), make_unique<CoreVar>("x")));

  BOOST_CHECK_EQUAL(output.str(),
                    "let\n"
                    "  x = 0;\n"
                    "  y = 1\n"
                    "in x");
}

BOOST_AUTO_TEST_CASE( ECase )
{
  std::ostringstream output;

  auto binders1 = make_shared<CoreCase::BndVec>();
  CoreCase::Alter a1  = { make_shared<CoreNum>(1), binders1, make_shared<CoreVar>("x") };
  auto binders2 = make_shared<CoreCase::BndVec>();
  binders2->push_back("y");
  binders2->push_back("z");
  CoreCase::Alter a2  = { make_shared<CoreNum>(2), binders2, make_shared<CoreVar>("y") };
  auto alters = make_shared<CoreCase::AlterVec>();
  alters->push_back(a1);
  alters->push_back(a2);

  TestPprint(output)
    .visit(CoreCase(make_unique<CoreVar>("x"), alters));

  BOOST_CHECK_EQUAL(output.str(),
                    "case x of\n"
                    "  <1> -> x;\n"
                    "  <2> y z -> y");
}

BOOST_AUTO_TEST_CASE( ELam )
{
  std::ostringstream output;

  auto binders = make_shared<CoreLam::BndVec>();
  binders->push_back("x");
  binders->push_back("y");

  TestPprint(output)
    .visit(CoreLam(binders, make_unique<CoreVar>("x")));

  BOOST_CHECK_EQUAL(output.str(), "\\ x y. x");
}

BOOST_AUTO_TEST_CASE( ScDefnWithoutArgs )
{
  std::ostringstream output;

  TestPprint(output)
    .visit(CoreScDefn("test", 0, make_unique<CoreVar>("x")));

  BOOST_CHECK_EQUAL(output.str(), "test = x");
}

BOOST_AUTO_TEST_CASE( ScDefnWithArgs )
{
  std::ostringstream output;

  auto binders = make_shared<CoreScDefn::BndVec>();
  binders->push_back("x");
  binders->push_back("y");

  TestPprint(output)
    .visit(CoreScDefn("test", binders, make_unique<CoreVar>("x")));

  BOOST_CHECK_EQUAL(output.str(), "test x y = x");
}

BOOST_AUTO_TEST_CASE( Program )
{
  std::ostringstream output;

  auto defn1 = make_shared<CoreScDefn>("test1", nullptr, make_unique<CoreVar>("x"));
  auto defn2 = make_shared<CoreScDefn>("test2", nullptr, make_unique<CoreVar>("y"));
  auto defns = make_shared<CoreProgram::ScDefnVec>();
  defns->push_back(defn1);
  defns->push_back(defn2);

  TestPprint(output)
    .visit(CoreProgram(defns));

  BOOST_CHECK_EQUAL(output.str(),
		    "test1 = x;\n"
		    "test2 = y\n");
}

BOOST_AUTO_TEST_CASE( AtomicPprintA )
{
  std::ostringstream output;

  TestPprintA(output)
    .visit(CoreVar("variable_name"));

  BOOST_CHECK_EQUAL(output.str(), "variable_name");
}

BOOST_AUTO_TEST_CASE( NonAtomicPprintA )
{
  std::ostringstream output;

  TestPprintA(output)
    .visit(CoreAp(make_unique<CoreVar>("fn"),
                  make_unique<CoreNum>(0)));

  BOOST_CHECK_EQUAL(output.str(), "(fn 0)");
}

BOOST_AUTO_TEST_CASE( StreamOutOp )
{
  std::ostringstream output;
  output << CoreVar("variable_name");

  BOOST_CHECK_EQUAL(output.str(), "variable_name");
}

BOOST_AUTO_TEST_SUITE_END()
