// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

PKB::PKB() {
  this->entity_manager_ = new EntityManager(); 
  this->relationship_manager_ = new RelationshipManager(); 
  this->pattern_manager_ = new PatternManager(); 
}

void PKB::populate(std::vector<Entity *> entities) { this->entity_manager_->Populate(entities); }
void PKB::populate(std::vector<Pattern *> patterns) { this->pattern_manager_->Populate(patterns); }

void PKB::populate(std::vector<Relationship *> &relationships) { this->relationship_manager_->Populate(relationships); }

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getEntities(EntityType entity_type) {
  return this->entity_manager_->Get(entity_type);
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByRelationship(RsType rs_type,
                                                                                               Entity *entity,
                                                                                               bool is_inverse) {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByPattern(Entity *entity,
                                                                                          std::string &right_pattern) {
  return this->pattern_manager_->Get(entity, right_pattern);
}

int PKB::getEntityCount() {
  int count = 0;
  for (auto &entity_table : this->entity_map_) { count += entity_table.second->getCount(); }

  return count;
}
int PKB::getRelationshipCount() {
  int count = 0;
  for (auto &relationship_table : this->relationship_map_) { count += relationship_table.second->getCount(); }
  return count;
}
