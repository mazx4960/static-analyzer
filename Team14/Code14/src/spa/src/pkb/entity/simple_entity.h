// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>

#include "commons/types.h"
#include "pkb/entity/result.h"

class SimpleEntityTable {
 protected:
  std::unordered_set<std::string> table_;

 public:
  static SimpleEntityTable *getTable(EntityType);

  void populate(const std::string &);
  Result getResult(QuerySynonym &);
  int getCount();
};

class VariableTable : public SimpleEntityTable {
 public:
  VariableTable() = default;
};

class ConstantTable : public SimpleEntityTable {
 public:
  ConstantTable() = default;
};

class ProcedureTable : public SimpleEntityTable {
 public:
  ProcedureTable() = default;
};

class StatementTable : public SimpleEntityTable {
 public:
  StatementTable() = default;
};