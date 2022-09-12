// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

enum class EntityType {
  kProcedure,
  kStatement,
  kVariable,
  kConstant,
};

enum class StmtType {
  kAssign,
  kCall,
  kIf,
  kWhile,
  kPrint,
  kRead,
};

enum class RsType {
  kFollows,
  kParent,
  kUses,
  kModifies,
};
