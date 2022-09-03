// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once
#include <list>
#include <map>
#include <string>

#include "pkb/entity/entity_manager.h"

class PKBPopulator {
 private:
  EntityManager entityManager;

 public:
  std::map<std::string, std::list<int>>*
  populate(std::string entity, std::list<int> lines);
};
