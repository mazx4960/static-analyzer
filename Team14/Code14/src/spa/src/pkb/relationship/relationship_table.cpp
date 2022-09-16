
#include "relationship_table.h"
RelationshipTable::RelationshipTable() {
  this->table_ = EntityPointerUnorderedMap();
  this->inverse_table_ = EntityPointerUnorderedMap();
}

EntityPointerUnorderedSet RelationshipTable::Empty() {
  return EntityPointerUnorderedSet();
}

EntityPointerUnorderedMap RelationshipTable::GetTable(bool inverse) {
  if (inverse) {
    return this->inverse_table_;
  }
  return this->table_;
}

EntityPointerUnorderedSet RelationshipTable::get(Entity *query_entity, bool is_inverse) {
  if (is_inverse) {
    if (this->inverse_table_.find(query_entity) == this->inverse_table_.end()) {
      return this->Empty();
    }
    return this->inverse_table_[query_entity];
  }
  if (this->table_.find(query_entity) == this->table_.end()) {
    return this->Empty();
  }
  return this->table_[query_entity];
}

void RelationshipTable::populate(Relationship &relationship) {
  Entity *first = relationship.GetFirst();
  Entity *second = relationship.GetSecond();
  this->table_[first].insert(second);
  this->inverse_table_[second].insert(first);
}
