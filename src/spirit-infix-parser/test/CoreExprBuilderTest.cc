/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include "CoreExprBuilder.h"

#include "Parser.h"

#include <boost/test/unit_test.hpp>

using namespace kfl;
using namespace kfl::Parser;

namespace {
    CoreFactory factory;
    CoreExprBuilder builder(factory);
}

BOOST_AUTO_TEST_SUITE( CoreExprBuilderTest )

BOOST_AUTO_TEST_CASE( Var )
{
  const AstExpr ast = AstVar("variable_name");

  auto e = boost::apply_visitor(builder, ast);

  const CoreVar * var = dynamic_cast<const CoreVar *>(e.get());
  BOOST_CHECK(var != nullptr);
  BOOST_CHECK_EQUAL(var->getId(), "variable_name");
}

BOOST_AUTO_TEST_CASE( Num )
{
    const AstExpr ast = AstNum(42);

    auto e = boost::apply_visitor(builder, ast);

    const CoreNum * num = dynamic_cast<const CoreNum *>(e.get());
    BOOST_CHECK_EQUAL(num->getNum(), 42);
}

BOOST_AUTO_TEST_CASE( Constr )
{
    const int cTag = 1;
    const int cArity = 2;
    const AstExpr ast = AstConstr{cTag, cArity};

    auto e = boost::apply_visitor(builder, ast);

    const CoreConstr * constr = dynamic_cast<const CoreConstr *>(e.get());
    BOOST_CHECK_EQUAL(constr->getTag(), cTag);
    BOOST_CHECK_EQUAL(constr->getArity(), cArity);
}

BOOST_AUTO_TEST_CASE( Ap )
{
    // "fn 1 2" == "((fn 1) 2)"
    const AstExpr ast = AstExprVec{ AstVar("fn"), AstNum(1), AstNum(2) };

    auto e = boost::apply_visitor(builder, ast);

    const CoreAp * apOuter = dynamic_cast<const CoreAp *>(e.get());
    const CoreAp & apInner = dynamic_cast<const CoreAp &>(apOuter->getFn());
    const CoreVar & fn = dynamic_cast<const CoreVar &>(apInner.getFn());
    BOOST_CHECK_EQUAL(fn.getId(), "fn");
    const CoreNum & arg1 = dynamic_cast<const CoreNum &>(apInner.getArg());
    BOOST_CHECK_EQUAL(arg1.getNum(), 1);
    const CoreNum & arg2 = dynamic_cast<const CoreNum &>(apOuter->getArg());
    BOOST_CHECK_EQUAL(arg2.getNum(), 2);
}

BOOST_AUTO_TEST_CASE( Let )
{
  const AstExpr ast = AstLet{
    AstDefnVec{ AstDefn{ AstVar("x"), AstNum(0) },
      AstDefn{ AstVar("y"), AstNum(1) } },
    AstVar("x")
  };

  auto e = boost::apply_visitor(builder, ast);

  const CoreLet * let = dynamic_cast<const CoreLet *>(e.get());
  BOOST_CHECK_EQUAL(let->isRec(), false);

  const CoreLet::DefnVec & defns = let->getDefns();
  BOOST_CHECK_EQUAL(defns.size(), 2);
  BOOST_CHECK_EQUAL(defns[0].first, "x");
  const CoreNum * num0 = dynamic_cast<const CoreNum *>(defns[0].second.get());
  BOOST_CHECK_EQUAL(num0->getNum(), 0);
  BOOST_CHECK_EQUAL(defns[1].first, "y");
  const CoreNum * num1 = dynamic_cast<const CoreNum *>(defns[1].second.get());
  BOOST_CHECK_EQUAL(num1->getNum(), 1);

  const CoreVar & var = dynamic_cast<const CoreVar &>(let->getBody());
  BOOST_CHECK_EQUAL(var.getId(), "x");
}

BOOST_AUTO_TEST_CASE( LetRec )
{
  const AstExpr ast = AstLetRec{
    AstDefnVec{ AstDefn{ AstVar("x"), AstNum(0) },
      AstDefn{ AstVar("y"), AstNum(1) } },
    AstVar("x")
  };

  auto e = boost::apply_visitor(builder, ast);

  const CoreLet * let = dynamic_cast<const CoreLet *>(e.get());
  BOOST_CHECK_EQUAL(let->isRec(), true);

  const CoreLet::DefnVec & defns = let->getDefns();
  BOOST_CHECK_EQUAL(defns.size(), 2);
  BOOST_CHECK_EQUAL(defns[0].first, "x");
  const CoreNum * num0 = dynamic_cast<const CoreNum *>(defns[0].second.get());
  BOOST_CHECK_EQUAL(num0->getNum(), 0);
  BOOST_CHECK_EQUAL(defns[1].first, "y");
  const CoreNum * num1 = dynamic_cast<const CoreNum *>(defns[1].second.get());
  BOOST_CHECK_EQUAL(num1->getNum(), 1);

  const CoreVar & var = dynamic_cast<const CoreVar &>(let->getBody());
  BOOST_CHECK_EQUAL(var.getId(), "x");
}

BOOST_AUTO_TEST_CASE( Case )
{
  AstExpr ast = AstCase{
    AstVar("x"),
    AstAltVec{
      AstAlt{ AstNum(1), AstVarVec{ }, AstVar("x") },
      AstAlt{ AstNum(2), AstVarVec{ AstVar("y"), AstVar("z") }, AstVar("y") } }
  };

  auto e = boost::apply_visitor(builder, ast);

  auto c = dynamic_cast<const CoreCase *>(e.get());

  const auto & var = dynamic_cast<const CoreVar &>(c->getExpr());
  BOOST_CHECK_EQUAL(var.getId(), "x");

  const auto & alters = c->getAlters();
  BOOST_CHECK_EQUAL(alters.size(), 2);
  BOOST_CHECK_EQUAL(alters[0].num->getNum(), 1);
  BOOST_CHECK_EQUAL(alters[1].num->getNum(), 2);

  BOOST_CHECK_EQUAL(alters[0].binders, nullptr);
  const CoreCase::BndVec binders{"y", "z"};
  BOOST_CHECK_EQUAL_COLLECTIONS(alters[1].binders->begin(), alters[1].binders->end(),
                                binders.begin(), binders.end());

  const auto & expr0 = dynamic_cast<const CoreVar *>(alters[0].expr.get());
  BOOST_CHECK_EQUAL(expr0->getId(), "x");
  const auto & expr1 = dynamic_cast<const CoreVar *>(alters[1].expr.get());
  BOOST_CHECK_EQUAL(expr1->getId(), "y");
}

BOOST_AUTO_TEST_CASE( Lambda )
{
  const AstExpr ast = AstLambda{
    AstVarVec{ AstVar("x"), AstVar("y"), AstVar("z") },
    AstVar("x")
  };

  auto e = boost::apply_visitor(builder, ast);

  auto l = dynamic_cast<CoreLam *>(e.get());
  const CoreLam::BndVec binders{"x", "y", "z"};
  BOOST_CHECK_EQUAL_COLLECTIONS(l->getBinders().begin(), l->getBinders().end(),
                                binders.begin(), binders.end());
  const auto & var = dynamic_cast<const CoreVar &>(l->getBody());
  BOOST_CHECK_EQUAL(var.getId(), "x");
}

BOOST_AUTO_TEST_CASE( Program )
{
  const AstProgram ast = AstProgram{
    AstScDefn{ AstVar("main"), AstVarVec{}, AstExprVec{ AstVar("double"), AstNum(21) } },
    AstScDefn{ AstVar("double"), AstVarVec{ AstVar("x") }, AstExprVec{ AstVar("plus"), AstVar("x"), AstVar("x") } },
    AstScDefn{ AstVar("K1"), AstVarVec{ AstVar("y"), AstVar("z") }, AstExprVec{ AstVar("z") } }
  };

  auto p = CoreProgramBuilder(factory)(ast);

  const auto & defns = p->getDefns();
  BOOST_CHECK_EQUAL(defns.size(), 3);

  BOOST_CHECK_EQUAL(defns[0]->getName(), "main");
  BOOST_CHECK_EQUAL(defns[1]->getName(), "double");
  BOOST_CHECK_EQUAL(defns[2]->getName(), "K1");

  BOOST_CHECK_EQUAL(defns[0]->getBinders(), nullptr);
  CoreScDefn::BndVec binders1{"x"};
  BOOST_CHECK_EQUAL_COLLECTIONS(defns[1]->getBinders()->begin(), defns[1]->getBinders()->end(),
                                binders1.begin(), binders1.end());
  CoreScDefn::BndVec binders2{"y", "z"};
  BOOST_CHECK_EQUAL_COLLECTIONS(defns[2]->getBinders()->begin(), defns[2]->getBinders()->end(),
                                binders2.begin(), binders2.end());

  const auto & body1 = dynamic_cast<const CoreAp &>(defns[0]->getBody());
  const auto & fn1 = dynamic_cast<const CoreVar &>(body1.getFn());
  const auto & arg1 = dynamic_cast<const CoreNum &>(body1.getArg());
  BOOST_CHECK_EQUAL(fn1.getId(), "double");
  BOOST_CHECK_EQUAL(arg1.getNum(), 21);

  const auto & body2 = dynamic_cast<const CoreAp &>(defns[1]->getBody());
  const auto & fn2Outer = dynamic_cast<const CoreAp &>(body2.getFn());
  const auto & arg2Outer = dynamic_cast<const CoreVar &>(body2.getArg());
  const auto & fn2Inner = dynamic_cast<const CoreVar &>(fn2Outer.getFn());
  const auto & arg2Inner = dynamic_cast<const CoreVar &>(fn2Outer.getArg());
  BOOST_CHECK_EQUAL(fn2Inner.getId(), "plus");
  BOOST_CHECK_EQUAL(arg2Inner.getId(), "x");
  BOOST_CHECK_EQUAL(arg2Outer.getId(), "x");

  const auto & body3 = dynamic_cast<const CoreVar &>(defns[2]->getBody());
  BOOST_CHECK_EQUAL(body3.getId(), "z");
}

BOOST_AUTO_TEST_SUITE_END()
