// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_manager.h"

#include<vector>
#include <unordered_set>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"

std::unordered_set<std::string>* 
EntityManager::populate(const std::vector<Entity>& entities) { 
  std::unordered_set<std::string>* table_pointer;
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
