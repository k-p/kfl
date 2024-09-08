/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __CORE_EXPR_BUILDER_H__
#define __CORE_EXPR_BUILDER_H__

#include "CoreExpr.h"
#include "Parser.h"

#include <boost/tuple/tuple.hpp>
#include <boost/variant.hpp>

namespace kfl {

  class CoreExprBuilder : public boost::static_visitor<std::unique_ptr<CoreExpr>>
  {
  public:
    CoreExprBuilder(CoreFactory & f);
    std::unique_ptr<CoreExpr> operator()(const Parser::AstVar & var) const;
    std::unique_ptr<CoreExpr> operator()(const Parser::AstNum & num) const;
    std::unique_ptr<CoreExpr> operator()(const Parser::AstConstr & constr) const;
    std::unique_ptr<CoreExpr> operator()(const Parser::AstExprVec & aexprs) const;
    std::unique_ptr<CoreExpr> operator()(const Parser::AstCase & case_) const;
    std::unique_ptr<CoreExpr> operator()(const Parser::AstLambda & lambda) const;
    std::unique_ptr<CoreExpr> operator()(const Parser::AstLet & let) const;
    std::unique_ptr<CoreExpr> operator()(const Parser::AstLetRec & letrec) const;
  protected:
    std::unique_ptr<CoreFactory::BndVec> buildBnds(const Parser::AstVarVec & vars) const;
    std::unique_ptr<CoreFactory::DefnVec> buildDefns(const Parser::AstDefnVec & defns) const;
    std::unique_ptr<CoreExpr> buildExpr(const Parser::AstExpr & expr) const;
    std::unique_ptr<CoreNum> buildNum(kfl::Parser::AstNum num) const;
  private:
    CoreFactory & m_factory;
  };

  class CoreProgramBuilder
  {
  public:
    CoreProgramBuilder(CoreFactory & f);
    std::unique_ptr<CoreProgram> operator()(const Parser::AstProgram & program) const;
  protected:
    std::unique_ptr<CoreFactory::BndVec> buildBnds(const Parser::AstVarVec & vars) const;
    std::unique_ptr<CoreExpr> buildExpr(const Parser::AstExpr & expr) const;
  private:
    CoreFactory & m_factory;
  };

} /* namespace */

#endif /* __CORE_EXPR_BUILDER_H__ */
