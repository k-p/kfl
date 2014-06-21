/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __EXPR_FACTORY_H__
#define __EXPR_FACTORY_H__

#include "boost/noncopyable.hpp"

#include "Expr.h"

namespace kfl {

  template<class T>
  class ExprFactory : private boost::noncopyable {
  public:
    typedef typename ECase<T>::AlterVec AlterVec;
    typedef typename ECase<T>::BndVec BndVec;
    typedef typename ELet<T>::DefnVec DefnVec;
    typedef typename Program<T>::ScDefnVec ScDefnVec;

    virtual ~ExprFactory();

    virtual AlterVec * createAlters();
    virtual void addAlter(AlterVec* alters, ENum<T> * num, BndVec * bnds, Expr<T> * expr);

    virtual BndVec * createBnds();
    virtual void addBnd(BndVec * bnds, const T & name);

    virtual DefnVec * createDefns();
    virtual void addDefn(DefnVec * defns, const T & name, Expr<T> * expr);

    virtual Expr<T> * createVar(const T & id);
    virtual Expr<T> * createNum(int n);
    virtual Expr<T> * createConstr(int tag, int arity);
    virtual Expr<T> * createAp(Expr<T> * fn, Expr<T> * arg);
    virtual Expr<T> * createLet(DefnVec * defns, Expr<T> * body);
    virtual Expr<T> * createLetRec(DefnVec * defns, Expr<T> * body);
    virtual Expr<T> * createCase(Expr<T> * expr, AlterVec * alters);
    virtual Expr<T> * createLambda(BndVec * bnds, Expr<T> * body);

    virtual ScDefn<T> * createScDefn(const T & name, BndVec * bnds, Expr<T> * body);

    virtual ScDefnVec * createScDefns();
    virtual void addScDefn(ScDefnVec * defns, const T & name, BndVec * bnds, Expr<T> * body);
    virtual Program<T> * createProgram(ScDefnVec * defns);
  };

} /* namespace */

#include "ExprFactory.icc"

#endif /* __EXPR_FACTORY_H__ */
