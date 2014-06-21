/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#ifndef __CORE_EXPR_H__
#define __CORE_EXPR_H__

#include "Expr.h"
#include "ExprFactory.h"

#include <string>

namespace kfl {

  typedef std::string CoreId;

  typedef Expr<CoreId> CoreExpr;
  typedef EVar<CoreId> CoreVar;
  typedef ENum<CoreId> CoreNum;
  typedef EConstr<CoreId> CoreConstr;
  typedef EAp<CoreId> CoreAp;
  typedef ELet<CoreId> CoreLet;
  typedef ECase<CoreId> CoreCase;
  typedef ELam<CoreId> CoreLam;

  typedef ScDefn<CoreId> CoreScDefn;
  typedef Program<CoreId> CoreProgram;

  typedef ExprFactory<CoreId> CoreExprFactory;

}

#endif /* __CORE_EXPR_H__ */
