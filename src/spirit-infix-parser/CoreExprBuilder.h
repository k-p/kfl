/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __CORE_EXPR_BUILDER_H__
#define __CORE_EXPR_BUILDER_H__

#include "boost/tuple/tuple.hpp"
#include "boost/variant.hpp"

#include "CoreExpr.h"
#include "Parser.h"

namespace kfl {

  class CoreExprBuilder : public boost::static_visitor<CoreExpr *>
  {
  public:
    CoreExprBuilder(CoreExprFactory & f);
    CoreExpr * operator()(const Parser::AstVar & var) const;
    CoreExpr * operator()(const Parser::AstNum & num) const;
    CoreExpr * operator()(const Parser::AstConstr & constr) const;
    CoreExpr * operator()(const Parser::AstExprVec & aexprs) const;
    CoreExpr * operator()(const Parser::AstCase & case_) const;
    CoreExpr * operator()(const Parser::AstLambda & lambda) const;
    CoreExpr * operator()(const Parser::AstLet & let) const;
    CoreExpr * operator()(const Parser::AstLetRec & letrec) const;
  protected:
    CoreExprFactory::BndVec * buildBnds(const Parser::AstVarVec & vars) const;
    CoreExprFactory::DefnVec * buildDefns(const Parser::AstDefnVec & defns) const;
    CoreExpr * buildExpr(const Parser::AstExpr & expr) const;
    CoreNum * buildNum(kfl::Parser::AstNum num) const;
  private:
    CoreExprFactory & m_factory;
  };

  class CoreProgramBuilder
  {
  public:
    CoreProgramBuilder(CoreExprFactory & f);
    CoreProgram * operator()(const Parser::AstProgram & program) const;
  protected:
    CoreExprFactory::BndVec * buildBnds(const Parser::AstVarVec & vars) const;
    CoreExpr * buildExpr(const Parser::AstExpr & expr) const;
  private:
    CoreExprFactory & m_factory;
  };

} /* namespace */

#endif /* __CORE_EXPR_BUILDER_H__ */
