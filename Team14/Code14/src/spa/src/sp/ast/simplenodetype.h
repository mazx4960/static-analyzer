#pragma once

enum class SimpleNodeType {
  kProgram,
  kProcedure,
  kStatementList,
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssign,
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
