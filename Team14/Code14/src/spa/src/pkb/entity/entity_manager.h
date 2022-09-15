// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>

#include "commons/types.h"
#include "entity_table.h"

class EntityManager {
 private:
  std::unordered_map<EntityType, EntityTable *> entity_table_map_;
  void CreateTable(EntityType);

 public:
  EntityManager() = default;
  EntityTable *GetTable(EntityType entity_type);
  void Populate(const std::vector<Entity *> &entities);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> Get(EntityType entity_type);
};