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

  template<typename Id, typename T> class Visitor;

  template<typename Id>
  class Expr : private boost::noncopyable {
  public:
    virtual ~Expr();
    virtual bool isAtomic() const;
    virtual void visit(Visitor<Id, void> & v) const = 0;
  };

  template<typename Id>
  class AtomicExpr : public Expr<Id> {
    using super = Expr<Id>;
  public:
    bool isAtomic() const override;
  };

  template<typename Id>
  class EVar : public AtomicExpr<Id> {
    using super = AtomicExpr<Id>;
  public:
    EVar(const Id & name);
    void visit(Visitor<Id, void> & v) const override;
    const Id & getId() const;
  private:
    const Id m_name;
  };

  template<typename Id>
  class ENum : public AtomicExpr<Id> {
    using super = AtomicExpr<Id>;
  public:
    ENum(int n);
    void visit(Visitor<Id, void> & v) const override;
    int getNum() const;
  private:
    const int m_num;
  };

  template<typename Id>
  class EConstr : public Expr<Id> {
    using super = Expr<Id>;
  public:
    EConstr(int tag, int arity);
    void visit(Visitor<Id, void> & v) const override;
    int getTag() const;
    int getArity() const;
  private:
    const int m_tag;
    const int m_arity;
  };

  template<typename Id>
  class EAp : public Expr<Id> {
    using super = Expr<Id>;
  public:
    EAp(std::shared_ptr<Expr<Id>> fn, std::shared_ptr<Expr<Id>> arg);
    void visit(Visitor<Id, void> & v) const override;
    const Expr<Id> & getFn() const;
    const Expr<Id> & getArg() const;
  private:
    std::shared_ptr<Expr<Id>> m_fn;
    std::shared_ptr<Expr<Id>> m_arg;
  };

  template<typename Id>
  class ELet : public Expr<Id> {
    using super = Expr<Id>;
  public:
    using Defn = std::pair<Id, std::shared_ptr<Expr<Id>>>;
    using DefnVec = std::vector<Defn>;
    ELet(std::shared_ptr<DefnVec> defns, std::shared_ptr<Expr<Id>> body, bool isRec = false);
    void visit(Visitor<Id, void> & v) const override;
    bool isRec() const;
    const DefnVec & getDefns() const;
    const Expr<Id> & getBody() const;
  private:
    std::shared_ptr<DefnVec> m_defns;
    std::shared_ptr<Expr<Id>> m_body;
    const bool m_isRec;
  };

  template<typename Id>
  class ECase : public Expr<Id> {
    using super = Expr<Id>;
  public:
    using BndVec = std::vector<Id>;
    struct Alter {
      std::shared_ptr<ENum<Id>> num;
      std::shared_ptr<BndVec> binders;
      std::shared_ptr<Expr<Id>> expr;
    };
    using AlterVec = std::vector<Alter>;
    ECase(std::shared_ptr<Expr<Id>> expr, std::shared_ptr<AlterVec> alters);
    void visit(Visitor<Id, void> & v) const override;
    const Expr<Id> & getExpr() const;
    const AlterVec & getAlters() const;
  private:
    std::shared_ptr<Expr<Id>> m_expr;
    std::shared_ptr<AlterVec> m_alters;
  };

  template<typename Id>
  class ELam : public Expr<Id> {
    using super = Expr<Id>;
  public:
    using BndVec = std::vector<Id>;
    ELam(std::shared_ptr<BndVec> binders, std::shared_ptr<Expr<Id>> body);
    void visit(Visitor<Id, void> & v) const override;
    const BndVec & getBinders() const;
    const Expr<Id> & getBody() const;
  private:
    std::shared_ptr<BndVec> m_binders;
    std::shared_ptr<Expr<Id>> m_body;
  };

  template<typename Id>
  class ScDefn : private boost::noncopyable {
  public:
    using BndVec = std::vector<Id>;
    ScDefn(const Id & name, std::shared_ptr<BndVec> binders, std::shared_ptr<Expr<Id>> body);
    virtual ~ScDefn();
    const Id & getName() const;
    const BndVec * getBinders() const;
    const Expr<Id> & getBody() const;
  private:
    const Id m_name;
    std::shared_ptr<BndVec> m_binders;
    std::shared_ptr<Expr<Id>> m_body;
  };

  template<typename Id>
  class Program : private boost::noncopyable {
  public:
    using ScDefnVec = std::vector<std::shared_ptr<ScDefn<Id>>>;
    Program(std::shared_ptr<ScDefnVec> defns);
    virtual ~Program();
    const ScDefnVec & getDefns() const;
  private:
    std::shared_ptr<ScDefnVec> m_defns;
  };

} /* end namespace kfl */

#include "Expr.hpp"
