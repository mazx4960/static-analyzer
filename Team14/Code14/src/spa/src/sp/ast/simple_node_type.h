#pragma once

enum class SimpleNodeType {
  kProgram,
  kProcedure,
  kStatementList,
  kStatement,
  kNot,
  kAnd,
  kOr,
  kGt,
  kGte,
  kLt,
  kLte,
  kEq,
  kNeq,
  kPlus,
  kMinus,
  kTimes,
  kDiv,
  kMod,
  kVariable,
  kConstant
};
