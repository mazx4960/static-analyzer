
#include "relationship_table.h"

RelationshipTable *RelationshipTable::getTable(RsType type) {
  switch (type) {
    case RsType::kUses: return new UsesTable();
    case RsType::kModifies: return new ModifiesTable();
    case RsType::kParent: return new ParentTable();
    case RsType::kFollows: return new FollowsTable();
    default: return nullptr;
  }
}

std::unordered_set<Entity*> RelationshipTable::Empty() {
  return std::unordered_set<Entity*>{};
}

std::unordered_set<Entity *> RelationshipTable::get(RsType type, Entity *query_entity, bool is_inverse) {
  switch (type) {
    case RsType::kFollows: case RsType::kParent: return this->getStatements(query_entity, is_inverse);
    case RsType::kFollowsT: case RsType::kParentT: return this->getTraversal(query_entity, is_inverse);
    default: return this->get(query_entity, is_inverse);
  }
}

std::unordered_set<Entity *> RelationshipTable::getStatements(Entity *query_entity, bool is_inverse) {
  if (is_inverse) {
    return this->Empty();
  }
  if (this->table_.find(*query_entity) == this->table_.end()) {
    return this->Empty();
  }
  return this->formatResults(this->table_[*query_entity]);
}

std::unordered_set<Entity *> RelationshipTable::getTraversal(Entity *query_entity, bool is_inverse) {
  if (is_inverse || this->table_.find(*query_entity) == this->table_.end()) {
    return this->Empty();
  }
  return this->formatResults(traversalHelper(query_entity));
}

std::unordered_set<Entity, EntityHashFunction> RelationshipTable::traversalHelper(Entity *query_entity) {
  if (this->table_.find(*query_entity) == this->table_.end()) {
    return std::unordered_set<Entity, EntityHashFunction> {};
  }
  auto next_statement = this->table_[*query_entity];
  auto next_entity = *next_statement.begin();
  auto recursive_result = this->traversalHelper(&next_entity);
  next_statement.insert(recursive_result.begin(), recursive_result.end());
  return next_statement;
}

std::unordered_set<Entity *> RelationshipTable::get(Entity *query_entity, bool inversion) {
  if (!inversion) {
    if (this->table_.find(*query_entity) == this->table_.end()) {
      return this->Empty();
    }
    return this->formatResults(this->table_[*query_entity]);
  }
  std::unordered_set<Entity, EntityHashFunction> results = {};

  for (auto [key, value]: this->table_) {
    for (const auto& entity: value) {
      if (entity.GetName() == query_entity->GetName()) {
        results.insert(key);
        break;
      }
    }
  }
  return formatResults(results);
}

std::unordered_set<Entity *> RelationshipTable::formatResults(const std::unordered_set<Entity, EntityHashFunction>& query_result) {
  std::unordered_set<Entity *> final_query_result = {};
  for (auto entity: query_result) {
    final_query_result.insert(&entity);
  }
  return final_query_result;
}