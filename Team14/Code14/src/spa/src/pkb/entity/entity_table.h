// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/result.h"
#include "commons/types.h"

class EntityTable {
 protected:
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> table_;

  EntityTable() = default;

 public:
  static EntityTable *getTable(EntityType);
  void populate(Entity &entity);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> get();


  // Currently only used for debugging and testing
  int getCount();
};

class VariableTable : public EntityTable {
 public:
  VariableTable() = default;
};

class ConstantTable : public EntityTable {
 public:
  ConstantTable() = default;
};

class ProcedureTable : public EntityTable {
 public:
  ProcedureTable() = default;
};

class StatementTable : public EntityTable {
 public:
  StatementTable() = default;
};