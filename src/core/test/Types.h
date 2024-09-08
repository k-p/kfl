/* -*-c++-*-
 *
 *  Created by Keith Dennison on 12/02/2021.
 *  Copyright (c) 2021 Keith Dennison. All rights reserved.
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include "Expr.h"

using TestEAp = kfl::EAp<std::string>;
using TestECase = kfl::ECase<std::string>;
using TestEConstr = kfl::EConstr<std::string>;
using TestELam = kfl::ELam<std::string>;
using TestELet = kfl::ELet<std::string>;
using TestENum = kfl::ENum<std::string>;
using TestEVar = kfl::EVar<std::string>;
using TestExpr = kfl::Expr<std::string>;
using TestProgram = kfl::Program<std::string>;
using TestScDefn = kfl::ScDefn<std::string>;

#endif /* __TYPES_H__ */
