// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once
#include <list>
#include <map>
#include <string>

#include "commons/types.h"
#include "pkb/entity/entity_manager.h"

class PKBPopulator {
   private:
    EntityManager entityManager_;
  
   public:
    std::list<std::string>* populate(std::list<Entity> entities);
  };
