// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/pkb_populator/pkb_populator.h"

EntityManager entity_manager;

std::unordered_set<std::string>* PKBPopulator::populate(const std::vector<Entity>& entities) {
  return entity_manager.populate(entities);
}
