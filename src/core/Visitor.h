/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __EXPR_VISITOR_H__
#define __EXPR_VISITOR_H__

#include "Expr.h"

namespace kfl {

  template<class T>
  class Visitor {
  public:
    virtual ~Visitor() { }
    void visit(const Program<T> & p) { visitProgram(p); }
    void visit(const ScDefn<T> & d) { visitScDefn(d); }
    void visit(const Expr<T> & e) { visitExpr(e); }
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

} /* end namespace kfl */

#endif /* __EXPR_VISITOR_H__ */
