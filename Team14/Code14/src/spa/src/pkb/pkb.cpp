// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

void PKB::save(std::vector<Entity *> &entities) {
  for (auto *entity : entities) {
    EntityType entity_type = entity->GetType();
    std::string entity_name = entity->GetName();

    // If table hasn't been created, create it first.
    if (this->entity_storage_map_.find(entity_type) == this->entity_storage_map_.end()) {
      this->entity_storage_map_[entity_type] = EntityStore::getStore(entity_type);
    }

    // Populate table here
    this->entity_storage_map_[entity_type]->save(*entity);
  }
}

Result PKB::get(PKBQuery &query) {
  return this->get(query.getEntityType(), query.getSynonym());
}

Result PKB::get(EntityType type, QuerySynonym synonym) {
  // Table not initialised, return empty Result
  if (this->entity_storage_map_.find(type) == this->entity_storage_map_.end()) {
    return Result::empty(synonym);
  }
  return this->entity_storage_map_[type]->get(synonym);
}

int PKB::getCount() {
  int count = 0;
  for (auto &entity_table : this->entity_storage_map_) {
    count += entity_table.second->getCount();
  }
  return count;
}
