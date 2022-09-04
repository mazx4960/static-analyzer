// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_manager.h"

#include <list>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"

std::list<std::string>* EntityManager::populate(const std::list<Entity>& entities) { 
  std::list<std::string>* variable_table_pointer;
  for (auto entity : entities) {
    std::string variable_table = entity.GetName();
    variable_table_pointer = v_.populate(variable_table);
  }
  return variable_table_pointer;
}
