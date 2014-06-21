/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include <sstream>

#include "gtest/gtest.h"
#include "Expr.h"

TEST(ExprTest, Var)
{
  typedef kfl::EVar<std::string> TestVar;

  const std::string cVarName = "name";
  const TestVar e(cVarName);

  ASSERT_TRUE(e.isAtomic());
  ASSERT_TRUE(e.getId() == cVarName);
}

TEST(ExprTest, Num)
{
  typedef kfl::ENum<std::string> TestNum;

  const int cNum = 1;
  const TestNum e(cNum);

  ASSERT_TRUE(e.isAtomic());
  ASSERT_TRUE(e.getNum() == cNum);
}

TEST(ExprTest, Constr)
{
  typedef kfl::EConstr<std::string> TestConstr;

  const int cTag = 1;
  const int cArity = 2;
  const TestConstr e(cTag,cArity);

  ASSERT_TRUE(!e.isAtomic());
  ASSERT_TRUE(e.getTag() == cTag);
  ASSERT_TRUE(e.getArity() == cArity);
}

TEST(ExprTest, Ap)
{
  typedef kfl::EAp<std::string> TestAp;
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ENum<std::string> TestNum;
  typedef kfl::EVar<std::string> TestVar;

  TestExpr * f = new TestVar("fn");
  TestExpr * arg = new TestNum(0);
  const TestAp e(f, arg);

  ASSERT_TRUE(!e.isAtomic());
  ASSERT_TRUE(e.getFn() == *f);
  ASSERT_TRUE(e.getArg() == *arg);
}

TEST(ExprTest, Let)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ELet<std::string> TestLet;
  typedef kfl::ENum<std::string> TestNum;
  typedef kfl::EVar<std::string> TestVar;

  TestLet::Defn d1("x", new TestNum(0));
  TestLet::Defn d2("y", new TestNum(1));
  TestLet::DefnVec * defns = new TestLet::DefnVec();
  defns->push_back(d1);
  defns->push_back(d2);
  TestExpr * body = new TestVar("x");
  const TestLet e(defns, body);

  ASSERT_TRUE(!e.isAtomic());
  ASSERT_TRUE(&e.getDefns() == defns);
  ASSERT_TRUE(&e.getBody() == body);
}

TEST(ExprTest, Case)
{
  typedef kfl::ECase<std::string> TestCase;
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ENum<std::string> TestNum;
  typedef kfl::EVar<std::string> TestVar;

  TestCase::BndVec * binders1 = 0;
  TestCase::Alter a1  = { new TestNum(1), binders1, new TestVar("x") };
  TestCase::BndVec * binders2 = new TestCase::BndVec();
  binders2->push_back("y");
  binders2->push_back("z");
  TestCase::Alter a2  = { new TestNum(2), binders2, new TestVar("y") };
  TestCase::AlterVec * alters = new TestCase::AlterVec();
  alters->push_back(a1);
  alters->push_back(a2);
  TestExpr * expr = new TestVar("x");
  const TestCase e(expr, alters);

  ASSERT_TRUE(!e.isAtomic());
  ASSERT_TRUE(&e.getExpr() == expr);
  ASSERT_TRUE(&e.getAlters() == alters);
}

TEST(ExprTest, Lam)
{
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::ELam<std::string> TestLam;
  typedef kfl::EVar<std::string> TestVar;

  TestLam::BndVec * binders = new TestLam::BndVec();
  binders->push_back("x");
  binders->push_back("y");
  TestExpr * body = new TestVar("x");
  const TestLam e(binders, body);

  ASSERT_TRUE(!e.isAtomic());
  ASSERT_TRUE(&e.getBinders() == binders);
  ASSERT_TRUE(&e.getBody() == body);
}

TEST(ExprTest, ScDefnWithoutArgs)
{
  typedef kfl::ScDefn<std::string> TestScDefn;
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::EVar<std::string> TestVar;

  const std::string name("test");
  TestExpr * body = new TestVar("x");
  TestScDefn defn(name, 0, body);
  ASSERT_TRUE(defn.getName() == name);
  ASSERT_TRUE(defn.getBinders() == 0);
  ASSERT_TRUE(&defn.getBody() == body);
}

TEST(ExprTest, ScDefnWithArgs)
{
  typedef kfl::ScDefn<std::string> TestScDefn;
  typedef kfl::Expr<std::string> TestExpr;
  typedef kfl::EVar<std::string> TestVar;

  const std::string name("test");
  TestScDefn::BndVec * binders = new TestScDefn::BndVec();
  binders->push_back("x");
  binders->push_back("y");
  TestExpr * body = new TestVar("x");
  TestScDefn defn(name, binders, body);

  ASSERT_TRUE(defn.getName() == name);
  ASSERT_TRUE(defn.getBinders() == binders);
  ASSERT_TRUE(&defn.getBody() == body);
}

TEST(ExprTest, Program)
{
  typedef kfl::Program<std::string> TestProgram;
  typedef kfl::ScDefn<std::string> TestScDefn;
  typedef kfl::EVar<std::string> TestVar;

  TestScDefn * defn1 = new TestScDefn("test1", 0, new TestVar("x"));
  TestScDefn * defn2 = new TestScDefn("test2", 0, new TestVar("y"));
  TestProgram::ScDefnVec * defns = new TestProgram::ScDefnVec();
  defns->push_back(defn1);
  defns->push_back(defn2);

  TestProgram program(defns);

  ASSERT_TRUE(&program.getDefns() == defns);
}
