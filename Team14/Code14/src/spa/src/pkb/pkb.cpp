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

// Return a set of names of a specific entity
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

// Return a set of Entity given a relationship
Result PKB::getResults(PKBSuchThatQuery &query) {
  return this->getResults(*query.getRelationship());
}

Result PKB::getResults(std::vector<PKBSuchThatQuery> &query) {
  std::unordered_set<Entity*> set_entity = {};
  auto synonym = QuerySynonym("placeholder");

  for (auto itr = query.begin(); itr != query.end(); ++itr) {
    Result result = this->getResults(*itr);
    set_entity.insert(result.get_results_entity_set().begin(), result.get_results_entity_set().end());
  }
  return Result(synonym, set_entity);
}

Result PKB::getResults(Relationship relationship) {
  auto type = relationship.GetType();

  if (this->relationship_map_.find(type) == this->relationship_map_.end()) {
    auto synonym = QuerySynonym("placeholder");
    return Result::empty(synonym);
  }
  return this->relationship_map_[type]->get(relationship.GetFirst(), relationship.GetSecond());
}

int PKB::getCount() {
  int count = 0;
  for (auto &entity_table : this->entity_map_) {
    count += entity_table.second->getCount();
  }
  return count;
}
