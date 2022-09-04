// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "pkb/pkb_populator/pkb_populator.h"

#include <list>
#include <map>
#include <string>
#include <utility>

#include "commons/types.h"
#include "commons/entity.h"
#include "pkb/entity/entity_manager.h"

EntityManager entity_manager;

std::list<std::string>* PKBPopulator::populate(const std::list<Entity>& entities) {
  return entity_manager.populate(std::move(entities));
}
