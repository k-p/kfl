/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include "ExprFactory.h"

#include "CoreExpr.h"

#include <boost/test/unit_test.hpp>

using namespace kfl;
using std::shared_ptr;

namespace {
  CoreFactory factory;
}

BOOST_AUTO_TEST_SUITE( ExprFactoryTest )

BOOST_AUTO_TEST_CASE( EVar )
{
  auto var = factory.createVar("variable_name");

  BOOST_CHECK_EQUAL(var->isAtomic(), true);
  BOOST_CHECK_EQUAL(var->getId(), "variable_name");
}

BOOST_AUTO_TEST_CASE( ENum )
{
  auto num = factory.createNum(42);

  BOOST_CHECK_EQUAL(num->isAtomic(), true);
  BOOST_CHECK_EQUAL(num->getNum(), 42);
}

BOOST_AUTO_TEST_CASE( EConstr )
{
  const int cTag = 1;
  const int cArity = 2;
  auto constr = factory.createConstr(cTag, cArity);

  BOOST_CHECK_EQUAL(constr->isAtomic(), false);
  BOOST_CHECK_EQUAL(constr->getTag(), cTag);
  BOOST_CHECK_EQUAL(constr->getArity(), cArity);
}

BOOST_AUTO_TEST_CASE( EAp )
{
  shared_ptr<CoreExpr> fn = factory.createVar("fn");
  shared_ptr<CoreExpr> arg = factory.createNum(0);
  auto ap = factory.createAp(fn, arg);

  BOOST_CHECK_EQUAL(ap->isAtomic(), false);
  BOOST_CHECK_EQUAL(&ap->getFn(), fn.get());
  BOOST_CHECK_EQUAL(&ap->getArg(), arg.get());
}

BOOST_AUTO_TEST_CASE( ELet )
{
  shared_ptr<CoreFactory::DefnVec> defns = factory.createDefns();
  factory.addDefn(*defns, "x", factory.createNum(0));
  factory.addDefn(*defns, "y", factory.createNum(1));
  shared_ptr<CoreExpr> body = factory.createVar("x");
  auto let = factory.createLet(defns, body);

  BOOST_CHECK_EQUAL(let->isAtomic(), false);
  BOOST_CHECK_EQUAL(let->isRec(), false);
  BOOST_CHECK_EQUAL(&let->getDefns(), defns.get());
  BOOST_CHECK_EQUAL(&let->getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( ELetRec )
{
  shared_ptr<CoreFactory::DefnVec> defns = factory.createDefns();
  factory.addDefn(*defns, "x", factory.createVar("y"));
  factory.addDefn(*defns, "y", factory.createVar("x"));
  shared_ptr<CoreExpr> body = factory.createVar("x");
  auto letrec = factory.createLetRec(defns, body);

  BOOST_CHECK_EQUAL(letrec->isAtomic(), false);
  BOOST_CHECK_EQUAL(letrec->isRec(), true);
  BOOST_CHECK_EQUAL(&letrec->getDefns(), defns.get());
  BOOST_CHECK_EQUAL(&letrec->getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( ECase )
{
  shared_ptr<CoreFactory::AlterVec> alters = factory.createAlters();
  factory.addAlter(*alters, factory.createNum(1), nullptr, factory.createVar("x"));
  shared_ptr<CoreFactory::BndVec> bnds = factory.createBnds();
  factory.addBnd(*bnds, "y");
  factory.addBnd(*bnds, "z");
  factory.addAlter(*alters, factory.createNum(2), bnds, factory.createVar("y"));
  shared_ptr<CoreExpr> expr = factory.createVar("x");
  auto case_ = factory.createCase(expr, alters);

  BOOST_CHECK_EQUAL(case_->isAtomic(), false);
  BOOST_CHECK_EQUAL(&case_->getExpr(), expr.get());
  BOOST_CHECK_EQUAL(&case_->getAlters(), alters.get());
}

BOOST_AUTO_TEST_CASE( ELambda )
{
  shared_ptr<CoreFactory::BndVec> bnds = factory.createBnds();
  factory.addBnd(*bnds, "x");
  factory.addBnd(*bnds, "y");
  shared_ptr<CoreExpr> body = factory.createVar("x");
  auto lambda = factory.createLambda(bnds, body);

  BOOST_CHECK_EQUAL(lambda->isAtomic(), false);
  BOOST_CHECK_EQUAL(&lambda->getBinders(), bnds.get());
  BOOST_CHECK_EQUAL(&lambda->getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( ScDefnWithoutArgs )
{
  const std::string name("test");
  shared_ptr<CoreExpr> body = factory.createVar("x");
  auto defn = factory.createScDefn(name, nullptr, body);

  BOOST_CHECK_EQUAL(defn->getName(), name);
  BOOST_CHECK_EQUAL(defn->getBinders(), nullptr);
  BOOST_CHECK_EQUAL(&defn->getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( ScDefnWithArgs )
{
  const std::string name("test");
  shared_ptr<CoreFactory::BndVec> bnds = factory.createBnds();
  factory.addBnd(*bnds, "x");
  factory.addBnd(*bnds, "y");
  shared_ptr<CoreExpr> body = factory.createVar("x");
  auto defn = factory.createScDefn(name, bnds, body);

  BOOST_CHECK_EQUAL(defn->getName(), name);
  BOOST_CHECK_EQUAL(defn->getBinders(), bnds.get());
  BOOST_CHECK_EQUAL(&defn->getBody(), body.get());
}

BOOST_AUTO_TEST_CASE( Program )
{
  std::shared_ptr<CoreFactory::ScDefnVec> defns = factory.createScDefns();
  factory.addScDefn(*defns, "test1", nullptr, factory.createVar("x"));
  factory.addScDefn(*defns, "test2", nullptr, factory.createVar("y"));
  auto program = factory.createProgram(defns);

  BOOST_CHECK_EQUAL(&program->getDefns(), defns.get());
}

BOOST_AUTO_TEST_SUITE_END()
