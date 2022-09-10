
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

Result RelationshipTable::get(Entity *first, Entity *second) {
  auto synonym = QuerySynonym("placeholder");
  std::unordered_set<Entity*> set_result = {};

  for (auto pair: this->table_) {
    if (pair.first->GetName() == first->GetName() && pair.second->GetName() == second->GetName()) {
      set_result.insert(second);
    }
  }
  return Result(synonym, set_result);
}
