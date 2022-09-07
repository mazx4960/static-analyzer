// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>

#include "commons/types.h"
#include "pkb/entity/result.h"

class EntityStorage {
 protected:
  std::unordered_set<std::string> table_;

 public:
  static EntityStorage *getStore(EntityType);

  void populate(const std::string &);
  Result getResult(QuerySynonym &);
  int getCount();
};

class VariableTable : public EntityStorage {
 public:
  VariableTable() = default;
};

class ConstantTable : public EntityStorage {
 public:
  ConstantTable() = default;
};

class ProcedureTable : public EntityStorage {
 public:
  ProcedureTable() = default;
};

class StatementTable : public EntityStorage {
 public:
  StatementTable() = default;
};