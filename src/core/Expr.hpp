/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Expr.h"
#include "Visitor.h"

#include <cassert>
#include <functional>

template<typename Id>
kfl::Expr<Id>::~Expr()
{
}

template<typename Id>
bool kfl::Expr<Id>::isAtomic() const
{
  return false;
}

template<typename Id>
bool kfl::AtomicExpr<Id>::isAtomic() const
{
  return true;
}

template<typename Id>
kfl::EVar<Id>::EVar(const Id & name)
  : super(), m_name(name)
{
  assert(!m_name.empty());
}

template<typename Id>
void kfl::EVar<Id>::visit(Visitor<Id, void> & v) const
{
  return v.visitVar(*this);
}

template<typename Id>
const Id & kfl::EVar<Id>::getId() const
{
  return m_name;
}

template<typename Id>
kfl::ENum<Id>::ENum(int n)
  : super(), m_num(n)
{
}

template<typename Id>
void kfl::ENum<Id>::visit(Visitor<Id, void> & v) const
{
  return v.visitNum(*this);
}

template<typename Id>
int kfl::ENum<Id>::getNum() const
{
  return m_num;
}

template<typename Id>
kfl::EConstr<Id>::EConstr(int tag, int arity)
  : super(), m_tag(tag), m_arity(arity)
{
}

template<typename Id>
void kfl::EConstr<Id>::visit(Visitor<Id, void> & v) const
{
  return v.visitConstr(*this);
}

template<typename Id>
int kfl::EConstr<Id>::getTag() const
{
  return m_tag;
}

template<typename Id>
int kfl::EConstr<Id>::getArity() const
{
  return m_arity;
}

template<typename Id>
kfl::EAp<Id>::EAp(std::shared_ptr<Expr<Id>> fn, std::shared_ptr<Expr<Id>> arg)
  : super(), m_fn(fn), m_arg(arg)
{
  assert(m_fn);
  assert(m_arg);
}

template<typename Id>
void kfl::EAp<Id>::visit(Visitor<Id, void> & v) const
{
  return v.visitAp(*this);
}

template<typename Id>
const kfl::Expr<Id> & kfl::EAp<Id>::getFn() const
{
  assert(m_fn);
  return *m_fn;
}

template<typename Id>
const kfl::Expr<Id> & kfl::EAp<Id>::getArg() const
{
  assert(m_arg);
  return *m_arg;
}

template<typename Id>
kfl::ELet<Id>::ELet(std::shared_ptr<DefnVec> defns, std::shared_ptr<Expr<Id>> body, bool isRec)
  : super(), m_defns(defns), m_body(body), m_isRec(isRec)
{
  assert(m_defns);
  assert(m_defns->size() >= 1);
  assert(m_body);
}

template<typename Id>
void kfl::ELet<Id>::visit(Visitor<Id, void> & v) const
{
  return v.visitLet(*this);
}

template<typename Id>
bool kfl::ELet<Id>::isRec() const
{
  return m_isRec;
}

template<typename Id>
const typename kfl::ELet<Id>::DefnVec & kfl::ELet<Id>::getDefns() const
{
  assert(m_defns);
  return *m_defns;
}

template<typename Id>
const kfl::Expr<Id> & kfl::ELet<Id>::getBody() const
{
  assert(m_body);
  return *m_body;
}

template<typename Id>
kfl::ECase<Id>::ECase(std::shared_ptr<Expr<Id>> expr, std::shared_ptr<AlterVec> alters)
  : super(), m_expr(expr), m_alters(alters)
{
  assert(m_expr);
  assert(m_alters);
  assert(m_alters->size() >= 1);
}

template<typename Id>
void kfl::ECase<Id>::visit(Visitor<Id, void> & v) const
{
  return v.visitCase(*this);
}

template<typename Id>
kfl::ELam<Id>::ELam(std::shared_ptr<BndVec> binders, std::shared_ptr<Expr<Id>> body)
  : super(), m_binders(binders), m_body(body)
{
  assert(m_binders);
  assert(m_binders->size() >= 1);
  assert(body);
}

template<typename Id>
void kfl::ELam<Id>::visit(Visitor<Id, void> & v) const
{
  return v.visitLam(*this);
}

template<typename Id>
const typename kfl::ELam<Id>::BndVec & kfl::ELam<Id>::getBinders() const
{
  assert(m_binders);
  return *m_binders;
}

template<typename Id>
const kfl::Expr<Id> & kfl::ELam<Id>::getBody() const
{
  assert(m_body);
  return *m_body;
}

template<typename Id>
const kfl::Expr<Id> & kfl::ECase<Id>::getExpr() const
{
  assert(m_expr);
  return *m_expr;
}

template<typename Id>
const typename kfl::ECase<Id>::AlterVec & kfl::ECase<Id>::getAlters() const
{
  assert(m_alters);
  return *m_alters;
}

template<typename Id>
kfl::ScDefn<Id>::ScDefn(const Id & name, std::shared_ptr<BndVec> binders,
                       std::shared_ptr<Expr<Id>> body)
  : m_name(name), m_binders(binders), m_body(body)
{
  assert(!m_name.empty());
  assert(m_body);
}

template<typename Id>
kfl::ScDefn<Id>::~ScDefn()
{
}

template<typename Id>
const Id & kfl::ScDefn<Id>::getName() const
{
  return m_name;
}

template<typename Id>
const typename kfl::ScDefn<Id>::BndVec * kfl::ScDefn<Id>::getBinders() const
{
  return m_binders.get();
}

template<typename Id>
const kfl::Expr<Id> & kfl::ScDefn<Id>::getBody() const
{
  assert(m_body);
  return *m_body;
}

template<typename Id>
kfl::Program<Id>::Program(std::shared_ptr<ScDefnVec> defns)
  : m_defns(defns)
{
}

template<typename Id>
kfl::Program<Id>::~Program()
{
}

template<typename Id>
const typename kfl::Program<Id>::ScDefnVec & kfl::Program<Id>::getDefns() const
{
  assert(m_defns);
  return *m_defns;
}
