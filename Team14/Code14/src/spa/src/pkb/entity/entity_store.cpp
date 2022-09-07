// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_store.h"
EntityStore *EntityStore::getStore(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return new ProcedureTable();
    case EntityType::kStatement: return new StatementTable();
    case EntityType::kVariable: return new VariableTable();
    case EntityType::kConstant: return new ConstantTable();
    default: return nullptr;
  }
}
void EntityStore::save(Entity &entity) {
  this->table_.insert(entity.GetName());
}

Result EntityStore::get(QuerySynonym &synonym) {
  return Result(synonym, this->table_);
}

int EntityStore::getCount() {
  return this->table_.size();
}
