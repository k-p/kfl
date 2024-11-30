/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Expr.h"
#include "Visitor.h"

#include "boost/noncopyable.hpp"

#include <ostream>

namespace kfl {

  template<class T>
  class Pprint : public Visitor<T, void>, private boost::noncopyable {
    using super = Visitor<T, void>;
  public:
    Pprint(std::ostream & os);
    virtual ~Pprint();
  protected:
    void visitProgram(const Program<T> & p) override;
    void visitScDefn(const ScDefn<T> & d) override;
    void visitVar(const EVar<T> & e) override;
    void visitNum(const ENum<T> & e) override;
    void visitConstr(const EConstr<T> & e) override;
    void visitAp(const EAp<T> & e) override;
    void visitLet(const ELet<T> & e) override;
    void visitLetDefn(const typename ELet<T>::Defn & defn);
    void visitCase(const ECase<T> & e) override;
    void visitCaseAlter(const typename ECase<T>::Alter & alter);
    void visitLam(const ELam<T> & e) override;
  protected:
    std::ostream & m_os;
  };

  template<class T>
  class PprintA : public Pprint<T> {
    using super = Pprint<T>;
  public:
    PprintA(std::ostream & os);
    virtual ~PprintA();
    virtual void visitExpr(const Expr<T> & e);
  };

  template<class T>
  std::ostream & operator<<(std::ostream & os, const Expr<T> & expr);

} /* end namespace kfl */

#include "Pprint.hpp"
