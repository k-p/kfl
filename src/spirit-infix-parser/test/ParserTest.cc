/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "Parser.h"

namespace {

  using boost::get;
  using std::ostringstream;
  using std::string;
  using namespace kfl::Parser;

  TEST(ParserTest, Var)
  {
    const string s = " \t\n name_12three   \t\t\t\n\n\n   ";
    const AstExpr expected = AstExprVec{ AstVar("name_12three") };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Num)
  {
    const string s = "42";
    const AstExpr expected = AstExprVec{ AstNum(42) };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Constr)
  {
    const string s = "Pack{13,42}";
    const AstExpr expected = AstExprVec{ AstConstr{AstNum(13), AstNum(42)} };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Ap)
  {
    const string s = "f x y";
    const AstExpr expected = AstExprVec{ AstVar("f"), AstVar("x"), AstVar("y") };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Let)
  {
    const string s = "let x=0; y=1 in x";
    const AstExpr expected = AstLet{
      AstDefnVec{ AstDefn{ AstVar("x"), AstExprVec{ AstNum(0) } },
		  AstDefn{ AstVar("y"), AstExprVec{ AstNum(1) } } },
      AstExprVec{ AstVar("x") }
    };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, LetRec)
  {
    const string s = "letrec x=0; y=1 in x";
    const AstExpr expected = AstLetRec{
      AstDefnVec{ AstDefn{ AstVar("x"), AstExprVec{ AstNum(0) } },
		  AstDefn{ AstVar("y"), AstExprVec{ AstNum(1) } } },
      AstExprVec{ AstVar("x") }
    };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Case)
  {
    const string s =
      "case x of "
      "<0> -> 0;"
      "<1> a -> a;"
      "<2> a b -> b";
    const AstExpr expected = AstCase{
      AstExprVec{ AstVar("x") },
      AstAltVec{ AstAlt{ AstNum(0), AstVarVec{ }, AstExprVec{ AstNum(0) } },
		 AstAlt{ AstNum(1), AstVarVec{ AstVar("a") }, AstExprVec{ AstVar("a") } },
		 AstAlt{ AstNum(2), AstVarVec{ AstVar("a"), AstVar("b") }, AstExprVec{ AstVar("b") } } }
    };
    

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Lambda)
  {
    const string s = "\\x y z.x";
    const AstExpr expected = AstLambda{
      AstVarVec{ AstVar("x"), AstVar("y"), AstVar("z") },
      AstExprVec{ AstVar("x") }
    };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Pexpr)
  {
    const string s = "\\x y z.z (y x)";
    const AstExpr expected = AstLambda{
      AstVarVec{ AstVar("x"), AstVar("y"), AstVar("z") },
      AstExprVec{ AstVar("z"), AstExprVec{ AstVar("y"), AstVar("x") } }
    };

    AstExpr ast;
    ASSERT_TRUE(parseExpr(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

  TEST(ParserTest, Program)
  {
    const string s = "main = double 21; double x = plus x x; K1 y z = z";
    const AstProgram expected = AstProgram{
      AstScDefn{ AstVar("main"), AstVarVec{}, AstExprVec{ AstVar("double"), AstNum(21) } },
      AstScDefn{ AstVar("double"), AstVarVec{ AstVar("x") }, AstExprVec{ AstVar("plus"), AstVar("x"), AstVar("x") } },
      AstScDefn{ AstVar("K1"), AstVarVec{ AstVar("y"), AstVar("z") }, AstExprVec{ AstVar("z") } }
    };

    AstProgram ast;
    ASSERT_TRUE(parseProgram(s.begin(), s.end(), ast));
    ASSERT_TRUE(ast == expected);
  }

} /* namespace */
