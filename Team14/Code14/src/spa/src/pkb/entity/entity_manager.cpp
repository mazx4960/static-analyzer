// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_manager.h"

#include <list>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"

std::list<std::string>* EntityManager::populate(std::list<Entity> entities) { 
  std::list<std::string>* variableTablePointer;
  for (auto entity : entities) {
    std::string variableName = entity.GetName();
    variableTablePointer = v.populate(variableName);
  }
  return variableTablePointer;
}
