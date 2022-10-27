// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_manager.h"

#include <spdlog/spdlog.h>

#include "commons/types.h"
#include "pkb/exception.h"

EntityTable *EntityManager::GetTable(EntityType entity_type) {
  // If table hasn't been created, create it first.
  if (this->entity_table_map_.find(entity_type) == this->entity_table_map_.end()) {
    spdlog::debug("{} table not found. Creating new...", EntityTypeToString(entity_type));
    this->CreateTable(entity_type);
  }
  return this->entity_table_map_[entity_type];
}
void EntityManager::CreateTable(EntityType entity_type) {
  EntityTable *table;
  switch (entity_type) {
    case EntityType::kProcedure:
      table = new ProcedureTable();
      break;
    case EntityType::kVariable:
      table = new VariableTable();
      break;
    case EntityType::kConstant:
      table = new ConstantTable();
      break;
    case EntityType::kAssignStmt:
      table = new AssignTable();
      break;
    case EntityType::kCallStmt:
      table = new CallTable();
      break;
    case EntityType::kIfStmt:
      table = new IfTable();
      break;
    case EntityType::kPrintStmt:
      table = new PrintTable();
      break;
    case EntityType::kReadStmt:
      table = new ReadTable();
      break;
    case EntityType::kWhileStmt:
      table = new WhileTable();
      break;
    default:
      throw PKBException(EntityTypeToString(entity_type) + "table could not be created");
  }
  this->entity_table_map_[entity_type] = table;
}

void EntityManager::Populate(const std::vector<Entity *> &entities) {
  for (auto *entity : entities) {
    EntityType entity_type = entity->GetType();

    auto *entity_table = GetTable(entity_type);
    entity_table->populate(*entity);
  }
}
EntityPointerUnorderedSet EntityManager::Get(EntityType entity_type) {
  this->num_queries_++;
  spdlog::debug("Retrieving all {}s", EntityTypeToString(entity_type));
  EntityPointerUnorderedSet matches;
  if (entity_type == EntityType::kStatement) {
    for (auto stmt_type : GetAllStmtTypes()) {
      auto *entity_table = GetTable(stmt_type);
      auto stmts = entity_table->get();
      matches.insert(stmts.begin(), stmts.end());
    }
  } else {
    auto *entity_table = GetTable(entity_type);
    matches = entity_table->get();
  }
  std::string result_string;
  for (auto *match : matches) {
    result_string += match->GetValue() + ", ";
  }
  spdlog::debug("Results[{}]: {}", matches.size(), result_string);
  return matches;
}

EntityPointerUnorderedSet EntityManager::Get(std::string &entity_value) {
  this->num_queries_++;
  spdlog::debug("Retrieving all entities with {}", entity_value);
  EntityPointerUnorderedSet result = {};
  for (auto [k, v] : this->entity_table_map_) {
    for (auto *entity : v->get()) {
      if (entity->GetValue() == entity_value) {
        result.insert(entity);
      }
    }
  }
  return result;
}
void EntityManager::LogStatistics() {
  spdlog::info("Number of queries: {}", this->num_queries_);
  for (auto [k, v] : this->entity_table_map_) {
    spdlog::info("{}: {}", EntityTypeToString(k), v->size());
  }
}
