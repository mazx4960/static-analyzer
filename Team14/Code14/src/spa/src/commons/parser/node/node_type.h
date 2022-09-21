// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

enum class NodeType {
  kProgram,
  kProcedure,
  kStatementList,
  kStatement,
  kVariable,
  kConstant,
  kCondExpr,
  kExpr,
};

enum class CondExprType {
  kNot,
  kAnd,
  kOr,
  kGt,
  kGte,
  kLt,
  kLte,
  kEq,
  kNeq,
};

enum class ExprType {
  kPlus,
  kMinus,
  kTimes,
  kDiv,
  kMod,
};