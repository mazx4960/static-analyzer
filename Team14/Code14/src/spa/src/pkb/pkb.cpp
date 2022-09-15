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

void PKB::populate(std::vector<Relationship *> &relationships) {
  for (auto *relationship : relationships) {
    RsType rs_type = relationship->GetType();

    // If table hasn't been created, create it first.
    if (this->relationship_map_.find(rs_type) == this->relationship_map_.end()) {
      this->relationship_map_[rs_type] = RelationshipTable::getTable(rs_type);
    }

    // Populate table here
    this->relationship_map_[rs_type]->populate(*relationship);
  }
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::Empty() {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getEntities(EntityType entity_type) {
  if (this->entity_map_.find(entity_type) == this->entity_map_.end()) {
    return this->Empty();
  }
  return this->entity_map_[entity_type]->get();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByRelationship(RsType rs_type, Entity *entity, bool is_inverse) {
  RsType temp;
  switch (rs_type) {
    case RsType::kParentT: temp = RsType::kParent; break;
    case RsType::kFollowsT: temp = RsType::kFollows; break;
    default: temp = rs_type;
  }
  if (this->relationship_map_.find(temp) == this->relationship_map_.end()) {
    return this->Empty();
  }
  return this->relationship_map_[temp]->get(rs_type, entity, is_inverse);
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByPattern(std::string &, std::string &) {
  return this->Empty();
}

int PKB::getCount() {
  int count = 0;
  for (auto &entity_table : this->entity_map_) {
    count += entity_table.second->getCount();
  }
  return count;
}
