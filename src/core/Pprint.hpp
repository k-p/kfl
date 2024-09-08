/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Pprint.h"

#include <cassert>
#include <functional>
#include <ostream>
#include <string>

namespace {
  template<class T, class ObjectPtrs>
  void pprintListWithSeparator(ObjectPtrs objectPtrs, std::ostream & os, const std::string & separator)
  {
    bool first = true;
    for (auto o : objectPtrs) {
      if (o) {
        if (!first) {
          os << separator;
        }
        else {
          first = false;
        }
        kfl::Pprint<T> pp(os);
        pp.visit(*o);
      }
    }
  }

  void pprintListWithPrefix(const std::vector<std::string> & strs, std::ostream & os, char prefix)
  {
    for (const auto & s : strs) {
      os << prefix << s;
    }
  }
}

template<class T>
kfl::Pprint<T>::Pprint(std::ostream & os)
  : super(), m_os(os)
{
}

template<class T>
kfl::Pprint<T>::~Pprint()
{
}

template<class T>
void kfl::Pprint<T>::visitProgram(const Program<T> & p)
{
  pprintListWithSeparator<T>(p.getDefns(), m_os, ";\n");
  m_os << "\n";
}

template<class T>
void kfl::Pprint<T>::visitScDefn(const ScDefn<T> & d)
{
  m_os << d.getName();
  if (const typename ScDefn<T>::BndVec * binders = d.getBinders()) {
    pprintListWithPrefix(*binders, m_os, ' ');
  }
  m_os << " = ";
  super::visitExpr(d.getBody());
}

template<class T>
void kfl::Pprint<T>::visitVar(const EVar<T> & e)
{
  m_os << e.getId();
}

template<class T>
void kfl::Pprint<T>::visitNum(const ENum<T> & e)
{
  m_os << e.getNum();
}

template<class T>
void kfl::Pprint<T>::visitConstr(const EConstr<T> & e)
{
  m_os << "Pack{" << e.getTag() << ',' << e.getArity() << '}';
}

template<class T>
void kfl::Pprint<T>::visitAp(const EAp<T> & e)
{
  super::visitExpr(e.getFn());
  m_os << ' ';
  super::visitExpr(e.getArg());
}

template<class T>
void kfl::Pprint<T>::visitLet(const ELet<T> & e)
{
  m_os << "let" << std::endl;
  const typename ELet<T>::DefnVec & defns = e.getDefns();
  typename ELet<T>::DefnVec::const_iterator d = defns.begin();
  if (d != defns.end()) {
    m_os << "  ";
    visitLetDefn(*d);
    ++d;
  }
  for (; d != defns.end(); ++d) {
    m_os << ';' << std::endl << "  ";
    visitLetDefn(*d);
  }
  m_os << std::endl << "in ";
  super::visitExpr(e.getBody());
}

template<class T>
void kfl::Pprint<T>::visitLetDefn(const typename ELet<T>::Defn & defn)
{
  assert(defn.second);

  m_os << defn.first << " = " << (*defn.second);
}

template<class T>
void kfl::Pprint<T>::visitCase(const ECase<T> & e)
{
  m_os << "case ";
  super::visitExpr(e.getExpr());
  m_os << " of" << std::endl;
  const typename ECase<T>::AlterVec & alters = e.getAlters();
  typename ECase<T>::AlterVec::const_iterator a = alters.begin();
  if (a != alters.end()) {
    m_os << "  ";
    visitCaseAlter(*a);
    ++a;
  }
  while (a != alters.end()) {
    m_os << ';' << std::endl << "  ";
    visitCaseAlter(*a);
    ++a;
  }
}

template<class T>
void kfl::Pprint<T>::visitCaseAlter(const typename ECase<T>::Alter & alter)
{
  assert(alter.num);
  assert(alter.expr);

  m_os << '<';
  visitNum(*alter.num);
  m_os << '>';
  if (alter.binders) {
    pprintListWithPrefix(*alter.binders, m_os, ' ');
  }
  m_os << " -> ";
  super::visitExpr(*alter.expr);

}

template<class T>
void kfl::Pprint<T>::visitLam(const ELam<T> & e)
{
  m_os << '\\';
  pprintListWithPrefix(e.getBinders(), m_os, ' ');
  m_os << ". ";
  super::visitExpr(e.getBody());
}

template<class T>
std::ostream & operator<<(std::ostream & os, const kfl::Expr<T> & e)
{
  kfl::Pprint<T> pp (os);
  pp.visit(e);
  return os;
}

template<class T>
kfl::PprintA<T>::PprintA(std::ostream & os)
  : super(os)
{
}

template<class T>
kfl::PprintA<T>::~PprintA()
{
}

template<class T>
void kfl::PprintA<T>::visitExpr(const Expr<T> & e)
{
  bool atomic = e.isAtomic();
  if (!atomic) {
    super::m_os << '(';
  }
  super::visitExpr(e);
  if (!atomic) {
    super::m_os << ')';
  }
}
