#include <cassert>
#include <string>

#include "Expr.h"
#include "ExprBuilder.h"

template<class T>
kfl::Expr<T> * kfl::ExprBuilder<T>::createVar(const std::string & name)
{
  return new kfl::EVar<T>(name);
}

template<class T>
kfl::Expr<T> * kfl::ExprBuilder<T>::createNum(const Num & n)
{
  return new kfl::ENum<T>(n);
}
