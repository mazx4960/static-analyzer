// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

#include "commons/relationship.h"
#include "entity_manager.h"

PKB::PKB() {
  this->entity_manager_ = new EntityManager();
}
Result PKB::get(PKBQuery &query) {
  QuerySynonym query_synonym = query.getSynonym();
  return this->entity_manager_->getResult(query.getEntityType(), query_synonym);
}
void PKB::save(std::vector<Entity *> &entities) {
  this->entity_manager_->populate(entities);
}
int PKB::getCount() {
  return this->entity_manager_->getCount();
}

