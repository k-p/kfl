/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __CORE_EXPR_H__
#define __CORE_EXPR_H__

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

  using CoreVisitor = Visitor<CoreId>;
  using CoreDefaultVisitor = DefaultVisitor<CoreId>;
}

#endif /* __CORE_EXPR_H__ */
