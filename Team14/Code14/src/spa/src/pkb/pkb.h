// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_set>

#include "pkb.h"
#include "entity_manager.h"
#include "commons/relationship.h"
#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"

class PKB {
 private:
  EntityManager *entity_manager_;

 public:
  PKB();
  Result get(PKBQuery &);
  void save(std::vector<Entity *> &entities);
};
