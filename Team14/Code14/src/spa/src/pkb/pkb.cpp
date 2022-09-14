// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

void PKB::populate(std::vector<Entity *> &entities) {
  for (auto *entity : entities) {
    EntityType entity_type = entity->GetType();

    // If table hasn't been created, create it first.
    if (this->entity_map_.find(entity_type) == this->entity_map_.end()) {
      this->entity_map_[entity_type] = EntityTable::getTable(entity_type);
    }

    // Populate table here
    this->entity_map_[entity_type]->populate(*entity);
  }
}

std::unordered_set<Entity *> PKB::Empty() {
  return std::unordered_set<Entity *>();
}

std::unordered_set<Entity *> PKB::getEntities(EntityType entity_type) {
  if (this->entity_map_.find(entity_type) == this->entity_map_.end()) {
    return this->Empty();
  }
  return this->entity_map_[entity_type]->get(entity_type);
}

std::unordered_set<Entity *> PKB::getByRelationship(RsType rs_type, Entity *entity, bool inversion) {
  if (this->relationship_map_.find(rs_type) == this->relationship_map_.end()) {
    return this->Empty();
  }
  return this->relationship_map_[rs_type]->get(rs_type, entity, inversion);
}

std::unordered_set<Entity *> PKB::getByPattern(std::string &, std::string &) {
  return this->Empty();
}

int PKB::getCount() {
  int count = 0;
  for (auto &entity_table : this->entity_map_) {
    count += entity_table.second->getCount();
  }
  return count;
}
