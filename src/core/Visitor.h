/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Expr.h"

namespace kfl {

  template<typename T>
  class Visitor {
  public:
    virtual ~Visitor() { }
    inline void visit(const Program<T> & p) { visitProgram(p); }
    inline void visit(const ScDefn<T> & d) { visitScDefn(d); }
    inline void visit(const Expr<T> & e) { visitExpr(e); }
  protected:
    virtual void visitProgram(const Program<T> & p) = 0;
    virtual void visitScDefn(const ScDefn<T> & d) = 0;
    virtual void visitExpr(const Expr<T> & e) { e.visit(*this); }
    virtual void visitVar(const EVar<T> & e) = 0;
    virtual void visitNum(const ENum<T> & e) = 0;
    virtual void visitConstr(const EConstr<T> & e) = 0;
    virtual void visitAp(const EAp<T> & e) = 0;
    virtual void visitLet(const ELet<T> & e) = 0;
    virtual void visitCase(const ECase<T> & e) = 0;
    virtual void visitLam(const ELam<T> & e) = 0;

    friend class EVar<T>;
    friend class ENum<T>;
    friend class EConstr<T>;
    friend class EAp<T>;
    friend class ELet<T>;
    friend class ECase<T>;
    friend class ELam<T>;
  };

  template<typename T>
  class DefaultVisitor : public Visitor<T> {
  protected:
    void visitProgram(const Program<T> & p) override { }
    void visitScDefn(const ScDefn<T> & d) override { }
    void visitVar(const EVar<T> & e) override { }
    void visitNum(const ENum<T> & e) override { }
    void visitConstr(const EConstr<T> & e) override { }
    void visitAp(const EAp<T> & e) override { }
    void visitLet(const ELet<T> & e) override { }
    void visitCase(const ECase<T> & e) override { }
    void visitLam(const ELam<T> & e) override { }
  };

} /* end namespace kfl */
