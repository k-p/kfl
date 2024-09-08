/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __EXPR_ICC__
#define __EXPR_ICC__

#include "Expr.h"
#include "Visitor.h"

#include <cassert>
#include <functional>

template<typename T>
kfl::Expr<T>::~Expr()
{
}

template<typename T>
bool kfl::Expr<T>::isAtomic() const
{
  return false;
}

template<typename T>
bool kfl::AtomicExpr<T>::isAtomic() const
{
  return true;
}

template<typename T>
kfl::EVar<T>::EVar(const T & name)
  : super(), m_name(name)
{
  assert(!m_name.empty());
}

template<typename T>
void kfl::EVar<T>::visit(Visitor<T> & v) const
{
  v.visitVar(*this);
}

template<typename T>
const T & kfl::EVar<T>::getId() const
{
  return m_name;
}

template<typename T>
kfl::ENum<T>::ENum(int n)
  : super(), m_num(n)
{
}

template<typename T>
void kfl::ENum<T>::visit(Visitor<T> & v) const
{
  v.visitNum(*this);
}

template<typename T>
int kfl::ENum<T>::getNum() const
{
  return m_num;
}

template<typename T>
kfl::EConstr<T>::EConstr(int tag, int arity)
  : super(), m_tag(tag), m_arity(arity)
{
}

template<typename T>
void kfl::EConstr<T>::visit(Visitor<T> & v) const
{
  v.visitConstr(*this);
}

template<typename T>
int kfl::EConstr<T>::getTag() const
{
  return m_tag;
}

template<typename T>
int kfl::EConstr<T>::getArity() const
{
  return m_arity;
}

template<typename T>
kfl::EAp<T>::EAp(std::shared_ptr<Expr<T>> fn, std::shared_ptr<Expr<T>> arg)
  : super(), m_fn(fn), m_arg(arg)
{
  assert(m_fn);
  assert(m_arg);
}

template<typename T>
void kfl::EAp<T>::visit(Visitor<T> & v) const
{
  v.visitAp(*this);
}

template<typename T>
const kfl::Expr<T> & kfl::EAp<T>::getFn() const
{
  assert(m_fn);
  return *m_fn;
}

template<typename T>
const kfl::Expr<T> & kfl::EAp<T>::getArg() const
{
  assert(m_arg);
  return *m_arg;
}

template<typename T>
kfl::ELet<T>::ELet(std::shared_ptr<DefnVec> defns, std::shared_ptr<Expr<T>> body, bool isRec)
  : super(), m_defns(defns), m_body(body), m_isRec(isRec)
{
  assert(m_defns);
  assert(m_defns->size() >= 1);
  assert(m_body);
}

template<typename T>
void kfl::ELet<T>::visit(Visitor<T> & v) const
{
  v.visitLet(*this);
}

template<typename T>
bool kfl::ELet<T>::isRec() const
{
  return m_isRec;
}

template<typename T>
const typename kfl::ELet<T>::DefnVec & kfl::ELet<T>::getDefns() const
{
  assert(m_defns);
  return *m_defns;
}

template<typename T>
const kfl::Expr<T> & kfl::ELet<T>::getBody() const
{
  assert(m_body);
  return *m_body;
}

template<typename T>
kfl::ECase<T>::ECase(std::shared_ptr<Expr<T>> expr, std::shared_ptr<AlterVec> alters)
  : super(), m_expr(expr), m_alters(alters)
{
  assert(m_expr);
  assert(m_alters);
  assert(m_alters->size() >= 1);
}

template<typename T>
void kfl::ECase<T>::visit(Visitor<T> & v) const
{
  v.visitCase(*this);
}

template<typename T>
kfl::ELam<T>::ELam(std::shared_ptr<BndVec> binders, std::shared_ptr<Expr<T>> body)
  : super(), m_binders(binders), m_body(body)
{
  assert(m_binders);
  assert(m_binders->size() >= 1);
  assert(body);
}

template<typename T>
void kfl::ELam<T>::visit(Visitor<T> & v) const
{
  v.visitLam(*this);
}

template<typename T>
const typename kfl::ELam<T>::BndVec & kfl::ELam<T>::getBinders() const
{
  assert(m_binders);
  return *m_binders;
}

template<typename T>
const kfl::Expr<T> & kfl::ELam<T>::getBody() const
{
  assert(m_body);
  return *m_body;
}

template<typename T>
const kfl::Expr<T> & kfl::ECase<T>::getExpr() const
{
  assert(m_expr);
  return *m_expr;
}

template<typename T>
const typename kfl::ECase<T>::AlterVec & kfl::ECase<T>::getAlters() const
{
  assert(m_alters);
  return *m_alters;
}

template<typename T>
kfl::ScDefn<T>::ScDefn(const T & name, std::shared_ptr<BndVec> binders,
                       std::shared_ptr<Expr<T>> body)
  : m_name(name), m_binders(binders), m_body(body)
{
  assert(!m_name.empty());
  assert(m_body);
}

template<typename T>
kfl::ScDefn<T>::~ScDefn()
{
}

template<typename T>
const T & kfl::ScDefn<T>::getName() const
{
  return m_name;
}

template<typename T>
const typename kfl::ScDefn<T>::BndVec * kfl::ScDefn<T>::getBinders() const
{
  return m_binders.get();
}

template<typename T>
const kfl::Expr<T> & kfl::ScDefn<T>::getBody() const
{
  assert(m_body);
  return *m_body;
}

template<typename T>
kfl::Program<T>::Program(std::shared_ptr<ScDefnVec> defns)
  : m_defns(defns)
{
}

template<typename T>
kfl::Program<T>::~Program()
{
}

template<typename T>
const typename kfl::Program<T>::ScDefnVec & kfl::Program<T>::getDefns() const
{
  assert(m_defns);
  return *m_defns;
}

#endif /* __EXPR_ICC__ */
