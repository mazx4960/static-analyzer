// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/entity_manager.h" 
#include "pkb/entity/variable.h"
  
std::map<std::string, std::list<int>>* EntityManager::populate(std::string variable, std::list<int> lines) {
  //switch(entityType) {
  //	case 'Variable':
  //		v.populate(entity, lines);
  //}
  return v.populate(variable, lines);
}
