// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity.h"

Entity::Entity(EntityType type, std::string name) {
  this->type = type;
  this->name = std::move(name);
}
EntityType Entity::GetType() {
  return this->type;
}
std::string Entity::GetName() {
  return this->name;
}
bool Entity::operator==(const Entity& other) const {
  return this->type == other.type && this->name == other.name;
}

StatementEntity::StatementEntity(StmtType stmt_type, int stmt_no)
    : Entity(EntityType::Statement, "") {
  this->stmt_type = stmt_type;
  this->stmt_no = stmt_no;
}
StmtType StatementEntity::GetStmtType() {
  return this->stmt_type;
}
int StatementEntity::GetStmtNo() const {
  return this->stmt_no;
}
bool StatementEntity::operator==(const StatementEntity& other) const {
  return this->stmt_type == other.stmt_type && this->stmt_no == other.stmt_no;
}
