// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "pkb.h"
#include "commons/relationship.h"
#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"
#include "pkb/entity/simple_entity.h"

class PKB {
 private:
  std::unordered_map<EntityType, SimpleEntityTable *> entity_table_map_;

 public:
  PKB() = default;

  Result getResult(PKBQuery &);
  Result getResult(EntityType, QuerySynonym);

  void save(std::vector<Entity *> &entities);
  int getCount();
};
