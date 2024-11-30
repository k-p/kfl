/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Expr.h"

namespace kfl {

  template<typename Id, typename T>
  class Visitor {
  public:
    virtual ~Visitor() { }
    inline T visit(const Program<Id> & p) { return visitProgram(p); }
    inline T visit(const ScDefn<Id> & d) { return visitScDefn(d); }
    inline T visit(const Expr<Id> & e) { return visitExpr(e); }
  private:
    virtual T visitProgram(const Program<Id> & p) = 0;
    virtual T visitScDefn(const ScDefn<Id> & d) = 0;
    virtual T visitExpr(const Expr<Id> & e) { return e.visit(*this); }
    virtual T visitVar(const EVar<Id> & e) = 0;
    virtual T visitNum(const ENum<Id> & e) = 0;
    virtual T visitConstr(const EConstr<Id> & e) = 0;
    virtual T visitAp(const EAp<Id> & e) = 0;
    virtual T visitLet(const ELet<Id> & e) = 0;
    virtual T visitCase(const ECase<Id> & e) = 0;
    virtual T visitLam(const ELam<Id> & e) = 0;

    friend class EVar<Id>;
    friend class ENum<Id>;
    friend class EConstr<Id>;
    friend class EAp<Id>;
    friend class ELet<Id>;
    friend class ECase<Id>;
    friend class ELam<Id>;
  };

  template<typename Id, typename T>
  class DefaultVisitor : public Visitor<Id, T> {
  private:
    T visitProgram(const Program<Id> & p) override { return T(); }
    T visitScDefn(const ScDefn<Id> & d) override { return T(); }
    T visitVar(const EVar<Id> & e) override { return T(); }
    T visitNum(const ENum<Id> & e) override { return T(); }
    T visitConstr(const EConstr<Id> & e) override { return T(); }
    T visitAp(const EAp<Id> & e) override { return T(); }
    T visitLet(const ELet<Id> & e) override { return T(); }
    T visitCase(const ECase<Id> & e) override { return T(); }
    T visitLam(const ELam<Id> & e) override { return T(); }
  };

  template<typename Id, typename T, typename Derived>
  class FnExprVisitor : public DefaultVisitor<Id, void>
  {
  public:
    ~FnExprVisitor() override = default;
    inline T operator()(const Expr<Id>& e) { e.visit(*this); return result_; }

  private:
    void visitVar(const EVar<Id> & e) override {
      result_ = static_cast<Derived*>(this)->visit(e);
    }
    void visitNum(const ENum<Id> & e) override {
      result_ = static_cast<Derived*>(this)->visit(e);
    }
    void visitConstr(const EConstr<Id> & e) override {
      result_ = static_cast<Derived*>(this)->visit(e);
    }
    void visitAp(const EAp<Id> & e) override {
      result_ = static_cast<Derived*>(this)->visit(e);
    }
    void visitLet(const ELet<Id> & e) override {
      result_ = static_cast<Derived*>(this)->visit(e);
    }
    void visitCase(const ECase<Id> & e) override {
      result_ = static_cast<Derived*>(this)->visit(e);
    }
    void visitLam(const ELam<Id> & e) override {
      result_ = static_cast<Derived*>(this)->visit(e);
    }

    T result_;
  };

} /* end namespace kfl */
