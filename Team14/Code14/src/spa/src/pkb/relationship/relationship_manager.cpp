// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_manager.h"

#include <spdlog/spdlog.h>

RelationshipManager::RelationshipManager() {
  this->relationship_table_map_ = std::unordered_map<RsType, RelationshipTable *>();
  this->cache_ = new Cache<EntityRsInv, EntityPointerUnorderedSet, TripletHash>(100);
}
/**
 * Form the cache query
 * @param entity
 * @param rs_type
 * @param is_inverse
 * @return
 */
EntityRsInv RelationshipManager::GetCacheQuery(Entity *entity, RsType rs_type, bool is_inverse) {
  return EntityRsInv(entity, rs_type, is_inverse);
}
/**
 * Clears the cache
 */
void RelationshipManager::ClearCache() { this->cache_->Clear(); }
/**
 * Get the relationship table for the given relationship type
 * @param rs_type
 * @return
 */
RelationshipTable *RelationshipManager::GetTable(RsType rs_type) {
  // If table hasn't been created, create it first.
  if (this->relationship_table_map_.find(rs_type) == this->relationship_table_map_.end()) {
    spdlog::debug("{} table not found. Creating new...", RsTypeToString(rs_type));
    this->CreateTable(rs_type);
  }
  return this->relationship_table_map_[rs_type];
}
/**
 * Create a new relationship table for the given relationship type
 * @param rs_type
 */
void RelationshipManager::CreateTable(RsType rs_type) {
  RelationshipTable *table;
  switch (rs_type) {
    case RsType::kFollows: table = new FollowsTable(); break;
    case RsType::kParent: table = new ParentTable(); break;
    case RsType::kModifies: table = new ModifiesTable(); break;
    case RsType::kUses: table = new UsesTable(); break;
    case RsType::kCalls: table = new CallsTable(); break;
    case RsType::kNext: table = new NextTable(); break;
    default: throw PKBException(RsTypeToString(rs_type) + " table could not be created");
  }
  this->relationship_table_map_[rs_type] = table;
}

void RelationshipManager::Populate(std::vector<Relationship *> &relationships) {
  for (auto *relationship : relationships) {
    RsType rs_type = relationship->GetType();

    auto *relationship_table = GetTable(rs_type);
    relationship_table->populate(*relationship);
  }
}

EntityPointerUnorderedSet RelationshipManager::Get(RsType rs_type, Entity *entity, bool is_inverse) {
  auto matches = EntityPointerUnorderedSet();
  switch (rs_type) {
    case RsType::kFollowsT: {
      matches = this->GetAll(RsType::kFollows, entity, is_inverse);
      break;
    }
    case RsType::kParentT: {
      matches = this->GetAll(RsType::kParent, entity, is_inverse);
      break;
    }
    case RsType::kCalls: {
      matches = this->GetCalls(entity, is_inverse);
      break;
    }
    case RsType::kCallsT: {
      matches = this->GetAllCalls(entity, is_inverse);
      break;
    }
    case RsType::kNextT: {
      matches = this->GetAll(RsType::kNext, entity, is_inverse);
      break;
    }
    case RsType::kModifies:// fallthrough
    case RsType::kUses: {
      matches = this->GetInference(rs_type, entity, is_inverse);
      break;
    }
    default: {
      auto *relationship_table = GetTable(rs_type);
      matches = relationship_table->get(entity, is_inverse);
    }
  }
  return matches;
}

EntityPointerUnorderedSet RelationshipManager::GetAll(RsType rs_type, Entity *entity, bool is_inverse) {
  auto matches = EntityPointerUnorderedSet();
  auto *relationship_table = GetTable(rs_type);

  std::queue<Entity *> queue;
  queue.push(entity);
  while (!queue.empty()) {
    auto *current = queue.front();
    auto sub_matches = relationship_table->get(current, is_inverse);
    for (auto *sub_match : sub_matches) {
      if (matches.find(sub_match) == matches.end()) {
        matches.insert(sub_match);
        queue.push(sub_match);
      }
    }
    queue.pop();
  }
  return matches;
}

EntityPointerUnorderedSet RelationshipManager::Empty() { return EntityPointerUnorderedSet(); }

EntityPointerUnorderedSet RelationshipManager::GetInference(RsType rs_type, Entity *entity, bool is_inverse) {
  auto entity_type = entity->GetType();
  auto *relationship_table = GetTable(rs_type);

  switch (entity_type) {
    case EntityType::kCallStmt: return this->GetInferenceGivenProcedure(relationship_table, entity);
    case EntityType::kProcedure:// fallthrough
    case EntityType::kIfStmt:   // fallthrough
    case EntityType::kWhileStmt: return this->GetInferenceFromChildren(relationship_table, entity);
    case EntityType::kVariable: return this->GetInferenceGivenVariable(relationship_table, entity);
    default: return relationship_table->get(entity, is_inverse);
  }
}

Entity *RelationshipManager::GetProcedureEntity(Entity *entity, bool is_call_statement_entity) {
  Entity *result = new ProcedureEntity("");
  if (is_call_statement_entity) {
    auto *calls_table = GetTable(RsType::kCalls);
    auto call_entries = calls_table->get(entity, false);
    if (call_entries.empty()) {
      result = new ProcedureEntity("");
    } else {
      result = *(call_entries.begin());
    }
  } else {
    auto parent_entries = GetAll(RsType::kParent, entity, true);
    for (auto *parent : parent_entries) {
      if (parent->GetType() == EntityType::kProcedure) {
        result = parent;
        break;
      }
    }
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetInferenceGivenProcedure(RelationshipTable *relationship_table,
                                                                          Entity *entity) {
  // Get corresponding procedure entity
  auto *procedure_entity = this->GetProcedureEntity(entity, true);
  if (procedure_entity->GetValue().empty()) { return this->Empty(); }
  return this->GetInferenceFromChildren(relationship_table, procedure_entity);
}

EntityPointerUnorderedSet RelationshipManager::GetInferenceFromChildren(RelationshipTable *relationship_table,
                                                                        Entity *entity) {
  EntityPointerUnorderedSet result = this->Empty();

  // Check if current statement is in relationship table.
  auto current_statement_variables = relationship_table->get(entity, false);
  if (current_statement_variables != this->Empty()) {
    result.insert(current_statement_variables.begin(), current_statement_variables.end());
  }

  // Check if child statements are in relationship table.
  auto children_statements = GetAll(RsType::kParent, entity, false);
  for (auto *child_entity : children_statements) {
    if (child_entity->GetType() == EntityType::kCallStmt) {
      auto *procedure_entity = this->GetProcedureEntity(child_entity, true);
      if (!procedure_entity->GetValue().empty()) {
        auto child_result = GetInferenceGivenProcedure(relationship_table, child_entity);
        result.insert(procedure_entity);
        result.insert(child_result.begin(), child_result.end());
      }
    } else {
      auto variable_entity_set = relationship_table->get(child_entity, false);
      if (variable_entity_set != this->Empty()) {
        result.insert(variable_entity_set.begin(), variable_entity_set.end());
      }
    }
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetInferenceGivenVariable(RelationshipTable *relationship_table,
                                                                         Entity *entity) {
  EntityPointerUnorderedSet result = this->Empty();

  auto statements = relationship_table->get(entity, true);
  for (auto *statement_entity : statements) {
    auto *procedure_entity = this->GetProcedureEntity(statement_entity, false);
    if (!procedure_entity->GetValue().empty()) {
      auto preceding_procedure = this->GetAllCalls(procedure_entity, true);
      auto subsequent_procedure = this->GetAllCalls(procedure_entity, false);
      result.insert(preceding_procedure.begin(), preceding_procedure.end());
      result.insert(subsequent_procedure.begin(), subsequent_procedure.end());
      result.insert(procedure_entity);
    }
    result.insert(statement_entity);
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetCalls(Entity *entity, bool is_inverse) {
  EntityPointerUnorderedSet result = this->Empty();
  if (is_inverse) {
    auto *calls_table = GetTable(RsType::kCalls);
    auto call_entries = calls_table->get(entity, true);
    for (auto *stmt_entity : call_entries) {
      auto parent_statements = GetAll(RsType::kParent, stmt_entity, true);
      for (auto *parent_entity : parent_statements) {
        if (parent_entity->GetType() == EntityType::kProcedure) {
          result.insert(parent_entity);
          break;
        }
      }
    }
  } else {
    auto children_statements = GetAll(RsType::kParent, entity, is_inverse);
    for (auto *child_entity : children_statements) {
      if (child_entity->GetType() == EntityType::kCallStmt) {
        auto *procedure_entity = this->GetProcedureEntity(child_entity, true);
        if (!procedure_entity->GetValue().empty()) { result.insert(procedure_entity); }
      }
    }
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetAllCalls(Entity *entity, bool is_inverse) {
  EntityPointerUnorderedSet result = this->Empty();
  std::queue<Entity *> queue;
  queue.push(entity);
  while (!queue.empty()) {
    auto *current = queue.front();
    auto procedure_set = this->GetCalls(current, is_inverse);
    for (auto *procedure_entity : procedure_set) {
      queue.push(procedure_entity);
      result.insert(procedure_entity);
    }
    queue.pop();
  }
  return result;
}
