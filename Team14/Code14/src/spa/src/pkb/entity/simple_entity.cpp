// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/simple_entity.h"
SimpleEntityTable *SimpleEntityTable::getTable(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return new ProcedureTable();
    case EntityType::kStatement: return new StatementTable();
    case EntityType::kVariable: return new VariableTable();
    case EntityType::kConstant: return new ConstantTable();
    default: throw std::invalid_argument("EntityType is invalid!");
  }
}
void SimpleEntityTable::populate(const std::string &entity_name) {
  this->table_.insert(entity_name);
}

Result SimpleEntityTable::getResult(QuerySynonym &synonym) {
  return Result(synonym, this->table_);
}

int SimpleEntityTable::getCount() {
  return this->table_.size();
}
