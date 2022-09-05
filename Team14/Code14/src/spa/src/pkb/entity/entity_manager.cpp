// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_manager.h"

void EntityManager::populate(const std::vector<Entity> &entities) {
  for (auto entity : entities) {
    EntityType entity_type = entity.GetType();
    std::string entity_name = entity.GetName();

    switch (entity_type) {
      case EntityType::kVariable: v_.populate(entity_name);
      case EntityType::kProcedure: // fall-through;
      case EntityType::kStatement: // fall-through;
      case EntityType::kConstant: // fall-through;
      default:continue;
    }
  }
}

Result EntityManager::getResult(EntityType type, QuerySynonym synonym) {
  switch (type) {
    case EntityType::kVariable: return v_.getResult(synonym);
    case EntityType::kProcedure: // fall-through;
    case EntityType::kStatement: // fall-through;
    case EntityType::kConstant: // fall-through;
    default:throw std::invalid_argument("Entity type not implemented yet!");
  }
}
