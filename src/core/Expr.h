/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __EXPR_H__
#define __EXPR_H__

#include <ostream>
#include <utility>
#include <vector>

#include "boost/noncopyable.hpp"
#include "boost/scoped_ptr.hpp"

namespace kfl {

  template<class T> class Visitor;

  template<class T>
  class Expr : private boost::noncopyable {
  public:
    virtual ~Expr();
    virtual bool isAtomic() const;
    virtual void visit(Visitor<T> & v) const = 0;
    virtual bool operator==(const Expr<T> & other) const;
  };

  template<class T>
  class AtomicExpr : public Expr<T> {
    typedef Expr<T> super;
  public:
    virtual ~AtomicExpr();
    virtual bool isAtomic() const;
  };

  template<class T>
  class EVar : public AtomicExpr<T> {
    typedef AtomicExpr<T> super;
  public:
    EVar(const T & name);
    virtual ~EVar();
    virtual void visit(Visitor<T> & v) const;
    virtual const T & getId() const;
  private:
    const T m_name;
  };

  template<class T>
  class ENum : public AtomicExpr<T> {
    typedef AtomicExpr<T> super;
  public:
    ENum(int n);
    virtual ~ENum();
    virtual void visit(Visitor<T> & v) const;
    virtual int getNum() const;
  private:
    const int m_num;
  };

  template<class T>
  class EConstr : public Expr<T> {
    typedef Expr<T> super;
  public:
    EConstr(int tag, int arity);
    virtual ~EConstr();
    virtual void visit(Visitor<T> & v) const;
    virtual int getTag() const;
    virtual int getArity() const;
  private:
    int m_tag;
    int m_arity;
  };

  template<class T>
  class EAp : public Expr<T> {
    typedef Expr<T> super;
  public:
    EAp(Expr<T> * fn, Expr<T> * arg);
    virtual ~EAp();
    virtual void visit(Visitor<T> & v) const;
    virtual const Expr<T> & getFn() const;
    virtual const Expr<T> & getArg() const;
  private:
    boost::scoped_ptr< Expr<T> > m_fn;
    boost::scoped_ptr< Expr<T> > m_arg;
  };

  template<class T>
  class ELet : public Expr<T> {
    typedef Expr<T> super;
  public:
    typedef std::pair<T, Expr<T>*> Defn;
    typedef std::vector<Defn> DefnVec;
    ELet(DefnVec * defns, Expr<T> * body, bool isRec = false);
    virtual ~ELet();
    virtual void visit(Visitor<T> & v) const;
    virtual bool isRec() const;
    virtual const DefnVec & getDefns() const;
    virtual const Expr<T> & getBody() const;
  private:
    boost::scoped_ptr< DefnVec > m_defns;
    boost::scoped_ptr< Expr<T> > m_body;
    bool m_isRec;
  };

  template<class T>
  class ECase : public Expr<T> {
    typedef Expr<T> super;
  public:
    typedef std::vector<T> BndVec;
    struct Alter { ENum<T> * m_num; BndVec * m_binders; Expr<T> * m_expr; };
    typedef std::vector<Alter> AlterVec;
    ECase(Expr<T> * expr, AlterVec * alters);
    virtual ~ECase();
    virtual void visit(Visitor<T> & v) const;
    virtual const Expr<T> & getExpr() const;
    virtual const AlterVec & getAlters() const;
  private:
    boost::scoped_ptr< Expr<T> > m_expr;
    AlterVec * m_alters;
  };

  template<class T>
  class ELam : public Expr<T> {
    typedef Expr<T> super;
  public:
    typedef std::vector<T> BndVec;
    ELam(BndVec * binders, Expr<T> * body);
    virtual ~ELam();
    virtual void visit(Visitor<T> & v) const;
    virtual const BndVec & getBinders() const;
    virtual const Expr<T> & getBody() const;
  private:
    boost::scoped_ptr< BndVec > m_binders;
    boost::scoped_ptr< Expr<T> > m_body;
  };

  template<class T>
  class ScDefn : private boost::noncopyable {
  public:
    typedef std::vector<T> BndVec;
    ScDefn(const T & name, BndVec * binders, Expr<T> * body);
    virtual ~ScDefn();
    virtual const T & getName() const;
    virtual const BndVec * getBinders() const;
    virtual const Expr<T> & getBody() const;
  private:
    const T m_name;
    boost::scoped_ptr< BndVec > m_binders;
    boost::scoped_ptr< Expr<T> > m_body;
  };

  template<class T>
  class Program : private boost::noncopyable {
  public:
    typedef std::vector<ScDefn<T>*> ScDefnVec;
    Program(ScDefnVec * defns);
    virtual ~Program();
    virtual const ScDefnVec & getDefns() const;
  private:
    ScDefnVec * m_defns;
  };

} /* end namespace kfl */

#include "Expr.icc"

#endif /* __EXPR_H__ */
