// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_store.h"
EntityStore *EntityStore::getStore(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return new ProcedureTable();
    case EntityType::kStatement: return new StatementTable();
    case EntityType::kVariable: return new VariableTable();
    case EntityType::kConstant: return new ConstantTable();
    default: return nullptr;
  }
}
void EntityStore::populate(const std::string &entity_name) {
  this->table_.insert(entity_name);
}

Result EntityStore::getResult(QuerySynonym &synonym) {
  return Result(synonym, this->table_);
}

int EntityStore::getCount() {
  return this->table_.size();
}
