// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

enum class EntityType {
  kProcedure,
  kStatement,
  kVariable,
  kConstant,
  // Statement types
  kAssignStmt,
  kCallStmt,
  kIfStmt,
  kWhileStmt,
  kPrintStmt,
  kReadStmt,
  // Used by PQL
  kWildcard,
  kExpression,
  kWildcardExpression,
  kString,
  kInteger,
};

enum class RsType {
  kFollows,
  kParent,
  kUses,
  kModifies,
  // Used by PQL
  kParentAll,
  kFollowsAll,
  kAssignPattern,
};
