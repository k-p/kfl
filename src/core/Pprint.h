/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __PPRINT_H__
#define __PPRINT_H__

#include <ostream>

#include "boost/noncopyable.hpp"

#include "Expr.h"
#include "Visitor.h"

namespace kfl {

  template<class T>
  class Pprint : public Visitor<T>, private boost::noncopyable {
    typedef Visitor<T> super;
  public:
    Pprint(std::ostream & os);
    virtual ~Pprint();
  protected:
    virtual void visitProgram(const Program<T> & p);
    virtual void visitScDefn(const ScDefn<T> & d);
    virtual void visitVar(const EVar<T> & e);
    virtual void visitNum(const ENum<T> & e);
    virtual void visitConstr(const EConstr<T> & e);
    virtual void visitAp(const EAp<T> & e);
    virtual void visitLet(const ELet<T> & e);
    virtual void visitLetDefn(const typename ELet<T>::Defn & defn);
    virtual void visitCase(const ECase<T> & e);
    virtual void visitCaseAlter(const typename ECase<T>::Alter & alter);
    virtual void visitLam(const ELam<T> & e);
  protected:
    std::ostream & m_os;
  };

  template<class T>
  class PprintA : public Pprint<T> {
    typedef Pprint<T> super;
  public:
    PprintA(std::ostream & os);
    virtual ~PprintA();
    virtual void visitExpr(const Expr<T> & e);
  };

} /* end namespace kfl */

template<class T>
std::ostream & operator<<(std::ostream & os, const kfl::Expr<T> & expr);

#include "Pprint.icc"

#endif /* __PPRINT_H__ */
