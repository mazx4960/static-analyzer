// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"
#include "commons/relationship.h"
#include "pkb/entity/entity_manager.h"

Result PKB::get(PKBQuery &query) {
  QuerySynonym query_synonym = query.getSynonym();
  EntityManager manager;

  switch (query.getEntityType()) {
    case EntityType::kVariable:return manager.getResult(EntityType::kVariable, query_synonym);
    case EntityType::kProcedure: // fall-through;
    case EntityType::kStatement: // fall-through;
    case EntityType::kConstant: // fall-through;
    default:throw std::invalid_argument("Entity type not implemented yet!");
  }
}

bool PKB::save(Relationship &rs) {
  return true;
}