/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>

#include "boost/noncopyable.hpp"
#include "boost/variant.hpp"

namespace kfl {

  namespace Parser {

    struct AstAlt;
    struct AstCase;
    struct AstConstr;
    struct AstDefn;
    struct AstLambda;
    struct AstLet;
    struct AstLetRec;
    struct AstScDefn;

    typedef unsigned long AstNum;
    typedef std::string AstVar;

    typedef std::vector<AstAlt> AstAltVec;
    typedef std::vector<AstDefn> AstDefnVec;
    typedef std::vector<AstVar> AstVarVec;

    struct AstConstr {
      AstNum tag;
      AstNum arity;
      bool operator==(const kfl::Parser::AstConstr & rhs) const {
	return (tag == rhs.tag && arity == rhs.arity);
      }
    };

    typedef boost::variant<AstVar, AstNum, AstConstr> AstAExpr;

    typedef boost::make_recursive_variant<
      AstVar,
      AstNum,
      AstConstr,
      boost::recursive_wrapper<AstCase>,
      boost::recursive_wrapper<AstLambda>,
      boost::recursive_wrapper<AstLet>,
      boost::recursive_wrapper<AstLetRec>,
      std::vector< boost::recursive_variant_ >
      >::type AstExpr;

    typedef std::vector<AstExpr> AstExprVec;

    struct AstAlt {
      AstNum tag;
      AstVarVec vars;
      AstExpr expr;
      bool operator==(const AstAlt & rhs) const {
	return (tag == rhs.tag && vars == rhs.vars && expr == rhs.expr);
      }
    };

    struct AstCase {
      AstExpr expr;
      AstAltVec alts;
      bool operator==(const AstCase & rhs) const {
	return (expr == rhs.expr && alts == rhs.alts);
      }
    };

    struct AstDefn {
      AstVar var;
      AstExpr expr;
      bool operator==(const AstDefn & rhs) const {
	return (var == rhs.var && expr == rhs.expr);
      }
    };

    struct AstLambda {
      AstVarVec args;
      AstExpr expr;
      bool operator==(const AstLambda & rhs) const {
	return (args == rhs.args && expr == rhs.expr);
      }
    };

    struct AstLet {
      AstDefnVec defns;
      AstExpr expr;
      bool operator==(const AstLet & rhs) const {
	return (defns == rhs.defns && expr == rhs.expr);
      }
    };

    struct AstLetRec {
      AstDefnVec defns;
      AstExpr expr;
      bool operator==(const AstLetRec & rhs) const {
	return (defns == rhs.defns && expr == rhs.expr);
      }
    };

    struct AstScDefn {
      AstVar name;
      AstVarVec args;
      AstExpr expr;
      bool operator==(const AstScDefn & rhs) const {
	return (name == rhs.name && args == rhs.args && expr == rhs.expr);
      }
    };

    typedef std::vector<AstScDefn> AstScDefnVec;
    typedef AstScDefnVec AstProgram;

    template<class Iterator>
      static bool parseExpr(Iterator begin, Iterator end, AstExpr & ast);

    template<class Iterator>
      static bool parseProgram(Iterator begin, Iterator end, AstProgram & ast);

  } /* end namespace Parser */

} /* end namespace kfl */

#include "Parser.icc"

#endif /* __PARSER_H__ */
