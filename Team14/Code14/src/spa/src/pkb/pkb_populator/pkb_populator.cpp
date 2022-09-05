// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb_populator.h"

EntityManager entity_manager;

void PKBPopulator::populate(const std::vector<Entity> &entities) {
  entity_manager.populate(entities);
}
