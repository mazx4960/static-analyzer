
#include "relationship_table.h"

#include <iostream>

RelationshipTable *RelationshipTable::getTable(RsType type) {
  switch (type) {
    case RsType::kUses: return new UsesTable();
    case RsType::kModifies: return new ModifiesTable();
    case RsType::kParent: return new ParentTable();
    case RsType::kFollows: return new FollowsTable();
    default: return nullptr;
  }
}

void RelationshipTable::populate(Relationship &relationship) {
  Entity* first = relationship.GetFirst();
  Entity* second = relationship.GetSecond();
  this->table_[*first].insert(*second);
}

std::unordered_set<Entity*> RelationshipTable::Empty() {
  return std::unordered_set<Entity*>{};
}

std::unordered_set<Entity *> RelationshipTable::get(RsType type, Entity *query_entity, bool is_inverse) {
  switch (type) {
    case RsType::kFollows: case RsType::kParent: return this->getStatements(query_entity, is_inverse);
    case RsType::kFollowsT: case RsType::kParentT: return this->getTraversal(query_entity, is_inverse);
    default: return this->get(query_entity, is_inverse);
  }
}

std::unordered_set<Entity *> RelationshipTable::getStatements(Entity *query_entity, bool is_inverse) {
  if (is_inverse) {
    //return this->Empty();
  }
  if (this->table_.find(*query_entity) == this->table_.end()) {
    std::cout << "not found" << std::endl;
    //return this->Empty();
  }
  return this->formatResults(this->table_[*query_entity]);
}

std::unordered_set<Entity *> RelationshipTable::getTraversal(Entity *query_entity, bool is_inverse) {
  if (is_inverse || this->table_.find(*query_entity) == this->table_.end()) {
    return this->Empty();
  }
  //return this->formatResults(traversalHelper(query_entity));
  return this->Empty();
}

std::unordered_set<Entity *> RelationshipTable::traversalHelper(Entity *query_entity) {
  if (this->table_.find(*query_entity) == this->table_.end()) {
    return std::unordered_set<Entity *> {};
  }
  auto next_statement = this->table_[*query_entity];
  auto next_entity = *next_statement.begin();
  auto recursive_result = this->traversalHelper(&next_entity);
  //next_statement.insert(recursive_result.begin(), recursive_result.end());
  //return next_statement;
  return this->Empty();
}

std::unordered_set<Entity *> RelationshipTable::get(Entity *query_entity, bool inversion) {
  //return this->Empty();
  return std::unordered_set<Entity *> {};
}

std::unordered_set<Entity *> RelationshipTable::formatResults(const std::unordered_set<Entity, EntityHashFunction>&  query_result) {
  std::unordered_set<Entity *> final_query_result;

  for (auto entity: query_result) {
    final_query_result.insert(&entity);
    std::cout << "Here have " << entity.GetValue() << std::endl;
  }
  auto *a = *(final_query_result.begin());
  std::cout << "Here don't have" << a->GetValue();
  return final_query_result;
}