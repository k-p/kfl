/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include "CoreExpr.h"

#include <boost/test/unit_test.hpp>

using namespace kfl;
using std::make_shared;

BOOST_AUTO_TEST_SUITE( ExprTest )

BOOST_AUTO_TEST_CASE( EVar )
{
  const CoreVar e("variable_name");

  BOOST_CHECK_EQUAL(e.isAtomic(), true);
  BOOST_CHECK_EQUAL(e.getId(), "variable_name");
}

BOOST_AUTO_TEST_CASE( ENum )
{
  const CoreNum e(1);

  BOOST_CHECK_EQUAL(e.isAtomic(), true);
  BOOST_CHECK_EQUAL(e.getNum(), 1);
}

BOOST_AUTO_TEST_CASE( EConstr )
{
  const int cTag = 1;
  const int cArity = 2;
  const CoreConstr e(cTag,cArity);

  BOOST_CHECK_EQUAL(e.isAtomic(), false);
  BOOST_CHECK_EQUAL(e.getTag(), cTag);
  BOOST_CHECK_EQUAL(e.getArity(), cArity);
}

BOOST_AUTO_TEST_CASE( EAp )
{
  auto f = make_shared<CoreVar>("fn");
  auto arg = make_shared<CoreNum>(0);
  const CoreAp e(f, arg);

  BOOST_CHECK_EQUAL(e.isAtomic(), false);
  BOOST_CHECK_EQUAL(&e.getFn(), f.get());
  BOOST_CHECK_EQUAL(&e.getArg(), arg.get());
}

BOOST_AUTO_TEST_CASE( ELet )
{
  CoreLet::Defn d1("x", make_shared<CoreNum>(0));
  CoreLet::Defn d2("y", make_shared<CoreNum>(1));
  auto defns = make_shared<CoreLet::DefnVec>();
  defns->push_back(d1);
  defns->push_back(d2);
  auto body = make_shared<CoreVar>("x");
  const CoreLet e(defns, body);

  BOOST_CHECK_EQUAL(e.isAtomic(), false);
  BOOST_CHECK_EQUAL(&e.getDefns(), defns.get());
  BOOST_CHECK_EQUAL(&e.getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( ECase )
{
  CoreCase::Alter a1 = { make_shared<CoreNum>(1), nullptr, make_shared<CoreVar>("x") };
  auto binders = make_shared<CoreCase::BndVec>();
  binders->push_back("y");
  binders->push_back("z");
  CoreCase::Alter a2  = { make_shared<CoreNum>(2), binders, make_shared<CoreVar>("y") };
  auto alters = make_shared<CoreCase::AlterVec>();
  alters->push_back(a1);
  alters->push_back(a2);
  auto expr = make_shared<CoreVar>("x");
  const CoreCase e(expr, alters);

  BOOST_CHECK_EQUAL(e.isAtomic(), false);
  BOOST_CHECK_EQUAL(&e.getExpr(), expr.get());
  BOOST_CHECK_EQUAL(&e.getAlters(), alters.get());
}

BOOST_AUTO_TEST_CASE( ELam )
{
  auto binders = make_shared<CoreLam::BndVec>();
  binders->push_back("x");
  binders->push_back("y");
  auto body = make_shared<CoreVar>("x");
  const CoreLam e(binders, body);

  BOOST_CHECK_EQUAL(e.isAtomic(), false);
  BOOST_CHECK_EQUAL(&e.getBinders(), binders.get());
  BOOST_CHECK_EQUAL(&e.getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( ScDefnWithoutArgs )
{
  auto body = make_shared<CoreVar>("x");
  CoreScDefn defn("test", nullptr, body);

  BOOST_CHECK_EQUAL(defn.getName(), "test");
  BOOST_CHECK_EQUAL(defn.getBinders(), nullptr);
  BOOST_CHECK_EQUAL(&defn.getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( ScDefnWithArgs )
{
  auto binders = make_shared<CoreScDefn::BndVec>();
  binders->push_back("x");
  binders->push_back("y");
  auto body = make_shared<CoreVar>("x");
  CoreScDefn defn("test", binders, body);

  BOOST_CHECK_EQUAL(defn.getName(), "test");
  BOOST_CHECK_EQUAL(defn.getBinders(), binders.get());
  BOOST_CHECK_EQUAL(&defn.getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( Program )
{
  auto defn1 = make_shared<CoreScDefn>("test1", nullptr, make_shared<CoreVar>("x"));
  auto defn2 = make_shared<CoreScDefn>("test2", nullptr, make_shared<CoreVar>("y"));
  auto defns = make_shared<CoreProgram::ScDefnVec>();
  defns->push_back(defn1);
  defns->push_back(defn2);

  CoreProgram program(defns);

  BOOST_CHECK_EQUAL(&program.getDefns(), defns.get());
}

BOOST_AUTO_TEST_SUITE_END()
