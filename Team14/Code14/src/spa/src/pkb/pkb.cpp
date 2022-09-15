// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

PKB::PKB() { this->entity_manager_ = new EntityManager(); }

void PKB::populate(std::vector<Entity *> &entities) { this->entity_manager_->Populate(entities); }

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getEntities(EntityType entity_type) {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByRelationship(RsType rs_type,
                                                                                               Entity *entity,
                                                                                               bool is_inverse) {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByPattern(Entity *entity,
                                                                                          std::string &right_pattern) {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
}
