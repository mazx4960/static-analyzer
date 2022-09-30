// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_manager.h"

#include <spdlog/spdlog.h>

RelationshipTable *RelationshipManager::GetTable(RsType rs_type) {
  // If table hasn't been created, create it first.
  if (this->relationship_table_map_.find(rs_type) == this->relationship_table_map_.end()) {
    spdlog::debug("{} table not found. Creating new...", RsTypeToString(rs_type));
    this->CreateTable(rs_type);
  }
  return this->relationship_table_map_[rs_type];
}

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

    auto *relationship_table = GetTable(rs_type);
    relationship_table->populate(*relationship);
  }
}

EntityPointerUnorderedSet RelationshipManager::Get(RsType rs_type, Entity *entity, bool is_inverse) {
  auto matches = EntityPointerUnorderedSet();
  switch (rs_type) {
    case RsType::kFollowsAll: {
      matches = this->GetAll(RsType::kFollows, entity, is_inverse);
      break;
    }
    case RsType::kParentAll: {
      matches = this->GetAll(RsType::kParent, entity, is_inverse);
      break;
    }
    case RsType::kCallsAll: {
      matches = this->GetAll(RsType::kCalls, entity, is_inverse);
      break;
    }
    case RsType::kNextAll: {
      matches = this->GetAll(RsType::kNext, entity, is_inverse);
      break;
    }
    case RsType::kModifies: case RsType::kUses: {
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

EntityPointerUnorderedSet RelationshipManager::Empty() {
  return EntityPointerUnorderedSet();
}

EntityPointerUnorderedSet RelationshipManager::GetInference(RsType rs_type, Entity *entity, bool is_inverse) {
  auto entity_type = entity->GetType();
  auto *relationship_table = GetTable(rs_type);

  switch(entity_type) {
    case EntityType::kCallStmt:
      return this->getInferenceFromProcedure(relationship_table, entity);
    case EntityType::kProcedure: // fallthrough
    case EntityType::kIfStmt:    // fallthrough
    case EntityType::kWhileStmt:
      return this->getInferenceFromChildren(relationship_table, entity);
    default: return relationship_table->get(entity, is_inverse);
  }
}

EntityPointerUnorderedSet RelationshipManager::getInferenceFromProcedure(RelationshipTable *relationship_table, Entity *entity) {
  // Get corresponding procedure entity
  auto *calls_table = GetTable(RsType::kCalls);
  auto call_entries = calls_table->get(entity, false);
  if (call_entries.empty()) {
    return this->Empty();
  }
  auto *procedure_entity = *(call_entries.begin());
  return this->getInferenceFromChildren(relationship_table, procedure_entity);
}

EntityPointerUnorderedSet RelationshipManager::getInferenceFromChildren(RelationshipTable *relationship_table, Entity *entity) {
  EntityPointerUnorderedSet result;

  // Check if current statement is in relationship table.
  auto current_statement_variables = relationship_table->get(entity, false);
  if (current_statement_variables != this->Empty()) {
    result.insert(current_statement_variables.begin(), current_statement_variables.end());
  }

  // Check if child statements are in relationship table.
  auto children_statements = GetAll(RsType::kParent, entity, false);
  for (auto *child_entity: children_statements) {
    auto variable_entity_set = relationship_table->get(child_entity, false);
    if (variable_entity_set != this->Empty()) {
      result.insert(variable_entity_set.begin(), variable_entity_set.end());
    }
  }
  return result;
}
