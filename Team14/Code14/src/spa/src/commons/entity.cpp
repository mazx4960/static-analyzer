// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity.h"

Entity::Entity(EntityType type, std::string name) {
  this->type_ = type;
  this->name_ = std::move(name);
}
EntityType Entity::GetType() const {
  return this->type_;
}
std::string Entity::GetName() const {
  return this->name_;
}
bool Entity::operator==(const Entity &other) const {
  return this->type_ == other.type_ && this->name_ == other.name_;
}
size_t Entity::GetHash() const {
  size_t hash = 0;
  HashCombine<EntityType>::hash_combine(hash, this->type_);
  HashCombine<std::string>::hash_combine(hash, this->name_);
  return hash;
}
std::string Entity::ToString() {
  std::string type_string;
  std::string name_string = this->name_;
  switch (this->type_) {
    case EntityType::kProcedure:type_string = "procedure";
      break;
    case EntityType::kVariable:type_string = "variable";
      break;
    case EntityType::kConstant:type_string = "constant";
      break;
    case EntityType::kStatement:auto *statement_entity = static_cast<StatementEntity *>(this);
      name_string = std::to_string(statement_entity->GetStmtNo());
      switch (statement_entity->GetStmtType()) {
        case StmtType::kAssign:type_string = "assign";
          break;
        case StmtType::kCall:type_string = "call";
          break;
        case StmtType::kIf:type_string = "if";
          break;
        case StmtType::kPrint:type_string = "print";
          break;
        case StmtType::kRead:type_string = "read";
          break;
        case StmtType::kWhile:type_string = "while";
          break;
        default:type_string = "unknown statement";
          break;
      }
  }
  return type_string + ": " + name_string;
}

StatementEntity::StatementEntity(StmtType stmt_type, int stmt_no)
    : Entity(EntityType::kStatement, "") {
  this->stmt_type_ = stmt_type;
  this->stmt_no_ = stmt_no;
}
StmtType StatementEntity::GetStmtType() const {
  return this->stmt_type_;
}
int StatementEntity::GetStmtNo() const {
  return this->stmt_no_;
}
bool StatementEntity::operator==(const StatementEntity &other) const {
  return this->stmt_type_ == other.stmt_type_ && this->stmt_no_ == other.stmt_no_;
}
size_t StatementEntity::GetHash() const {
  size_t hash = 0;
  HashCombine<StmtType>::hash_combine(hash, this->stmt_type_);
  HashCombine<int>::hash_combine(hash, this->stmt_no_);
  return hash;
}
