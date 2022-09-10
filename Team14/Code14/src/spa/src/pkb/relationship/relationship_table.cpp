
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

Result RelationshipTable::get(RsType type, Entity *first, Entity *second) {
  auto synonym = QuerySynonym("placeholder");

  switch(type) {
    case RsType::kFollows: case RsType::kParent: return getStatements(synonym, first, second);
    default: return Result::empty(synonym);
  }
}

Result RelationshipTable::getStatements(QuerySynonym &syn, Entity *first, Entity *second) {
  std::unordered_map<QuerySynonym, std::unordered_set<Entity*>, QuerySynonymHashFunction> result_map_;
  for (auto pair: this->table_) {
    if (pair.first->GetName() == first->GetName() && pair.second->GetName() == second->GetName()) {
      result_map_[syn].insert(second);
    }
  }
  return Result(syn, result_map_);
}
