// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_table.h" 

RelationshipTable *RelationshipTable::getTable(RsType type) {
  switch (type) {
    case RsType::kFollows: return new FollowsTable();
    case RsType::kModifies: return new ModifiesTable();
    case RsType::kParent: return new ParentTable();
    case RsType::kUses: return new UsesTable();
    default: return nullptr;
  }
}

void RelationshipTable::populate(Relationship &relationship) { 
  Entity* first = relationship.GetFirst();
  Entity* second = relationship.GetSecond();
  this->table_[first].insert(second);
} 

int RelationshipTable::getCount() { 
  int count = 0;
  for (auto &entities : this->table_) { 
    count += entities.second.size(); 
  } 
  return count;
}
