// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_table.h"

void RelationshipTable::populate(Relationship& relationship) {
  Entity* first = relationship.GetFirst();
  Entity* second = relationship.GetSecond();
  this->table_[first].insert(second);
  this->inverse_table_[second].insert(first);
}

std::unordered_map<Entity*, std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality>, EntityHashFunction,
                   EntityPointerEquality>
    RelationshipTable::get() {
  return this->table_;
}

std::unordered_map<Entity*, std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality>, EntityHashFunction,
                   EntityPointerEquality>
RelationshipTable::get_inverse() {
  return this->inverse_table_;
}