/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __EXPR_FACTORY_ICC__
#define __EXPR_FACTORY_ICC__

template<class T>
kfl::ExprFactory<T>::~ExprFactory()
{
}

template<class T>
std::unique_ptr<typename kfl::ExprFactory<T>::AlterVec>
kfl::ExprFactory<T>::createAlters()
{
  return std::make_unique<AlterVec>();
}

template<class T>
void
kfl::ExprFactory<T>::addAlter(AlterVec & alters, std::shared_ptr<ENum<T>> num,
                              std::shared_ptr<BndVec> bnds, std::shared_ptr<Expr<T>> expr)
{
  using Alter = typename ECase<T>::Alter;
  alters.push_back(Alter{num, bnds, expr});
}

template<class T>
std::unique_ptr<typename kfl::ExprFactory<T>::BndVec>
kfl::ExprFactory<T>::createBnds()
{
  return std::make_unique<BndVec>();
}

template<class T>
void
kfl::ExprFactory<T>::addBnd(BndVec & bnds, const T & name)
{
  bnds.push_back(name);
}

template<class T>
std::unique_ptr<typename kfl::ExprFactory<T>::DefnVec>
kfl::ExprFactory<T>::createDefns()
{
  return std::make_unique<DefnVec>();
}

template<class T>
void
kfl::ExprFactory<T>::addDefn(DefnVec & defns, const T & name, std::shared_ptr<Expr<T>> expr)
{
  using Defn = typename ELet<T>::Defn;
  defns.push_back(Defn(name, expr));
}

template<class T>
std::unique_ptr<kfl::EVar<T>>
kfl::ExprFactory<T>::createVar(const T & id)
{
  return std::make_unique<EVar<T>>(id);
}

template<class T>
std::unique_ptr<kfl::ENum<T>>
kfl::ExprFactory<T>::createNum(int n)
{
  return std::make_unique<ENum<T>>(n);
}

template<class T>
std::unique_ptr<kfl::EConstr<T>>
kfl::ExprFactory<T>::createConstr(int tag, int arity)
{
  return std::make_unique<EConstr<T>>(tag, arity);
}

template<class T>
std::unique_ptr<kfl::EAp<T>>
kfl::ExprFactory<T>::createAp(std::shared_ptr<Expr<T>> fn,
                              std::shared_ptr<Expr<T>> arg)
{
  return std::make_unique<EAp<T>>(fn, arg);
}

template<class T>
std::unique_ptr<kfl::ELet<T>>
kfl::ExprFactory<T>::createLet(std::shared_ptr<DefnVec> defns,
                               std::shared_ptr<Expr<T>> body)
{
  return std::make_unique<ELet<T>>(defns, body);
}

template<class T>
std::unique_ptr<kfl::ELet<T>>
kfl::ExprFactory<T>::createLetRec(std::shared_ptr<DefnVec> defns,
                                  std::shared_ptr<Expr<T>> body)
{
  return std::make_unique<ELet<T>>(defns, body, true);
}

template<class T>
std::unique_ptr<kfl::ECase<T>>
kfl::ExprFactory<T>::createCase(std::shared_ptr<Expr<T>> expr,
                                std::shared_ptr<AlterVec> alters)
{
  return std::make_unique<ECase<T>>(expr, alters);
}

template<class T>
std::unique_ptr<kfl::ELam<T>>
kfl::ExprFactory<T>::createLambda(std::shared_ptr<BndVec> bnds, std::shared_ptr<Expr<T>> expr)
{
  return std::make_unique<ELam<T>>(bnds, expr);
}

template<class T>
std::unique_ptr<kfl::ScDefn<T>>
kfl::ExprFactory<T>::createScDefn(const T & name, std::shared_ptr<BndVec> bnds,
                                  std::shared_ptr<Expr<T>> body)
{
  return std::make_unique<ScDefn<T>>(name, bnds, body);
}

template<class T>
std::unique_ptr<typename kfl::ExprFactory<T>::ScDefnVec>
kfl::ExprFactory<T>::createScDefns()
{
  return std::make_unique<ScDefnVec>();
}

template<class T>
void
kfl::ExprFactory<T>::addScDefn(ScDefnVec & defns, const T & name, std::shared_ptr<BndVec> bnds,
                               std::shared_ptr<Expr<T>> body)
{
  defns.push_back(createScDefn(name, bnds, body));
}

template<class T>
std::unique_ptr<kfl::Program<T>>
kfl::ExprFactory<T>::createProgram(std::shared_ptr<ScDefnVec> defns)
{
  return std::make_unique<Program<T>>(defns);
}

#endif /* __EXPR_FACTORY_ICC__ */
