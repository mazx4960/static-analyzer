// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_table.h"

EntityTable *EntityTable::getTable(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return new ProcedureTable();
    case EntityType::kVariable: return new VariableTable();
    case EntityType::kConstant: return new ConstantTable();
    case EntityType::kAssign: return new AssignTable();
    case EntityType::kCall: return new CallTable();
    case EntityType::kIf: return new IfTable();
    case EntityType::kPrint: return new PrintTable();
    case EntityType::kRead: return new ReadTable();
    case EntityType::kWhile: return new WhileTable();
    default: return nullptr;
  }
}

void EntityTable::populate(Entity &entity) { 
  this->table_.insert(&entity); 
}
 
std::unordered_set<Entity *> EntityTable::get() { 
  return std::unordered_set<Entity *>();
} 

int EntityTable::getCount() { 
  return this->table_.size(); 
}
