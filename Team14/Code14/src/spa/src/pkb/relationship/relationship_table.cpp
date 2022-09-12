
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

std::unordered_set<Entity *> RelationshipTable::get(RsType rs_type, Entity *first_entity, Entity *second_entity) {
  switch (rs_type) {
    case RsType::kFollows: case RsType::kParent: return getMatchingStatement(first_entity, second_entity);
    case RsType::kFollowsT: case RsType::kParentT: return getTraversalStatement(first_entity,second_entity);
    default: return getMatchingEntity(first_entity, second_entity);
  }
}

// TODO: (leeenen) waiting for wildcard
std::unordered_set<Entity *> RelationshipTable::getMatchingEntity(Entity *first_entity, Entity *second_entity) {
  std::unordered_set<Entity *> result = {};

  return result;
}

std::unordered_set<Entity *> RelationshipTable::getMatchingStatement(Entity *first_entity, Entity *second_entity) {
  std::unordered_set<Entity *> result = {};

  // Convert into StatementEntity
  auto *first_statement = static_cast<StatementEntity *>(first_entity);
  auto *second_statement = static_cast<StatementEntity *>(second_entity);

  // Loop through table
  for (auto entity_pair: this->table_) {
    auto *pair_first_statement = static_cast<StatementEntity *>(entity_pair.first);
    auto *pair_second_statement = static_cast<StatementEntity *>(entity_pair.second);

    if (pair_first_statement->GetStmtNo() == first_statement->GetStmtNo()
        && pair_second_statement->GetStmtNo() == second_statement->GetStmtNo()) {
      result.insert(first_entity);
      result.insert(second_entity);
    }
  }
  return result;
}

// TODO: (leeenen)
std::unordered_set<Entity *> RelationshipTable::getTraversalStatement(Entity *first_entity, Entity *second_entity) {
  return std::unordered_set<Entity *>{};
}