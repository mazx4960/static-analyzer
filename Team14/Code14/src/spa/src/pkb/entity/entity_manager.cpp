// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_manager.h"

#include<vector>
#include <unordered_set>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"
#include "pkb_query.h"

std::unordered_set<std::string> *EntityManager::populate(const std::vector<Entity> &entities) { 
  std::unordered_set<std::string> *table_pointer;
  for (auto entity : entities) {
    EntityType entity_type = entity.GetType();
    std::string entity_name = entity.GetName();
    switch (entity_type) {
      case EntityType::kVariable:
        table_pointer = v_.populate(entity_name);
    } 
  }
  return table_pointer;
}

Result EntityManager::getResult(EntityType type, QuerySynonym synonym) {
  switch (type) {
    case EntityType::kVariable: return v_.getResult(synonym);
  }
  return Result::empty(synonym);
}
