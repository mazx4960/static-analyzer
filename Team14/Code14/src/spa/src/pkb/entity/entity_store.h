// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/types.h"
#include "pkb/entity/result.h"

class EntityStore {
 protected:
  std::unordered_set<std::string> table_;

 public:
  static EntityStore *getStore(EntityType);
  void save(Entity &entity);
  Result get(QuerySynonym &synonym);

  // Currently only used for debugging and testing
  int getCount();
};

class VariableTable : public EntityStore {
 public:
  VariableTable() = default;
};

class ConstantTable : public EntityStore {
 public:
  ConstantTable() = default;
};

class ProcedureTable : public EntityStore {
 public:
  ProcedureTable() = default;
};

class StatementTable : public EntityStore {
 public:
  StatementTable() = default;
};