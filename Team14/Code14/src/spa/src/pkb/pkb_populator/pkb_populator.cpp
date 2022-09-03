// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "pkb/pkb_populator/pkb_populator.h"

#include <list>
#include <map>
#include <string>

#include "pkb/entity/entity_manager.h"

EntityManager entityManager;

std::map<std::string, std::list<int>>*
PKBPopulator::populate(std::string entity, std::list<int> lines) {
	return entityManager.populate(entity, lines);
}
