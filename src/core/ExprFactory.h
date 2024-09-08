/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Expr.h"

#include <boost/noncopyable.hpp>
#include <memory>

namespace kfl {

  template<class T>
  class ExprFactory : private boost::noncopyable {
  public:
    using AlterVec = typename ECase<T>::AlterVec;
    using BndVec = typename ECase<T>::BndVec;
    using DefnVec = typename ELet<T>::DefnVec;
    using ScDefnVec = typename Program<T>::ScDefnVec;

    virtual ~ExprFactory();

    virtual std::unique_ptr<AlterVec> createAlters();
    virtual void addAlter(AlterVec & alters, std::shared_ptr<ENum<T>> num,
                          std::shared_ptr<BndVec> bnds, std::shared_ptr<Expr<T>> expr);

    virtual std::unique_ptr<BndVec> createBnds();
    virtual void addBnd(BndVec & bnds, const T & name);

    virtual std::unique_ptr<DefnVec> createDefns();
    virtual void addDefn(DefnVec & defns, const T & name, std::shared_ptr<Expr<T>> expr);

    virtual std::unique_ptr<EVar<T>>        createVar(const T & id);
    virtual std::unique_ptr<ENum<T>>        createNum(int n);
    virtual std::unique_ptr<EConstr<T>>     createConstr(int tag, int arity);
    virtual std::unique_ptr<EAp<T>>         createAp(std::shared_ptr<Expr<T>> fn,
                                                     std::shared_ptr<Expr<T>> arg);
    virtual std::unique_ptr<ELet<T>>        createLet(std::shared_ptr<DefnVec> defns,
                                                      std::shared_ptr<Expr<T>> body);
    virtual std::unique_ptr<ELet<T>>        createLetRec(std::shared_ptr<DefnVec> defns,
                                                         std::shared_ptr<Expr<T>> body);
    virtual std::unique_ptr<ECase<T>>       createCase(std::shared_ptr<Expr<T>> expr,
                                                       std::shared_ptr<AlterVec> alters);
    virtual std::unique_ptr<ELam<T>>        createLambda(std::shared_ptr<BndVec> bnds,
                                                         std::shared_ptr<Expr<T>> body);

    virtual std::unique_ptr<ScDefn<T>>      createScDefn(const T & name,
                                                         std::shared_ptr<BndVec> bnds,
                                                         std::shared_ptr<Expr<T>> body);

    virtual std::unique_ptr<ScDefnVec> createScDefns();
    virtual void addScDefn(ScDefnVec & defns, const T & name, std::shared_ptr<BndVec> bnds,
                           std::shared_ptr<Expr<T>> body);

    virtual std::unique_ptr<Program<T>> createProgram(std::shared_ptr<ScDefnVec> defns);
  };

} /* namespace */

#include "ExprFactory.hpp"
