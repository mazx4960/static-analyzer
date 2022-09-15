// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

PKB::PKB() { this->entity_manager_ = new EntityManager(); }

void PKB::populate(std::vector<Entity *> &entities) { this->entity_manager_->Populate(entities); }

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
