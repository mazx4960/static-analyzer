// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"
#include "pkb/entity/result.h"
#include "pkb_query.h"

class EntityManager {
 private:
  Variable v_;

 public:
  void populate(const std::vector<Entity *> &entities);
  Result getResult(EntityType, QuerySynonym);
};
