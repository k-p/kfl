/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include <sstream>

#include "gtest/gtest.h"
#include "Expr.h"
#include "Pprint.h"

TEST(PprintTest, Var)
{
  typedef kfl::EVar<std::string> TestVar;

  const std::string cVarName = "name";
  const TestVar e(cVarName);

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(e);
  ASSERT_TRUE(oss.str() == cVarName);
}

TEST(PprintTest, Num)
{
  typedef kfl::ENum<std::string> TestNum;

  const int cNum = 1;
  const TestNum e(cNum);

  const std::string cNumStr = "1";
  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(e);
  ASSERT_TRUE(oss.str() == cNumStr);
}

TEST(PprintTest, Constr)
{
  typedef kfl::EConstr<std::string> TestConstr;

  const int cTag = 1;
  const int cArity = 2;
  const TestConstr e(cTag,cArity);

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(e);
  const std::string constrpprint = "Pack{1,2}";
  ASSERT_TRUE(oss.str() == constrpprint);
}

TEST(PprintTest, Ap)
{
  typedef kfl::EAp<std::string> TestAp;
  typedef kfl::ENum<std::string> TestNum;
  typedef kfl::EVar<std::string> TestVar;

  const TestAp e(new TestVar("fn"), new TestNum(0));

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(e);
  ASSERT_TRUE(oss.str() == "fn 0");
}

TEST(PprintTest, Let)
{
  typedef kfl::ELet<std::string> TestLet;
  typedef kfl::ENum<std::string> TestNum;
  typedef kfl::EVar<std::string> TestVar;

  TestLet::Defn d1("x", new TestNum(0));
  TestLet::Defn d2("y", new TestNum(1));
  TestLet::DefnVec * defns = new TestLet::DefnVec();
  defns->push_back(d1);
  defns->push_back(d2);
  const TestLet e(defns, new TestVar("x"));

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(e);
  const std::string letpprint =
    "let\n"
    "  x = 0;\n"
    "  y = 1\n"
    "in x";
  ASSERT_TRUE(oss.str() == letpprint);
}

TEST(PprintTest, testCase)
{
  typedef kfl::ECase<std::string> TestCase;
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
  const TestCase e(new TestVar("x"), alters);

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(e);
  const std::string casepprint =
    "case x of\n"
    "  <1> -> x;\n"
    "  <2> y z -> y";
  ASSERT_TRUE(oss.str() == casepprint);
}

TEST(PprintTest, Lam)
{
  typedef kfl::ELam<std::string> TestLam;
  typedef kfl::EVar<std::string> TestVar;

  TestLam::BndVec * binders = new TestLam::BndVec();
  binders->push_back("x");
  binders->push_back("y");
  const TestLam e(binders, new TestVar("x"));

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(e);
  const std::string lampprint = "\\ x y. x";
  ASSERT_TRUE(oss.str() == lampprint);
}

TEST(PprintTest, ScDefnWithoutArgs)
{
  typedef kfl::ScDefn<std::string> TestScDefn;
  typedef kfl::EVar<std::string> TestVar;

  TestScDefn defn("test", 0, new TestVar("x"));

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(defn);
  ASSERT_TRUE(oss.str() == "test = x");
}

TEST(PprintTest, ScDefnWithArgs)
{
  typedef kfl::ScDefn<std::string> TestScDefn;
  typedef kfl::EVar<std::string> TestVar;

  TestScDefn::BndVec * binders = new TestScDefn::BndVec();
  binders->push_back("x");
  binders->push_back("y");
  TestScDefn defn("test", binders, new TestVar("x"));

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(defn);
  ASSERT_TRUE(oss.str() == "test x y = x");
}

TEST(PprintTest, Program)
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

  std::ostringstream oss;
  kfl::Pprint<std::string> pp(oss);
  pp.visit(program);
  const std::string programpprint =
    "test1 = x;\n"
    "test2 = y\n";
  ASSERT_TRUE(oss.str() == programpprint);
}

TEST(PprintTest, AtomicPprintA)
{
  typedef kfl::EVar<std::string> TestVar;

  const std::string cVarName = "name";
  const TestVar e(cVarName);

  std::ostringstream oss;
  kfl::PprintA<std::string> pp(oss);
  pp.visit(e);
  ASSERT_TRUE(oss.str() == cVarName);
}

TEST(PprintTest, NonAtomicPprintA)
{
  typedef kfl::EAp<std::string> TestAp;
  typedef kfl::ENum<std::string> TestNum;
  typedef kfl::EVar<std::string> TestVar;

  const TestAp e(new TestVar("fn"), new TestNum(0));

  std::ostringstream oss;
  kfl::PprintA<std::string> pp(oss);
  pp.visit(e);
  ASSERT_TRUE(oss.str() == "(fn 0)");
}

TEST(PprintTest, StreamOutOp)
{
  typedef kfl::EVar<std::string> TestVar;

  const std::string cVarName = "name";
  const TestVar e(cVarName);

  std::ostringstream oss;
  oss << e;
  ASSERT_TRUE(oss.str() == cVarName);
}
