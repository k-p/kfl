/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#include "CoreExpr.h"
#include "CoreExprBuilder.h"

namespace kfl {

  CoreExprFactory::BndVec * buildBnds(CoreExprFactory & factory, const Parser::AstVarVec & vars)
  {
    CoreExprFactory::BndVec * bnds = nullptr;
    if (!vars.empty()) {
      bnds = factory.createBnds();
      for(const auto & v : vars) {
        factory.addBnd(bnds, v);
      }
    }
    return bnds;
  }

  CoreExpr * buildExpr(CoreExprFactory & factory, const Parser::AstExpr & expr)
  {
    CoreExprBuilder exprBuilder(factory);
    return boost::apply_visitor(exprBuilder, expr);
  }

} /* end namespace */

kfl::CoreExprBuilder::CoreExprBuilder(kfl::CoreExprFactory & f)
  : m_factory(f)
{
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstVar & v) const
{
  return m_factory.createVar(v);
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstNum & n) const
{
  return m_factory.createNum(n);
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstConstr & c) const
{
  return m_factory.createConstr(c.tag, c.arity);
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstExprVec & exprs) const
{
  CoreExpr * arg = buildExpr(exprs.back());
  if (exprs.size() == 1) {
    return arg;
  }
  else {
    Parser::AstExpr ast(Parser::AstExprVec(exprs.begin(), --exprs.end()));
    CoreExpr * fn = buildExpr(ast);
    return m_factory.createAp(fn, arg);
  }
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstCase & case_) const
{
  CoreExprFactory::AlterVec * alters = m_factory.createAlters();
  for(const auto & a : case_.alts) {
    m_factory.addAlter(alters, buildNum(a.tag), buildBnds(a.vars), buildExpr(a.expr));
  }
  return m_factory.createCase(buildExpr(case_.expr), alters);
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstLambda & lambda) const
{
  return m_factory.createLambda(buildBnds(lambda.args), buildExpr(lambda.expr));
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstLet & let) const
{
  return m_factory.createLet(buildDefns(let.defns), buildExpr(let.expr));
}

kfl::CoreExpr * kfl::CoreExprBuilder::operator()(const kfl::Parser::AstLetRec & letrec) const
{
  return m_factory.createLetRec(buildDefns(letrec.defns), buildExpr(letrec.expr));
}

kfl::CoreExprFactory::BndVec * kfl::CoreExprBuilder::buildBnds(const kfl::Parser::AstVarVec & vars) const
{
  return kfl::buildBnds(m_factory, vars);
}

kfl::CoreExprFactory::DefnVec * kfl::CoreExprBuilder::buildDefns(const kfl::Parser::AstDefnVec & defns) const
{
  CoreExprFactory::DefnVec * coreDefns = m_factory.createDefns();
  for(const auto & d : defns) {
    m_factory.addDefn(coreDefns, d.var, buildExpr(d.expr));
  }
  return coreDefns;
}

kfl::CoreExpr * kfl::CoreExprBuilder::buildExpr(const Parser::AstExpr & expr) const
{
  return kfl::buildExpr(m_factory, expr);
}

kfl::CoreNum * kfl::CoreExprBuilder::buildNum(kfl::Parser::AstNum num) const
{
  return new CoreNum(num);
}

kfl::CoreProgramBuilder::CoreProgramBuilder(CoreExprFactory & f)
  : m_factory(f)
{
}

kfl::CoreProgram * kfl::CoreProgramBuilder::operator()(const Parser::AstProgram & program) const
{
  CoreExprFactory::ScDefnVec * scDefns = m_factory.createScDefns();
  for(const auto & d : program) {
    m_factory.addScDefn(scDefns, d.name, buildBnds(d.args), buildExpr(d.expr));
  }
  return m_factory.createProgram(scDefns);
}

kfl::CoreExprFactory::BndVec * kfl::CoreProgramBuilder::buildBnds(const kfl::Parser::AstVarVec & vars) const
{
  return kfl::buildBnds(m_factory, vars);
}

kfl::CoreExpr * kfl::CoreProgramBuilder::buildExpr(const Parser::AstExpr & expr) const
{
  return kfl::buildExpr(m_factory, expr);
}
