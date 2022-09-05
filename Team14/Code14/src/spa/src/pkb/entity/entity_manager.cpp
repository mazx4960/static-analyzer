// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_manager.h"

#include <vector>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"

std::vector<std::string>* EntityManager::populate(const std::vector<Entity>& entities) { 
  std::vector<std::string>* variable_table_pointer;
  for (auto entity : entities) {
    std::string variable_name = entity.GetName();
    variable_table_pointer = v_.populate(variable_name);
  }
  return variable_table_pointer;
}
