// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

PKB::PKB() {
  this->entity_manager_ = new EntityManager();
  this->pattern_manager_ = new PatternManager();
}

void PKB::populate(std::vector<Entity *> entities) { this->entity_manager_->Populate(entities); }
void PKB::populate(std::vector<Pattern *> patterns) { this->pattern_manager_->Populate(patterns); }

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::Empty() {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getEntities(EntityType entity_type) {
  return this->entity_manager_->Get(entity_type);


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

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByPattern(Entity *entity,
                                                                                          std::string &right_pattern) {
  return this->pattern_manager_->Get(entity, right_pattern);
}
