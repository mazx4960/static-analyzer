// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

PKB::PKB() {
  this->entity_manager_ = new EntityManager();
  this->pattern_manager_ = new PatternManager();
  this->relationship_manager_ = new RelationshipManager();
}

void PKB::populate(std::vector<Entity *> entities) { this->entity_manager_->Populate(entities); }
void PKB::populate(std::vector<Pattern *> patterns) { this->pattern_manager_->Populate(patterns); }

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getEntities(EntityType entity_type) {
  return this->entity_manager_->Get(entity_type);
}
std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByRelationship(
    RsType rs_type, Entity * entity, bool is_inverse) {
  return this->relationship_manager_->Get(rs_type, entity, is_inverse);
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByPattern(
    Entity * entity, std::string & right_pattern) {
  return this->pattern_manager_->Get(entity, right_pattern);
}
