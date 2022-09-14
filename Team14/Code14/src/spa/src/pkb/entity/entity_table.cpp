// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_table.h"
EntityTable *EntityTable::getTable(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return new ProcedureTable();
    case EntityType::kStatement: return new StatementTable();
    case EntityType::kVariable: return new VariableTable();
    case EntityType::kConstant: return new ConstantTable();
    default: return nullptr;
  }
}

void EntityTable::populate(Entity &entity) {
  this->table_.insert(entity);
}

std::unordered_set<Entity *> EntityTable::get() {
  std::unordered_set<Entity *> result = {};
  for (auto entity: this->table_) {
    result.insert(&entity);
  }
  return result;
}

int EntityTable::getCount() {
  return this->table_.size();
}
