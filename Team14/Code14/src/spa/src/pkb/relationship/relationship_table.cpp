
#include "relationship_table.h"
RelationshipTable::RelationshipTable() {
  this->table_ = std::unordered_map<Entity *,
                                    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                                    EntityHashFunction, EntityPointerEquality>();
  this->inverse_table_ = std::unordered_map<Entity *,
                                            std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                                            EntityHashFunction, EntityPointerEquality>();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::Empty() {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>{};
}

std::unordered_map<Entity *,
                   std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                   EntityHashFunction, EntityPointerEquality> RelationshipTable::GetTable(bool inverse) {
  if (inverse) {
    return this->inverse_table_;
  }
  return this->table_;
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::get(RsType type,
                                                                                               Entity *query_entity,
                                                                                               bool is_inverse) {
  return this->get(query_entity, is_inverse);
}



void RelationshipTable::populate(Relationship &relationship) {
  Entity *first = relationship.GetFirst();
  Entity *second = relationship.GetSecond();
  this->table_[first].insert(second);
  this->inverse_table_[second].insert(first);
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::get(Entity *query_entity,
                                                                                               bool is_inverse) {
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
