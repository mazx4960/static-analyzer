
#include "relationship_table.h"

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::Empty() {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>{};
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> RelationshipTable::get(RsType type,
                                                                                               Entity *query_entity,
                                                                                               bool is_inverse) {
  switch (type) {
    case RsType::kFollowsT:
    case RsType::kParentT: return this->getTraversal(query_entity, is_inverse);
    default: return this->get(query_entity, is_inverse);
  }
}

std::unordered_set<Entity *,
                   EntityHashFunction,
                   EntityPointerEquality> RelationshipTable::getTraversal(Entity *query_entity,
                                                                          bool is_inverse) {
  std::unordered_map<Entity *,
                     std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                     EntityHashFunction, EntityPointerEquality> *table_ptr;
  table_ptr = is_inverse ? &this->inverse_table_ : &this->table_;
  if (table_ptr->find(query_entity) == table_ptr->end()) {
    return this->Empty();
  }
  return this->traversalHelper(query_entity, table_ptr);
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>
RelationshipTable::traversalHelper(Entity *query_entity,
                                   std::unordered_map<Entity *, std::unordered_set<Entity *,
                                                                                   EntityHashFunction,
                                                                                   EntityPointerEquality>,
                                                      EntityHashFunction,
                                                      EntityPointerEquality> *table_ptr) {
  if (table_ptr->find(query_entity) == table_ptr->end()) {
    return this->Empty();
  }
  auto next_statement = (*table_ptr)[query_entity];
  auto *next_entity = *next_statement.begin();
  auto recursive_result = this->traversalHelper(next_entity, table_ptr);
  next_statement.insert(recursive_result.begin(), recursive_result.end());
  return next_statement;
}

void RelationshipTable::populate(Relationship &relationship) {
  Entity *first = relationship.GetFirst();
  Entity *second = relationship.GetSecond();
  this->table_[first].insert(second);
  this->inverse_table_[second].insert(first);
}

std::unordered_map<Entity *,
                   std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                   EntityHashFunction,
                   EntityPointerEquality>
RelationshipTable::get_inverse() {
  return this->inverse_table_;
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
