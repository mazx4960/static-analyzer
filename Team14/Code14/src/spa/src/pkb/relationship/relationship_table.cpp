// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_table.h"

void RelationshipTable::populate(Relationship& relationship) {
  Entity* first = relationship.GetFirst();
  Entity* second = relationship.GetSecond();
  this->table_[first].insert(second);
  this->inverse_table_[second].insert(first);
}

int RelationshipTable::getCount() {
  int count = 0;
  for (auto& relationships : this->table_) { count += relationships.second.size(); }
  return count;
}
