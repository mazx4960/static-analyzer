// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_storage.h"
EntityStorage *EntityStorage::getTable(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return new ProcedureTable();
    case EntityType::kStatement: return new StatementTable();
    case EntityType::kVariable: return new VariableTable();
    case EntityType::kConstant: return new ConstantTable();
    default: return nullptr;
  }
}
void EntityStorage::populate(const std::string &entity_name) {
  this->table_.insert(entity_name);
}

Result EntityStorage::getResult(QuerySynonym &synonym) {
  return Result(synonym, this->table_);
}

int EntityStorage::getCount() {
  return this->table_.size();
}
