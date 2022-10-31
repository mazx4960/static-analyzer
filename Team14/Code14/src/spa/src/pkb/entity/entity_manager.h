// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>

#include "commons/types.h"
#include "entity_table.h"

class EntityManager {
 private:
  int num_queries_ = 0;
  std::unordered_map<EntityType, EntityTable *> entity_table_map_;
  static std::vector<EntityType> GetAllEntityTypes();
  void CreateTable(EntityType);

 public:
  EntityManager() = default;
  EntityTable *GetTable(EntityType entity_type);
  void Populate(const std::vector<Entity *> &entities);
  EntityPointerUnorderedSet Get(EntityType entity_type);
  EntityPointerUnorderedSet Get(std::string &entity_value);
  std::unordered_map<EntityType, int> Get();

  void LogStatistics();
};