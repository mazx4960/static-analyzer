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
  void populate(Entity &entity);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> get(); 
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

class AssignTable : public EntityTable {
 public:
  AssignTable() = default;
};

class CallTable : public EntityTable {
 public:
  CallTable() = default;
};

class IfTable : public EntityTable {
 public:
  IfTable() = default;
};

class WhileTable : public EntityTable {
 public:
  WhileTable() = default;
};

class ReadTable : public EntityTable {
 public:
  ReadTable() = default;
};

class PrintTable : public EntityTable {
 public:
  PrintTable() = default;
};
