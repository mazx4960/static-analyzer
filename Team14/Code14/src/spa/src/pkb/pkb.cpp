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

Result PKB::getResults(PKBEntityQuery &query) {
  return this->getResults(query.getEntityType(), query.getSynonym());
}

Result PKB::getResults(EntityType type, QuerySynonym synonym) {
  // Table not initialised, return empty Result
  if (this->entity_map_.find(type) == this->entity_map_.end()) {
    return Result::empty(synonym);
  }
  return this->entity_map_[type]->get(synonym);
}

Result PKB::getResults(PKBSuchThatQuery &query) {
  return this->getResults(*query.getRelationship());
}

Result PKB::getResults(Relationship relationship) {
  auto type = relationship.GetType();

  if (this->relationship_map_.find(type) == this->relationship_map_.end()) {
    auto synonym = QuerySynonym("placeholder");
    return Result::empty(synonym);
  }
  return this->relationship_map_[type]->get(type, relationship.GetFirst(), relationship.GetSecond());
}

int PKB::getCount() {
  int count = 0;
  for (auto &entity_table : this->entity_map_) {
    count += entity_table.second->getCount();
  }
  return count;
}
