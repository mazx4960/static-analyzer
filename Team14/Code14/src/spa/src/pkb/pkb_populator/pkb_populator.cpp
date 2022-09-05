// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/pkb_populator/pkb_populator.h"

#include <unordered_set>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "commons/entity.h"
#include "commons/types.h"
#include "pkb/entity/entity_manager.h"

EntityManager entity_manager;

std::unordered_set<std::string>* PKBPopulator::populate(const std::vector<Entity>& entities) {
  return entity_manager.populate(entities);
}
