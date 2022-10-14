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
void RelationshipManager::ClearCache() {
  this->cache_->Clear();
}
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
    case RsType::kFollows: table = new FollowsTable();
      break;
    case RsType::kParent: table = new ParentTable();
      break;
    case RsType::kModifies: table = new ModifiesTable();
      break;
    case RsType::kUses: table = new UsesTable();
      break;
    case RsType::kCalls: table = new CallsTable();
      break;
    case RsType::kNext: table = new NextTable();
      break;
    default: throw PKBException(RsTypeToString(rs_type) + " table could not be created");
  }
  this->relationship_table_map_[rs_type] = table;
}

void RelationshipManager::Populate(std::vector<Relationship *> &relationships) {
  for (auto *relationship : relationships) {
    RsType rs_type = relationship->GetType();

    auto *relationship_table = this->GetTable(rs_type);
    relationship_table->populate(*relationship);
  }
}

EntityPointerUnorderedSet RelationshipManager::Get(RsType rs_type, Entity *entity, bool is_inverse) {
  auto matches = EntityPointerUnorderedSet();
  switch (rs_type) {
    case RsType::kAffects: {
      matches = this->GetAffects(entity, is_inverse);
      break;
    }
    case RsType::kCalls: {
      matches = this->GetCalls(entity, is_inverse);
      break;
    }
    case RsType::kAffectsT: {
      matches = this->GetAll(RsType::kAffects, entity, is_inverse);
      break;
    }
    case RsType::kCallsT: {
      matches = this->GetAll(RsType::kCalls, entity, is_inverse);
      break;
    }
    case RsType::kFollowsT: {
      matches = this->GetAll(RsType::kFollows, entity, is_inverse);
      break;
    }
    case RsType::kParentT: {
      matches = this->GetAll(RsType::kParent, entity, is_inverse);
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

EntityPointerUnorderedSet RelationshipManager::Empty() {
  return EntityPointerUnorderedSet();
}

EntityPointerUnorderedSet RelationshipManager::GetAll(RsType rs_type, Entity *entity, bool is_inverse) {
  EntityPointerUnorderedSet matches = this->Empty();
  EntityPointerUnorderedSet visited = this->Empty();

  std::queue<Entity *> queue;
  queue.push(entity);
  while (!queue.empty()) {
    auto *current = queue.front();
    auto sub_matches = this->GetSubMatches(rs_type, current, is_inverse);
    for (auto *sub_match : sub_matches) {
      if (visited.find(sub_match) == visited.end()) {
        matches.insert(sub_match);
        visited.insert(sub_match);
        queue.push(sub_match);
      }
    }
    queue.pop();
  }
  return matches;
}

EntityPointerUnorderedSet RelationshipManager::GetInference(RsType rs_type, Entity *entity, bool is_inverse) {
  auto entity_type = entity->GetType();
  auto *relationship_table = this->GetTable(rs_type);

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
    auto *calls_table = this->GetTable(RsType::kCalls);
    auto call_entries = calls_table->get(entity, false);
    if (call_entries.empty()) {
      result = new ProcedureEntity("");
    } else {
      result = *(call_entries.begin());
    }
  } else {
    auto parent_entries = this->GetAll(RsType::kParent, entity, true);
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
  if (procedure_entity->GetValue().empty()) {
    return this->Empty();
  }
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
  auto children_statements = this->GetAll(RsType::kParent, entity, false);
  for (auto *child_entity : children_statements) {
    if (child_entity->GetType() == EntityType::kCallStmt) {
      auto *procedure_entity = this->GetProcedureEntity(child_entity, true);
      if (!procedure_entity->GetValue().empty()) {
        auto child_result = this->GetInferenceGivenProcedure(relationship_table, child_entity);
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
      auto preceding_procedure = this->GetAll(RsType::kCalls, procedure_entity, true);
      auto subsequent_procedure = this->GetAll(RsType::kCalls, procedure_entity, false);
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
    auto *calls_table = this->GetTable(RsType::kCalls);
    auto call_entries = calls_table->get(entity, true);
    for (auto *stmt_entity : call_entries) {
      auto parent_statements = this->GetAll(RsType::kParent, stmt_entity, true);
      for (auto *parent_entity : parent_statements) {
        if (parent_entity->GetType() == EntityType::kProcedure) {
          result.insert(parent_entity);
          break;
        }
      }
    }
  } else {
    auto children_statements = this->GetAll(RsType::kParent, entity, is_inverse);
    for (auto *child_entity : children_statements) {
      if (child_entity->GetType() == EntityType::kCallStmt) {
        auto *procedure_entity = this->GetProcedureEntity(child_entity, true);
        if (!procedure_entity->GetValue().empty()) {
          result.insert(procedure_entity);
        }
      }
    }
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetAffects(Entity *query_entity, bool is_inverse) {
  RelationshipTable *table;
  if (query_entity->GetType() != EntityType::kAssignStmt) {
    return this->Empty();
  }
  if (is_inverse) {
    table = this->GetTable(RsType::kUses);
  } else {
    table = this->GetTable(RsType::kModifies);
  }
  auto next_statements = this->GetTable(RsType::kNext)->get(query_entity, is_inverse);
  auto query_statement_variables = table->get(query_entity, false);
  std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> stack;

  for (auto *entity : next_statements) {
    stack.push(std::pair(entity, query_statement_variables));
  }
  if (is_inverse) {
    return this->GetAffectsInverseHelper(&stack);
  }
  return this->GetAffectsHelper(&stack);
}

EntityPointerUnorderedSet RelationshipManager::GetAffectsHelper(
    std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> *stack) {
  EntityPointerUnorderedSet result = this->Empty();
  EntityPointerUnorderedSet visited = this->Empty();
  while (!stack->empty()) {
    auto current_pair = stack->top();
    auto *current_statement = current_pair.first;
    auto query_statement_variables = current_pair.second;
    visited.insert(current_statement);
    stack->pop();

    if (IsVariableUsed(&query_statement_variables, current_statement)) {
      result.insert(current_statement);
    }
    if (IsVariableModified(&query_statement_variables, current_statement)) {
      continue;
    }
    auto subsequent_statement = this->GetTable(RsType::kNext)->get(current_statement, false);
    for (auto *next_statement : subsequent_statement) {
      if (visited.find(next_statement) == visited.end()) {
        stack->push(std::pair(next_statement, query_statement_variables));
      }
    }
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetAffectsInverseHelper(
    std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> *stack) {
  EntityPointerUnorderedSet result = this->Empty();
  EntityPointerUnorderedSet visited = this->Empty();
  while (!stack->empty()) {
    auto current_pair = stack->top();
    auto *current_statement = current_pair.first;
    auto query_statement_variables = current_pair.second;
    visited.insert(current_statement);
    stack->pop();

    if (IsVariableModified(&query_statement_variables, current_statement)
        && current_statement->GetType() == EntityType::kAssignStmt) {
      auto *variable_entity = *(GetTable(RsType::kModifies)->get(current_statement, false).begin());
      query_statement_variables.erase(variable_entity);
      result.insert(current_statement);
    }
    if (query_statement_variables.empty()) {
      break;
    }
    auto subsequent_statement = this->GetTable(RsType::kNext)->get(current_statement, true);
    for (auto *next_statement : subsequent_statement) {
      if (visited.find(next_statement) == visited.end()) {
        stack->push(std::pair(next_statement, query_statement_variables));
      }
    }
  }
  return result;
}

bool RelationshipManager::IsVariableUsed(EntityPointerUnorderedSet *query_statement_variables, Entity *stmt_entity) {
  for (auto *variable_entity : *query_statement_variables) {
    auto uses_entry = this->GetTable(RsType::kUses)->get(stmt_entity, false);
    for (auto *entity : uses_entry) {
      if (stmt_entity->GetType() == EntityType::kAssignStmt && entity->GetValue() == variable_entity->GetValue()) {
        return true;
      }
    }
  }
  return false;
}

bool RelationshipManager::IsVariableModified(EntityPointerUnorderedSet *query_statement_variables,
                                             Entity *stmt_entity) {
  EntityPointerUnorderedSet modified_variables = this->Empty();
  if (stmt_entity->GetType() == EntityType::kCallStmt) {
    modified_variables = this->GetInference(RsType::kModifies, stmt_entity, false);
  } else {
    modified_variables = this->GetTable(RsType::kModifies)->get(stmt_entity, false);
  }
  if (modified_variables.empty()) {
    return false;
  }
  for (auto *query_entity : *query_statement_variables) {
    for (auto *modified_variable_entity : modified_variables) {
      if (query_entity->GetValue() == modified_variable_entity->GetValue()) {
        return true;
      }
    }
  }
  return false;
}

EntityPointerUnorderedSet RelationshipManager::GetSubMatches(RsType rs_type, Entity *query_entity, bool is_inverse) {
  switch (rs_type) {
    case RsType::kAffects: return this->GetAffects(query_entity, is_inverse);
    case RsType::kCalls: return this->GetCalls(query_entity, is_inverse);
    default: return this->GetTable(rs_type)->get(query_entity, is_inverse);
  }
}
