// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include<unordered_set>
#include <list>
#include <map>
#include <string>

#include "commons/types.h"
#include "pkb/entity/entity_manager.h"

class PKBPopulator {
   private:
    EntityManager entityManager_;
  
   public:
    static std::unordered_set<std::string>* populate(const std::vector<Entity>& entities);
  };
