/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include <sstream>

#include "boost/scoped_ptr.hpp"

#include "gtest/gtest.h"
#include "ExprFactory.h"

TEST(ExprFactoryTest, Var)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::EVar<std::string> TestVar;

  const std::string cVarName = "name";
  TestExprFactory f;
  boost::scoped_ptr<TestExpr> e(f.createVar(cVarName));

  TestVar * v;
  ASSERT_TRUE(v = dynamic_cast<TestVar *>(e.get()));
  ASSERT_TRUE(v->isAtomic());
  ASSERT_TRUE(v->getId() == cVarName);
}

TEST(ExprFactoryTest, Num)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::ENum<std::string> TestNum;

  const int cNum = 1;
  TestExprFactory f;
  boost::scoped_ptr<TestExpr> e(f.createNum(cNum));

  TestNum * n;
  ASSERT_TRUE(n = dynamic_cast<TestNum *>(e.get()));
  ASSERT_TRUE(n->isAtomic());
  ASSERT_TRUE(n->getNum() == cNum);
}

TEST(ExprFactoryTest, Constr)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::EConstr<std::string> TestConstr;

  const int cTag = 1;
  const int cArity = 2;
  TestExprFactory f;
  boost::scoped_ptr<TestExpr> e(f.createConstr(cTag,cArity));

  TestConstr * c;
  ASSERT_TRUE(c = dynamic_cast<TestConstr *>(e.get()));
  ASSERT_TRUE(!c->isAtomic());
  ASSERT_TRUE(c->getTag() == cTag);
  ASSERT_TRUE(c->getArity() == cArity);
}

TEST(ExprFactoryTest, Ap)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::EAp<std::string> TestAp;

  TestExprFactory f;
  TestExpr * fn = f.createVar("fn");
  TestExpr * arg = f.createNum(0);
  boost::scoped_ptr<TestExpr> e(f.createAp(fn, arg));

  TestAp * a;
  ASSERT_TRUE(a = dynamic_cast<TestAp *>(e.get()));
  ASSERT_TRUE(!a->isAtomic());
  ASSERT_TRUE(a->getFn() == *fn);
  ASSERT_TRUE(a->getArg() == *arg);
}

TEST(ExprFactoryTest, Let)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::ELet<std::string> TestLet;

  TestExprFactory f;
  TestExprFactory::DefnVec * defns = f.createDefns();
  f.addDefn(defns, "x", f.createNum(0));
  f.addDefn(defns, "y", f.createNum(1));
  TestExpr * body = f.createVar("x");
  boost::scoped_ptr<TestExpr> e(f.createLet(defns, body));

  TestLet * l;
  ASSERT_TRUE(l = dynamic_cast<TestLet *>(e.get()));
  ASSERT_TRUE(!l->isAtomic());
  ASSERT_TRUE(!l->isRec());
  ASSERT_TRUE(&l->getDefns() == defns);
  ASSERT_TRUE(&l->getBody() == body);
}

TEST(ExprFactoryTest, LetRec)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::ELet<std::string> TestLet;

  TestExprFactory f;
  TestExprFactory::DefnVec * defns = f.createDefns();
  f.addDefn(defns, "x", f.createVar("y"));
  f.addDefn(defns, "y", f.createVar("x"));
  TestExpr * body = f.createVar("x");
  boost::scoped_ptr<TestExpr> e(f.createLetRec(defns, body));

  TestLet * l;
  ASSERT_TRUE(l = dynamic_cast<TestLet *>(e.get()));
  ASSERT_TRUE(!l->isAtomic());
  ASSERT_TRUE(l->isRec());
  ASSERT_TRUE(&l->getDefns() == defns);
  ASSERT_TRUE(&l->getBody() == body);
}

TEST(ExprFactoryTest, Case)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::ENum<std::string> TestNum;
  typedef kfl::ECase<std::string> TestCase;

  TestExprFactory f;
  TestExprFactory::AlterVec * alters = f.createAlters();
  f.addAlter(alters, new TestNum(1), 0, f.createVar("x"));
  TestExprFactory::BndVec * bnds = f.createBnds();
  f.addBnd(bnds, "y");
  f.addBnd(bnds, "z");
  f.addAlter(alters, new TestNum(2), bnds, f.createVar("y"));
  TestExpr * expr = f.createVar("x");
  boost::scoped_ptr<TestExpr> e(f.createCase(expr, alters));

  TestCase * c;
  ASSERT_TRUE(c = dynamic_cast<TestCase *>(e.get()));
  ASSERT_TRUE(!c->isAtomic());
  ASSERT_TRUE(&c->getExpr() == expr);
  ASSERT_TRUE(&c->getAlters() == alters);
}

TEST(ExprFactoryTest, Lambda)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::ELam<std::string> TestLam;

  TestExprFactory f;
  TestExprFactory::BndVec * bnds = f.createBnds();
  f.addBnd(bnds, "x");
  f.addBnd(bnds, "y");
  TestExpr * body = f.createVar("x");
  boost::scoped_ptr<TestExpr> e(f.createLambda(bnds, body));

  TestLam * l;
  ASSERT_TRUE(l = dynamic_cast<TestLam *>(e.get()));
  ASSERT_TRUE(!l->isAtomic());
  ASSERT_TRUE(&l->getBinders() == bnds);
  ASSERT_TRUE(&l->getBody() == body);
}

TEST(ExprFactoryTest, ScDefnWithoutArgs)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::ScDefn<std::string> TestScDefn;

  TestExprFactory f;
  const std::string name("test");
  TestExpr * body = f.createVar("x");
  boost::scoped_ptr<TestScDefn> defn(f.createScDefn(name, 0, body));

  ASSERT_TRUE(defn->getName() == name);
  ASSERT_TRUE(defn->getBinders() == 0);
  ASSERT_TRUE(&defn->getBody() == body);
}

TEST(ExprFactoryTest, ScDefnWithArgs)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::ScDefn<std::string> TestScDefn;

  TestExprFactory f;
  const std::string name("test");
  TestExprFactory::BndVec * bnds = f.createBnds();
  f.addBnd(bnds, "x");
  f.addBnd(bnds, "y");
  TestExpr * body = f.createVar("x");
  boost::scoped_ptr<TestScDefn> defn(f.createScDefn(name, bnds, body));

  ASSERT_TRUE(defn->getName() == name);
  ASSERT_TRUE(defn->getBinders() == bnds);
  ASSERT_TRUE(&defn->getBody() == body);
}

TEST(ExprFactoryTest, Program)
{
  typedef kfl::ExprFactory<std::string> TestExprFactory;
  typedef kfl::Program<std::string> TestProgram;
  typedef kfl::ScDefn<std::string> TestScDefn;
  typedef kfl::EVar<std::string> TestVar;

  TestExprFactory f;
  TestExprFactory::ScDefnVec * defns = f.createScDefns();
  f.addScDefn(defns, "test1", 0, new TestVar("x"));
  f.addScDefn(defns, "test2", 0, new TestVar("y"));
  boost::scoped_ptr<TestProgram> program(f.createProgram(defns));

  ASSERT_TRUE(&program->getDefns() == defns);
}
