
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

void RelationshipTable::populate(Relationship &relationship) {
  Entity* first = relationship.GetFirst();
  Entity* second = relationship.GetSecond();
  this->table_[first].insert(second);
}

std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality> RelationshipTable::Empty() {
  return std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality>{};
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::get(RsType type,
                                                                                               Entity *query_entity,
                                                                                               bool is_inverse) {
  switch (type) {
    case RsType::kFollows: case RsType::kParent: return this->getStatements(query_entity, is_inverse);
    case RsType::kFollowsT: case RsType::kParentT: return this->getTraversal(query_entity, is_inverse);
    default: return this->get(query_entity, is_inverse);
  }
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::getStatements(Entity *query_entity,
                                                                                                         bool is_inverse) {
  if (is_inverse || this->table_.find(query_entity) == this->table_.end()) {
    return this->Empty();
  }
  return this->table_[query_entity];
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::getTraversal(Entity *query_entity,
                                                                                                        bool is_inverse) {
  if (is_inverse || this->table_.find(query_entity) == this->table_.end()) {
    return this->Empty();
  }
  auto result = traversalHelper(query_entity);
  return result;
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::traversalHelper(Entity *query_entity) {
  if (this->table_.find(query_entity) == this->table_.end()) {
    return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> {};
  }
  auto next_statement = this->table_[query_entity];
  auto *next_entity = *next_statement.begin();
  auto recursive_result = this->traversalHelper(next_entity);
  next_statement.insert(recursive_result.begin(), recursive_result.end());
  return next_statement;
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::get(Entity *query_entity,
                                                                                               bool is_inverse) {
  if (is_inverse) {
    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> results = {};
    for (auto [k, v]: this->table_) {
      for (auto *entity: v) {
        if (entity->GetValue() == query_entity->GetValue()) {
          results.insert(k);
          break;
        }
      }
    }
    return results;
  }
  return this->table_[query_entity];
}
