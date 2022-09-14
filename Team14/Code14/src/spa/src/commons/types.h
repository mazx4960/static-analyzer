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
};

enum class RsType {
  kFollows,
  kFollowsT,
  kParent,
  kParentT,
  kUses,
  kModifies,
};
