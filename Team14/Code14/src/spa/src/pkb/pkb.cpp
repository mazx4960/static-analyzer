// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

#include "commons/relationship.h"
#include "entity_manager.h"

PKB::PKB() {
  this->entity_manager_ = new EntityManager();
}
Result PKB::get(PKBQuery &query) {
  QuerySynonym query_synonym = query.getSynonym();
  switch (query.getEntityType()) {
    case EntityType::kVariable:return this->entity_manager_->getResult(EntityType::kVariable, query_synonym);
    case EntityType::kProcedure: // fall-through;
    case EntityType::kStatement: // fall-through;
    case EntityType::kConstant: // fall-through;
    default:throw std::invalid_argument("Entity type not implemented yet!");
  }
}
void PKB::save(std::vector<Entity *> &entities) {
  this->entity_manager_->populate(entities);
}

