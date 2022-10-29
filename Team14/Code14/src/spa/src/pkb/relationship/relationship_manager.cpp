// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_manager.h"

#include <spdlog/spdlog.h>

#include <utility>

RelationshipManager::RelationshipManager() {
  this->relationship_table_map_ = std::unordered_map<RsType, RelationshipTable *>();
  this->cache_ = new Cache<EntityRsInv, EntityPointerUnorderedSet, TripletHash>(100);
}

RelationshipManager::~RelationshipManager() {
  this->ClearCache();
}

EntityPointerUnorderedSet RelationshipManager::Empty() {
  return EntityPointerUnorderedSet();
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

void RelationshipManager::AddResultToCache(EntityRsInv key, EntityPointerUnorderedSet *value) {
  this->cache_->Add(std::move(key), *value);
}

bool RelationshipManager::IsResultInCache(EntityRsInv key) {
  auto cache_query = this->cache_->Get(std::move(key));
  return cache_query.found;
}

EntityPointerUnorderedSet RelationshipManager::GetResultInCache(EntityRsInv key) {
  return this->cache_->Get(std::move(key)).value;
}

Cache<EntityRsInv, EntityPointerUnorderedSet, TripletHash> *RelationshipManager::GetCache() {
  return this->cache_;
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
    case RsType::kFollows:
      table = new FollowsTable();
      break;
    case RsType::kParent:
      table = new ParentTable();
      break;
    case RsType::kModifies:
      table = new ModifiesTable();
      break;
    case RsType::kUses:
      table = new UsesTable();
      break;
    case RsType::kCalls:
      table = new CallsTable();
      break;
    case RsType::kNext:
      table = new NextTable();
      break;
    default:
      throw PKBException(RsTypeToString(rs_type) + " table could not be created");
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
  this->num_queries_++;
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
    case RsType::kAffectsT:// fallthrough
    case RsType::kCallsT:  // fallthrough
    case RsType::kFollowsT:// fallthrough
    case RsType::kParentT: // fallthrough
    case RsType::kNextT: {
      matches = this->GetAll(rs_type, entity, is_inverse);
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

RsType RelationshipManager::GetRsTypeMapping(RsType rs_type) {
  switch (rs_type) {
    case RsType::kAffectsT:
      return RsType::kAffects;
    case RsType::kCallsT:
      return RsType::kCalls;
    case RsType::kFollowsT:
      return RsType::kFollows;
    case RsType::kParentT:
      return RsType::kParent;
    case RsType::kNextT:
      return RsType::kNext;
    default:
      return rs_type;
  }
}

EntityPointerUnorderedSet RelationshipManager::GetAll(RsType rs_type, Entity *entity, bool is_inverse) {
  auto cache_query = this->GetCacheQuery(entity, rs_type, is_inverse);
  bool is_result_found_in_cache = this->IsResultInCache(cache_query);

  if (is_result_found_in_cache) {
   return this->GetResultInCache(cache_query);
  }

  rs_type = this->GetRsTypeMapping(rs_type);

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
  this->AddResultToCache(cache_query, &matches);
  return matches;
}

EntityPointerUnorderedSet RelationshipManager::GetInference(RsType rs_type, Entity *entity, bool is_inverse) {
  auto cache_query = this->GetCacheQuery(entity, rs_type, is_inverse);
  bool is_result_found_in_cache = this->IsResultInCache(cache_query);

  if (is_result_found_in_cache) {
    return this->GetResultInCache(cache_query);
  }

  auto entity_type = entity->GetType();
  auto *relationship_table = this->GetTable(rs_type);
  auto results = this->Empty();

  switch (entity_type) {
    case EntityType::kCallStmt: {
      results = this->GetInferenceGivenCallStatement(rs_type, entity);
      break;
    }
    case EntityType::kProcedure:// fallthrough
    case EntityType::kIfStmt:   // fallthrough
    case EntityType::kWhileStmt: {
      results = this->GetInferenceFromChildren(rs_type, entity);
      break;
    }
    case EntityType::kVariable: {
      results = this->GetInferenceGivenVariable(rs_type, entity);
      break;
    }
    default:
      results = relationship_table->get(entity, is_inverse);
  }
  this->AddResultToCache(cache_query, &results);
  return results;
}

Entity *RelationshipManager::GetProcedureEntity(Entity *entity, bool is_call_statement_entity) {
  if (is_call_statement_entity) {
    auto *calls_table = this->GetTable(RsType::kCalls);
    auto call_entries = calls_table->get(entity, false);
    if (call_entries.empty()) {
      return new ProcedureEntity("");
    }
    return *(call_entries.begin());
  }
  while (entity->GetType() != EntityType::kProcedure) {
    auto *parent_entity = *(this->GetTable(RsType::kParent)->get(entity, true).begin());
    entity = parent_entity;
  }
  return entity;
}

EntityPointerUnorderedSet RelationshipManager::GetInferenceGivenCallStatement(RsType rs_type, Entity *entity) {
  // Get corresponding procedure entity
  auto *procedure_entity = this->GetProcedureEntity(entity, true);
  if (procedure_entity->GetValue().empty()) {
    return this->Empty();
  }
  return this->GetInferenceFromChildren(rs_type, procedure_entity);
}

EntityPointerUnorderedSet RelationshipManager::GetInferenceFromChildren(RsType rs_type, Entity *entity) {
  EntityPointerUnorderedSet result = this->Empty();

  // Check if current statement is in relationship table.
  auto *relationship_table = this->GetTable(rs_type);
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
        auto child_result = this->GetInferenceFromChildren(rs_type, procedure_entity);
        result.insert(procedure_entity);
        result.insert(child_result.begin(), child_result.end());
      }
    } else {
      auto variable_entity_set = relationship_table->get(child_entity, false);
      result.insert(variable_entity_set.begin(), variable_entity_set.end());
    }
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetInferenceGivenVariable(RsType rs_type, Entity *entity) {
  EntityPointerUnorderedSet result = this->Empty();
  auto *relationship_table = this->GetTable(rs_type);
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
  auto *calls_table = this->GetTable(RsType::kCalls);

  if (is_inverse) {
    auto call_entries = calls_table->get(entity, true);
    for (auto *call_statements : call_entries) {
      auto *parent_entity = this->GetProcedureEntity(call_statements, false);
      result.insert(parent_entity);
    }
  } else {
    for (auto [call_statements, procedure_entity_set] : calls_table->GetTable(false)) {
      if (this->GetProcedureEntity(call_statements, false)->GetValue() == entity->GetValue()) {
        result.insert(*(procedure_entity_set.begin()));
      }
    }
  }
  return result;
}

EntityPointerUnorderedSet RelationshipManager::GetAffects(Entity *query_entity, bool is_inverse) {
  auto cache_query = this->GetCacheQuery(query_entity, RsType::kAffects, is_inverse);
  bool is_result_found_in_cache = this->IsResultInCache(cache_query);

  if (is_result_found_in_cache) {
    return this->GetResultInCache(cache_query);
  }

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
  auto results = this->Empty();
  std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> stack;

  for (auto *entity : next_statements) {
    stack.push(std::pair(entity, query_statement_variables));
  }
  if (is_inverse) {
    results = this->GetAffectsInverseHelper(&stack);
  } else {
    results = this->GetAffectsHelper(&stack);
  }
  this->AddResultToCache(cache_query, &results);
  return results;
}

EntityPointerUnorderedSet RelationshipManager::GetAffectsHelper(std::stack<std::pair<Entity *,
                                                                                     EntityPointerUnorderedSet>> *stack) {
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

EntityPointerUnorderedSet RelationshipManager::GetAffectsInverseHelper(std::stack<std::pair<Entity *,
                                                                                            EntityPointerUnorderedSet>> *stack) {
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

bool RelationshipManager::IsVariableUsed(EntityPointerUnorderedSet *query_statement_variables,
                                         Entity *statement_entity) {
  for (auto *variable_entity : *query_statement_variables) {
    auto uses_entry = this->GetTable(RsType::kUses)->get(statement_entity, false);
    for (auto *entity : uses_entry) {
      if (statement_entity->GetType() == EntityType::kAssignStmt && entity->GetValue() == variable_entity->GetValue()) {
        return true;
      }
    }
  }
  return false;
}

bool RelationshipManager::IsVariableModified(EntityPointerUnorderedSet *query_statement_variables,
                                             Entity *statement_entity) {
  EntityPointerUnorderedSet modified_variables = this->Empty();
  if (statement_entity->GetType() == EntityType::kCallStmt) {
    auto *procedure_entity = this->GetProcedureEntity(statement_entity, true);
    modified_variables = this->GetInference(RsType::kModifies, procedure_entity, false);
  } else {
    modified_variables = this->GetTable(RsType::kModifies)->get(statement_entity, false);
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
    case RsType::kAffects:
      return this->GetAffects(query_entity, is_inverse);
    case RsType::kCalls:
      return this->GetCalls(query_entity, is_inverse);
    default:
      return this->GetTable(rs_type)->get(query_entity, is_inverse);
  }
}
void RelationshipManager::LogStatistics() {
  spdlog::info("Number of queries: {}", this->num_queries_);
  spdlog::info("Cache size: {}", this->cache_->Size());
  spdlog::info("Number of cache hits: {}", this->cache_->Hits());
  spdlog::info("Number of cache misses: {}", this->cache_->Misses());
}
