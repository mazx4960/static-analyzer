// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>

#include "commons/types.h"
#include "pkb/entity/entity_manager.h"

class PKBPopulator {
 public:
  static void populate(const std::vector<Entity> &entities);
};
