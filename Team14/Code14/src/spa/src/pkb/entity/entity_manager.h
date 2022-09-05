// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once
#include <vector>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/variable.h"

class EntityManager {
 private:
  Variable v_;

 public:
  std::vector<std::string>* populate(const std::vector<Entity>& entities);
};
