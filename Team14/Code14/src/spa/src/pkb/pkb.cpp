// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

PKB::PKB() {
  this->entity_manager_ = new EntityManager();
  this->pattern_manager_ = new PatternManager();
  this->relationship_manager_ = new RelationshipManager();
}

void PKB::populate(std::vector<Entity *> entities) { this->entity_manager_->Populate(entities); }
void PKB::populate(std::vector<Pattern *> patterns) { this->pattern_manager_->Populate(patterns); }
void PKB::populate(std::vector<Relationship *> relationships) { this->relationship_manager_->Populate(relationships); }

EntityPointerUnorderedSet PKB::getEntities(EntityType entity_type) {
  return this->entity_manager_->Get(entity_type);
}
EntityPointerUnorderedSet PKB::getByRelationship(RsType rs_type, Entity *entity, bool is_inverse) {
  return this->relationship_manager_->Get(rs_type, entity, is_inverse);
}

EntityPointerUnorderedSet PKB::getByPattern(Entity *entity, std::string &right_pattern, bool isSubmatch) {
  return this->pattern_manager_->Get(entity, right_pattern, isSubmatch);
}

EntityPointerUnorderedSet PKB::getEntitiesByString(std::string &entity_value) {
  return this->entity_manager_->Get(entity_value);
}