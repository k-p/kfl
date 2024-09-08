/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include <boost/noncopyable.hpp>

#include <memory>
#include <ostream>
#include <utility>
#include <vector>

namespace kfl {

  template<typename T> class Visitor;

  template<typename T>
  class Expr : private boost::noncopyable {
  public:
    virtual ~Expr();
    virtual bool isAtomic() const;
    virtual void visit(Visitor<T> & v) const = 0;
  };

  template<typename T>
  class AtomicExpr : public Expr<T> {
    using super = Expr<T>;
  public:
    bool isAtomic() const override;
  };

  template<typename T>
  class EVar : public AtomicExpr<T> {
    using super = AtomicExpr<T>;
  public:
    EVar(const T & name);
    void visit(Visitor<T> & v) const override;
    const T & getId() const;
  private:
    const T m_name;
  };

  template<typename T>
  class ENum : public AtomicExpr<T> {
    using super = AtomicExpr<T>;
  public:
    ENum(int n);
    void visit(Visitor<T> & v) const override;
    int getNum() const;
  private:
    const int m_num;
  };

  template<typename T>
  class EConstr : public Expr<T> {
    using super = Expr<T>;
  public:
    EConstr(int tag, int arity);
    void visit(Visitor<T> & v) const override;
    int getTag() const;
    int getArity() const;
  private:
    const int m_tag;
    const int m_arity;
  };

  template<typename T>
  class EAp : public Expr<T> {
    using super = Expr<T>;
  public:
    EAp(std::shared_ptr<Expr<T>> fn, std::shared_ptr<Expr<T>> arg);
    void visit(Visitor<T> & v) const override;
    const Expr<T> & getFn() const;
    const Expr<T> & getArg() const;
  private:
    std::shared_ptr<Expr<T>> m_fn;
    std::shared_ptr<Expr<T>> m_arg;
  };

  template<typename T>
  class ELet : public Expr<T> {
    using super = Expr<T>;
  public:
    using Defn = std::pair<T, std::shared_ptr<Expr<T>>>;
    using DefnVec = std::vector<Defn>;
    ELet(std::shared_ptr<DefnVec> defns, std::shared_ptr<Expr<T>> body, bool isRec = false);
    void visit(Visitor<T> & v) const override;
    bool isRec() const;
    const DefnVec & getDefns() const;
    const Expr<T> & getBody() const;
  private:
    std::shared_ptr<DefnVec> m_defns;
    std::shared_ptr<Expr<T>> m_body;
    const bool m_isRec;
  };

  template<typename T>
  class ECase : public Expr<T> {
    using super = Expr<T>;
  public:
    using BndVec = std::vector<T>;
    struct Alter {
      std::shared_ptr<ENum<T>> num;
      std::shared_ptr<BndVec> binders;
      std::shared_ptr<Expr<T>> expr;
    };
    using AlterVec = std::vector<Alter>;
    ECase(std::shared_ptr<Expr<T>> expr, std::shared_ptr<AlterVec> alters);
    void visit(Visitor<T> & v) const override;
    const Expr<T> & getExpr() const;
    const AlterVec & getAlters() const;
  private:
    std::shared_ptr<Expr<T>> m_expr;
    std::shared_ptr<AlterVec> m_alters;
  };

  template<typename T>
  class ELam : public Expr<T> {
    using super = Expr<T>;
  public:
    using BndVec = std::vector<T>;
    ELam(std::shared_ptr<BndVec> binders, std::shared_ptr<Expr<T>> body);
    void visit(Visitor<T> & v) const override;
    const BndVec & getBinders() const;
    const Expr<T> & getBody() const;
  private:
    std::shared_ptr<BndVec> m_binders;
    std::shared_ptr<Expr<T>> m_body;
  };

  template<typename T>
  class ScDefn : private boost::noncopyable {
  public:
    using BndVec = std::vector<T>;
    ScDefn(const T & name, std::shared_ptr<BndVec> binders, std::shared_ptr<Expr<T>> body);
    virtual ~ScDefn();
    const T & getName() const;
    const BndVec * getBinders() const;
    const Expr<T> & getBody() const;
  private:
    const T m_name;
    std::shared_ptr<BndVec> m_binders;
    std::shared_ptr<Expr<T>> m_body;
  };

  template<typename T>
  class Program : private boost::noncopyable {
  public:
    using ScDefnVec = std::vector<std::shared_ptr<ScDefn<T>>>;
    Program(std::shared_ptr<ScDefnVec> defns);
    virtual ~Program();
    const ScDefnVec & getDefns() const;
  private:
    std::shared_ptr<ScDefnVec> m_defns;
  };

} /* end namespace kfl */

#include "Expr.hpp"
