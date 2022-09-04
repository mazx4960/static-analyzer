#pragma once

enum class SimpleNodeType {
  kProgram,
  kProcedure,
  kStatementList,
  kStatement,
  // TODO: refactor these start
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
  // TODO: refactor these end
  kVariable,
  kConstant
};

enum class SimpleStmtNodeType {
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssign,
};
