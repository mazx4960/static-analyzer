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
bool Entity::operator==(const Entity *other) const {
  return this->type_ == other->type_ && this->name_ == other->name_;
}
size_t Entity::GetHash() const {
  return std::hash<std::string>()(this->name_);
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
    case EntityType::kStatement: {
      auto *statement_entity = static_cast<StatementEntity *>(this);
      name_string = std::to_string(statement_entity->GetStmtNo());
      switch (statement_entity->GetType()) {
        case EntityType::kAssignStmt:type_string = "assign";
          break;
        case EntityType::kCallStmt:type_string = "call";
          break;
        case EntityType::kIfStmt:type_string = "if";
          break;
        case EntityType::kPrintStmt:type_string = "print";
          break;
        case EntityType::kReadStmt:type_string = "read";
          break;
        case EntityType::kWhileStmt:type_string = "while";
          break;
        default:type_string = "unknown statement";
          break;
      }
    }
    default: break; // string entity types should not reach this stage
  }
  return type_string + ": " + name_string;
}

StatementEntity::StatementEntity(int stmt_no)
    : Entity(EntityType::kStatement, "") {
  this->stmt_no_ = stmt_no;
}
int StatementEntity::GetStmtNo() const {
  return this->stmt_no_;
}
bool StatementEntity::operator==(const StatementEntity &other) const {
  return this->stmt_type_ == other.stmt_type_ && this->stmt_no_ == other.stmt_no_;
}
size_t StatementEntity::GetHash() const {
  return std::hash<int>()(this->stmt_no_);
}
