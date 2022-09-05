// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/simple_entity.h"
#include "pkb/entity/result.h"
#include "pkb/entity/pkb_query.h"

class EntityManager {
 private:

 public:
  std::unordered_map<EntityType, SimpleEntityTable *> entity_table_map_;

  void populate(std::vector<Entity *> &entities);
  Result getResult(EntityType, QuerySynonym);
  int getCount();
};
