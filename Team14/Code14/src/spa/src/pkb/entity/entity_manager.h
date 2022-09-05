// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once
#include<unordered_set>
#include <vector>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"
#include "pkb/entity/result.h"
#include "pkb_query.h"

class EntityManager {
 private:
  Variable v_;

 public:
  std::unordered_set<std::string> *populate(const std::vector<Entity> &entities);
  Result getResult(EntityType, QuerySynonym &);
};
