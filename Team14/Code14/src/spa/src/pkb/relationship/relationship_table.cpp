
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

std::unordered_set<Entity *> RelationshipTable::get(RsType rs_type, Entity *entity, bool inversion) {
  switch (rs_type) {
    case RsType::kFollows: case RsType::kParent: return getMatchingStatement(entity, inversion);
    case RsType::kFollowsT: case RsType::kParentT: return getTraversalStatement(entity, inversion);
    default: return getMatchingEntity(entity, inversion);
  }
}

std::unordered_set<Entity *> RelationshipTable::getMatchingEntity(Entity *entity, bool inversion) {
  if (inversion) {
    return getMatchingSecondEntity(entity);
  }
  return getMatchingFirstEntity(entity);
}

std::unordered_set<Entity *> RelationshipTable::getMatchingFirstEntity(Entity *entity) {
  std::unordered_set<Entity *> result = {};

  for (auto entity_pair: this->table_) {
    if (entity_pair.first->GetName() == entity->GetName()) {
      result.insert(entity_pair.second);
    }
  }
  return result;
}

std::unordered_set<Entity *> RelationshipTable::getMatchingSecondEntity(Entity *entity) {
  std::unordered_set<Entity *> result = {};

  for (auto entity_pair: this->table_) {
    if (entity_pair.second->GetName() == entity->GetName()) {
      result.insert(entity_pair.first);
    }
  }
  return result;
}

std::unordered_set<Entity *> RelationshipTable::getMatchingStatement(Entity *entity, bool inversion) {
  if (inversion) {
    return getMatchingSecondStatement(entity);
  }
  return getMatchingFirstStatement(entity);
}

std::unordered_set<Entity *> RelationshipTable::getMatchingFirstStatement(Entity *entity) {
  auto *query_statement_entity = static_cast<StatementEntity *>(entity);

  for (auto entity_pair: this->table_) {
    if (entity_pair.first->GetType() == EntityType::kStatement
        && entity_pair.second->GetType() == EntityType::kStatement) {
      auto *pair_statement_entity = static_cast<StatementEntity *>(entity_pair.first);

      if (pair_statement_entity->GetStmtNo() == query_statement_entity->GetStmtNo()) {
        return std::unordered_set<Entity *> {entity_pair.second};
      }
    }
  }
  return std::unordered_set<Entity *> {};
}

std::unordered_set<Entity *> RelationshipTable::getMatchingSecondStatement(Entity *entity) {
  auto *query_statement_entity = static_cast<StatementEntity *>(entity);

  for (auto entity_pair: this->table_) {
    if (entity_pair.second->GetType() == EntityType::kStatement
        && entity_pair.first->GetType() == EntityType::kStatement) {
      auto *pair_statement_entity = static_cast<StatementEntity *>(entity_pair.second);

      if (pair_statement_entity->GetStmtNo() == query_statement_entity->GetStmtNo()) {
        return std::unordered_set<Entity *> {entity_pair.first};
      }
    }
  }
  return std::unordered_set<Entity *> {};
}

// TODO: (leeenen)
std::unordered_set<Entity *> RelationshipTable::getTraversalStatement(Entity *entity, bool inversion) {
  return std::unordered_set<Entity *>{};
}