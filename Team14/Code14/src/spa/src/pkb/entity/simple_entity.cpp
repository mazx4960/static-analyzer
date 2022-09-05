// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/simple_entity.h"
SimpleEntityTable *SimpleEntityTable::getTable(EntityType type) {
  switch (type) {
    case EntityType::kVariable: return new VariableTable();
    case EntityType::kConstant: return new ConstantTable();
    case EntityType::kProcedure: // fall-through;
    case EntityType::kStatement: // fall-through;
    default:throw std::invalid_argument("Entity type not implemented yet!");
  }
}
void SimpleEntityTable::populate(std::string entity_name) {
  this->table_.insert(entity_name);
}

Result SimpleEntityTable::getResult(QuerySynonym &synonym) {
  return Result(synonym, this->table_);
}

int SimpleEntityTable::getCount() {
  return this->table_.size();
}
