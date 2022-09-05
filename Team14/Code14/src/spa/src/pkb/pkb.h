// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_set>

#include "commons/relationship.h"
#include "pkb.h"
#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"
#include "pkb/pkb_populator/pkb_populator.h"

class PKB {
 private:
  EntityManager *entity_manager_;
 public:
  PKB();
  static Result get(PKBQuery &);
  void Save(std::vector<Entity *> &entities);
};
