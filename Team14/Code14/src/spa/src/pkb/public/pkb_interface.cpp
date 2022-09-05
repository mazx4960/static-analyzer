// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb_interface.h"
#include "commons/relationship.h"
#include "pkb/entity/variable.h"
#include "pkb/entity/entity_manager.h"

Result PKBInterface::get(PKBQuery &query) {
  QuerySynonym synonym_placeholder = QuerySynonym("placeholder");
  switch (query.getEntityType()) {
    case EntityType::kVariable:
      EntityManager manager;
      return manager.getResult(EntityType::kVariable, query.getSynonym());
  }
  return Result::empty(synonym_placeholder);
}

bool PKBInterface::save(Relationship &rs) {
  return true;
}