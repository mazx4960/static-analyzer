
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

Result RelationshipTable::get(RsType type, StatementEntity *first, StatementEntity *second) {
  switch (type) {
    case RsType::kFollowsT: case RsType::kParentT: return this->getTraversalResult(first, second);
    default: return this->getResult(first, second);
  }
}

Result RelationshipTable::getTraversalResult(StatementEntity *first, StatementEntity *second) {
  std::unordered_set<Entity*> set_result = {};
  QuerySynonym synonym = QuerySynonym("placeholder");
  // TODO testing of logic
  for (auto pair: this->table_) {
    std::unordered_set<Entity*> temp = {};
    if (pair.first->GetStmtNo() == first->GetStmtNo()) {
      if (pair.second->GetStmtNo() == second->GetStmtNo()) {
        temp.insert(second);
        return Result(synonym, temp);
      }
      Result result = getTraversalResult(pair.second, second);
      if (!result.is_empty()) {
        temp.insert(result.get_results_entity_set().begin(), result.get_results_entity_set().end());
        return Result(synonym, temp);
      }
    }
  }
  return Result::empty(synonym);
}

Result RelationshipTable::getResult(StatementEntity *first, StatementEntity *second) {
  std::unordered_set<Entity*> set_result = {};
  QuerySynonym synonym = QuerySynonym("placeholder");

  for (auto pair: this->table_) {
    if (pair.first->GetName() == first->GetName() && pair.second->GetName() == second->GetName()) {
      set_result.insert(second);
    }
  }
  return Result(synonym, set_result);
}