// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity.h"

#include "spdlog/spdlog.h"

Entity::Entity(EntityType type, std::string value) {
  this->type_ = type;
  this->value_ = std::move(value);
}
EntityType Entity::GetType() const { return this->type_; }
std::string Entity::GetValue() const { return this->value_; }
bool Entity::operator==(const Entity &other) const {
  return this->type_ == other.GetType() && this->value_ == other.GetValue();
}
size_t Entity::GetHash() const {
  size_t hash = 0;
  HashCombine<EntityType>::hash_combine(hash, this->GetType());
  HashCombine<std::string>::hash_combine(hash, this->GetValue());
  return hash;
}
std::string Entity::ToString() {
  std::string type_string;
  std::string name_string = this->value_;
  switch (this->type_) {
    case EntityType::kProcedure: type_string = "procedure"; break;
    case EntityType::kStatement: type_string = "statement"; break;
    case EntityType::kVariable: type_string = "variable"; break;
    case EntityType::kConstant: type_string = "constant"; break;
    case EntityType::kAssignStmt: type_string = "assign"; break;
    case EntityType::kCallStmt: type_string = "call"; break;
    case EntityType::kIfStmt: type_string = "if"; break;
    case EntityType::kPrintStmt: type_string = "print"; break;
    case EntityType::kReadStmt: type_string = "read"; break;
    case EntityType::kWhileStmt: type_string = "while"; break;
    default: type_string = "unknown"; break;
  }

  return type_string + ": " + name_string;
}
