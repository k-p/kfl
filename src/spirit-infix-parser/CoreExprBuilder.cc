/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include "CoreExpr.h"
#include "CoreExprBuilder.h"

namespace {

  std::unique_ptr<kfl::CoreFactory::BndVec>
  buildBnds(kfl::CoreFactory & factory, const kfl::Parser::AstVarVec & vars)
  {
    if (!vars.empty()) {
      auto bnds = factory.createBnds();
      for(const auto & v : vars) {
        factory.addBnd(*bnds, v);
      }
      return bnds;
    }
    else {
      return nullptr;
    }
  }

  std::unique_ptr<kfl::CoreExpr>
  buildExpr(kfl::CoreFactory & factory, const kfl::Parser::AstExpr & expr)
  {
    kfl::CoreExprBuilder exprBuilder(factory);
    return boost::apply_visitor(exprBuilder, expr);
  }

} /* end namespace */

kfl::CoreExprBuilder::CoreExprBuilder(kfl::CoreFactory & f)
  : m_factory(f)
{
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstVar & v) const
{
  return m_factory.createVar(v);
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstNum & n) const
{
  return m_factory.createNum(n);
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstConstr & c) const
{
  return m_factory.createConstr(c.tag, c.arity);
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstExprVec & exprs) const
{
  auto arg = buildExpr(exprs.back());
  if (exprs.size() == 1) {
    return arg;
  }
  else {
    Parser::AstExpr ast(Parser::AstExprVec(exprs.begin(), --exprs.end()));
    auto fn = buildExpr(ast);
    auto ap = m_factory.createAp(std::move(fn), std::move(arg));
    return ap;
  }
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstCase & case_) const
{
  auto alters = m_factory.createAlters();
  for(const auto & a : case_.alts) {
    m_factory.addAlter(*alters, buildNum(a.tag), buildBnds(a.vars), buildExpr(a.expr));
  }
  return m_factory.createCase(buildExpr(case_.expr), std::move(alters));
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstLambda & lambda) const
{
  return m_factory.createLambda(buildBnds(lambda.args), buildExpr(lambda.expr));
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstLet & let) const
{
  return m_factory.createLet(buildDefns(let.defns), buildExpr(let.expr));
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::operator()(const kfl::Parser::AstLetRec & letrec) const
{
  return m_factory.createLetRec(buildDefns(letrec.defns), buildExpr(letrec.expr));
}

std::unique_ptr<kfl::CoreFactory::BndVec>
kfl::CoreExprBuilder::buildBnds(const kfl::Parser::AstVarVec & vars) const
{
  return ::buildBnds(m_factory, vars);
}

std::unique_ptr<kfl::CoreFactory::DefnVec>
kfl::CoreExprBuilder::buildDefns(const kfl::Parser::AstDefnVec & defns) const
{
  auto coreDefns = m_factory.createDefns();
  for(const auto & d : defns) {
    m_factory.addDefn(*coreDefns, d.var, buildExpr(d.expr));
  }
  return coreDefns;
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreExprBuilder::buildExpr(const Parser::AstExpr & expr) const
{
  return ::buildExpr(m_factory, expr);
}

std::unique_ptr<kfl::CoreNum>
kfl::CoreExprBuilder::buildNum(kfl::Parser::AstNum num) const
{
  return std::make_unique<CoreNum>(num);
}

kfl::CoreProgramBuilder::CoreProgramBuilder(CoreFactory & f)
  : m_factory(f)
{
}

std::unique_ptr<kfl::CoreProgram>
kfl::CoreProgramBuilder::operator()(const Parser::AstProgram & program) const
{
  auto scDefns = m_factory.createScDefns();
  for(const auto & d : program) {
    m_factory.addScDefn(*scDefns, d.name, buildBnds(d.args), buildExpr(d.expr));
  }
  return m_factory.createProgram(std::move(scDefns));
}

std::unique_ptr<kfl::CoreFactory::BndVec>
kfl::CoreProgramBuilder::buildBnds(const kfl::Parser::AstVarVec & vars) const
{
  return ::buildBnds(m_factory, vars);
}

std::unique_ptr<kfl::CoreExpr>
kfl::CoreProgramBuilder::buildExpr(const Parser::AstExpr & expr) const
{
  return ::buildExpr(m_factory, expr);
}
