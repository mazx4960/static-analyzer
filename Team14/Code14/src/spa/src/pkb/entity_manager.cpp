// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_manager.h"

void EntityManager::populate(std::vector<Entity *> &entities) {
  for (auto *entity : entities) {
    EntityType entity_type = entity->GetType();
    std::string entity_name = entity->GetName();

    if (this->entity_table_map_.find(entity_type) == this->entity_table_map_.end()) {
      this->entity_table_map_[entity_type] = SimpleEntityTable::getTable(entity_type);
    }

    this->entity_table_map_[entity_type]->populate(entity_name);
  }
}

Result EntityManager::getResult(EntityType type, QuerySynonym synonym) {
  // Table not initialised, return empty Result
  if (this->entity_table_map_.find(type) == this->entity_table_map_.end()) {
    return Result::empty(synonym);
  }
  return this->entity_table_map_[type]->getResult(synonym);
}

int EntityManager::getCount() {
  int count = 0;
  for (auto &entity_table : this->entity_table_map_) {
    count += entity_table.second->getCount();
  }
  return count;
}
