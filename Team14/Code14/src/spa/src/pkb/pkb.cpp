// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"

PKB::PKB() { this->entity_manager_ = new EntityManager(); }

void PKB::populate(std::vector<Entity *> &entities) { this->entity_manager_->Populate(entities); }

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getEntities(EntityType entity_type) {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByRelationship(RsType rs_type,
                                                                                               Entity *entity,
                                                                                               bool is_inverse) {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
void PKB::populate(std::vector<Relationship *> &relationships) {
  for (auto *relationship : relationships) {
    RsType rs_type = relationship->GetType();

    // If table hasn't been created, create it first.
    if (this->relationship_map_.find(rs_type) == this->relationship_map_.end()) {
      this->relationship_map_[rs_type] = RelationshipTable::getTable(rs_type);
    }

    // Populate table here
    this->relationship_map_[rs_type]->populate(*relationship);
  }
}

std::unordered_set<Entity *> PKB::getEntities(EntityType  /*entity_type*/) {
  return std::unordered_set<Entity *>();
}

std::unordered_set<Entity *> PKB::getByRelationship(RsType rs_type, Entity *entity, bool is_inverse) {
  return std::unordered_set<Entity *>();
}

std::unordered_set<Entity *> PKB::getByPattern(std::string &left_pattern, std::string &right_pattern) {
  return std::unordered_set<Entity *>();
}

std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> PKB::getByPattern(std::string &left_pattern,
                                                                                          std::string &right_pattern) {
  return std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>();
}

int PKB::getRelationshipCount() { 
  int count = 0;
  for (auto &relationship_table : this->relationship_map_) {   
    count += relationship_table.second->getCount();
  }
  return count;
}
