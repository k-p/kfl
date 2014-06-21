/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include <sstream>
#include <string>

#include "boost/scoped_ptr.hpp"

#include "gtest/gtest.h"
#include "CoreExprBuilder.h"
#include "Parser.h"

namespace {

  using std::ostringstream;
  using std::string;
  using namespace kfl;

  TEST(CoreExprBuilderTest, Var)
  {
    using namespace kfl::Parser;
    const string cVarName = "name";
    const AstExpr ast = AstVar(cVarName);

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    const CoreVar * var = dynamic_cast<const CoreVar *>(e.get());
    ASSERT_TRUE(var->getId() == cVarName);
  }

  TEST(CoreExprBuilderTest, Num)
  {
    using namespace kfl::Parser;
    const int cNum = 1;
    const AstExpr ast = AstNum(cNum);

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    const CoreNum * num = dynamic_cast<const CoreNum *>(e.get());
    ASSERT_TRUE(num->getNum() == cNum);
  }

  TEST(CoreExprBuilderTest, Constr)
  {
    using namespace kfl::Parser;
    const int cTag = 1;
    const int cArity = 2;
    const AstExpr ast = AstConstr{cTag, cArity};

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    const CoreConstr * constr = dynamic_cast<const CoreConstr *>(e.get());
    ASSERT_TRUE(constr->getTag() == cTag);
    ASSERT_TRUE(constr->getArity() == cArity);
  }

  TEST(CoreExprBuilderTest, Ap)
  {
    using namespace kfl::Parser;
    // "fn 1 2" == "((fn 1) 2)"
    const string cFnName = "fn";
    const int cArg1 = 1;
    const int cArg2 = 2;
    const AstExpr ast = AstExprVec{ AstVar(cFnName), AstNum(cArg1), AstNum(cArg2) };

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    const CoreAp * apOuter = dynamic_cast<const CoreAp *>(e.get());
    const CoreAp & apInner = dynamic_cast<const CoreAp &>(apOuter->getFn());
    const CoreVar & fn = dynamic_cast<const CoreVar &>(apInner.getFn());
    ASSERT_TRUE(fn.getId() == cFnName);
    const CoreNum & arg1 = dynamic_cast<const CoreNum &>(apInner.getArg());
    ASSERT_TRUE(arg1.getNum() == cArg1);
    const CoreNum & arg2 = dynamic_cast<const CoreNum &>(apOuter->getArg());
    ASSERT_TRUE(arg2.getNum() == cArg2);
  }

  TEST(CoreExprBuilderTest, Let)
  {
    using namespace kfl::Parser;
    const AstExpr ast = AstLet{
      AstDefnVec{ AstDefn{ AstVar("x"), AstNum(0) },
		  AstDefn{ AstVar("y"), AstNum(1) } },
      AstVar("x")
    };

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    const CoreLet * let = dynamic_cast<const CoreLet *>(e.get());
    ASSERT_TRUE(!let->isRec());

    const CoreLet::DefnVec & defns = let->getDefns();
    ASSERT_TRUE(defns.size() == 2);
    ASSERT_TRUE(defns[0].first == "x");
    const CoreNum * num0 = dynamic_cast<const CoreNum *>(defns[0].second);
    ASSERT_TRUE(num0->getNum() == 0);
    ASSERT_TRUE(defns[1].first == "y");
    const CoreNum * num1 = dynamic_cast<const CoreNum *>(defns[1].second);
    ASSERT_TRUE(num1->getNum() == 1);

    const CoreVar & var = dynamic_cast<const CoreVar &>(let->getBody());
    ASSERT_TRUE(var.getId() == "x");
  }

  TEST(CoreExprBuilderTest, LetRec)
  {
    using namespace kfl::Parser;
    const AstExpr ast = AstLetRec{
      AstDefnVec{ AstDefn{ AstVar("x"), AstNum(0) },
		  AstDefn{ AstVar("y"), AstNum(1) } },
      AstVar("x")
    };

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    const CoreLet * let = dynamic_cast<const CoreLet *>(e.get());
    ASSERT_TRUE(let->isRec());

    const CoreLet::DefnVec & defns = let->getDefns();
    ASSERT_TRUE(defns.size() == 2);
    ASSERT_TRUE(defns[0].first == "x");
    const CoreNum * num0 = dynamic_cast<const CoreNum *>(defns[0].second);
    ASSERT_TRUE(num0->getNum() == 0);
    ASSERT_TRUE(defns[1].first == "y");
    const CoreNum * num1 = dynamic_cast<const CoreNum *>(defns[1].second);
    ASSERT_TRUE(num1->getNum() == 1);

    const CoreVar & var = dynamic_cast<const CoreVar &>(let->getBody());
    ASSERT_TRUE(var.getId() == "x");
  }

  TEST(CoreExprBuilderTest, Case)
  {
    using namespace kfl::Parser;
    AstExpr ast = AstCase{
      AstVar("x"),
      AstAltVec{
	AstAlt{ AstNum(1), AstVarVec{ }, AstVar("x") },
	AstAlt{ AstNum(2), AstVarVec{ AstVar("y"), AstVar("z") }, AstVar("y") } }
    };

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    auto c = dynamic_cast<const CoreCase *>(e.get());

    const auto & var = dynamic_cast<const CoreVar &>(c->getExpr());
    ASSERT_TRUE(var.getId() == "x");

    const auto & alters = c->getAlters();
    ASSERT_TRUE(alters.size() == 2);
    ASSERT_TRUE(alters[0].m_num->getNum() == 1);
    ASSERT_TRUE(alters[1].m_num->getNum() == 2);

    ASSERT_TRUE(alters[0].m_binders == nullptr);
    const CoreCase::BndVec binders{"y", "z"};
    ASSERT_TRUE(*alters[1].m_binders == binders);

    const auto & expr0 = dynamic_cast<const CoreVar *>(alters[0].m_expr);
    ASSERT_TRUE(expr0->getId() == "x");
    const auto & expr1 = dynamic_cast<const CoreVar *>(alters[1].m_expr);
    ASSERT_TRUE(expr1->getId() == "y");
  }

  TEST(CoreExprBuilderTest, Lambda)
  {
    using namespace kfl::Parser;
    const AstExpr ast = AstLambda{
      AstVarVec{ AstVar("x"), AstVar("y"), AstVar("z") },
      AstVar("x")
    };

    CoreExprFactory factory;
    CoreExprBuilder builder(factory);
    boost::scoped_ptr<CoreExpr> e(boost::apply_visitor(builder, ast));

    auto l = dynamic_cast<CoreLam *>(e.get());
    const CoreLam::BndVec binders{"x", "y", "z"};
    ASSERT_TRUE(l->getBinders() == binders);
    const auto & var = dynamic_cast<const CoreVar &>(l->getBody());
    ASSERT_TRUE(var.getId() == "x");
  }

  TEST(CoreExprBuilderTest, Program)
  {
    using namespace kfl::Parser;
    const AstProgram ast = AstProgram{
      AstScDefn{ AstVar("main"), AstVarVec{}, AstExprVec{ AstVar("double"), AstNum(21) } },
      AstScDefn{ AstVar("double"), AstVarVec{ AstVar("x") }, AstExprVec{ AstVar("plus"), AstVar("x"), AstVar("x") } },
      AstScDefn{ AstVar("K1"), AstVarVec{ AstVar("y"), AstVar("z") }, AstExprVec{ AstVar("z") } }
    };

    CoreExprFactory factory;
    CoreProgramBuilder builder(factory);
    boost::scoped_ptr<CoreProgram> p(builder(ast));

    const auto & defns = p->getDefns();
    ASSERT_TRUE(defns.size() == 3);

    ASSERT_TRUE(defns[0]->getName() == "main");
    ASSERT_TRUE(defns[1]->getName() == "double");
    ASSERT_TRUE(defns[2]->getName() == "K1");

    ASSERT_TRUE(defns[0]->getBinders() == nullptr);
    CoreScDefn::BndVec binders1{"x"};
    ASSERT_TRUE(*defns[1]->getBinders() == binders1);
    CoreScDefn::BndVec binders2{"y", "z"};
    ASSERT_TRUE(*defns[2]->getBinders() == binders2);

    const auto & body1 = dynamic_cast<const CoreAp &>(defns[0]->getBody());
    const auto & fn1 = dynamic_cast<const CoreVar &>(body1.getFn());
    const auto & arg1 = dynamic_cast<const CoreNum &>(body1.getArg());
    ASSERT_TRUE(fn1.getId() == "double");
    ASSERT_TRUE(arg1.getNum() == 21);

    const auto & body2 = dynamic_cast<const CoreAp &>(defns[1]->getBody());
    const auto & fn2Outer = dynamic_cast<const CoreAp &>(body2.getFn());
    const auto & arg2Outer = dynamic_cast<const CoreVar &>(body2.getArg());
    const auto & fn2Inner = dynamic_cast<const CoreVar &>(fn2Outer.getFn());
    const auto & arg2Inner = dynamic_cast<const CoreVar &>(fn2Outer.getArg());
    ASSERT_TRUE(fn2Inner.getId() == "plus");
    ASSERT_TRUE(arg2Inner.getId() == "x");
    ASSERT_TRUE(arg2Outer.getId() == "x");

    const auto & body3 = dynamic_cast<const CoreVar &>(defns[2]->getBody());
    ASSERT_TRUE(body3.getId() == "z");
  }

} /* namespace */
