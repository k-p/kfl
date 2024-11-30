/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "Expr.h"
#include "ExprFactory.h"
#include "Visitor.h"

#include <string>

namespace kfl {

  using CoreId = std::string;

  using CoreExpr = Expr<CoreId>;
  using CoreVar = EVar<CoreId>;
  using CoreNum = ENum<CoreId>;
  using CoreConstr = EConstr<CoreId>;
  using CoreAp = EAp<CoreId>;
  using CoreLet = ELet<CoreId>;
  using CoreCase = ECase<CoreId>;
  using CoreLam = ELam<CoreId>;

  using CoreScDefn = ScDefn<CoreId>;
  using CoreProgram = Program<CoreId>;

  using CoreFactory = ExprFactory<CoreId>;

  template<typename T> class CoreVisitor : public Visitor<CoreId, T> { };
  template<typename T> class CoreDefaultVisitor : public DefaultVisitor<CoreId, T> { };
  template<typename T, typename Derived> class CoreFnExprVisitor : public FnExprVisitor<CoreId, T, Derived> { };
}
