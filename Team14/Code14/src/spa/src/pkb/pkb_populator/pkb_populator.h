// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once
#include "pkb/entity/entity_manager.h"
#include <string>
#include <list>

class PKBPopulator { 
 private: 
  EntityManager entityManager;

 public:
  std::map<std::string, std::list<int>>* populate(std::string entity, std::list<int> lines);
};
