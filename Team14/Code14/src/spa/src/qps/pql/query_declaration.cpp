// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

QuerySynonym *QueryDeclaration::getSynonym() const { return this->query_synonym_; }
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  if (this->type_ == EntityType::kWildcard || other.getType() == EntityType::kWildcard) {
    return true;
  }
  return this->type_ == other.getType()
      && *(this->query_synonym_) == *(other.getSynonym())
      && this->string_ == other.getString()
      && this->number_ == other.getNumber();
}
bool QueryDeclaration::operator==(const QueryDeclaration *other) const {
  if (this->type_ == EntityType::kWildcard || other->getType() == EntityType::kWildcard) {
    return true;
  }
  return this->type_ == other->getType()
      && this->query_synonym_ == other->getSynonym()
      && this->string_ == other->getString()
      && this->number_ == other->getNumber();
}
EntityType QueryDeclaration::getType() const { return this->type_; }
std::string QueryDeclaration::getString() const { return this->string_; }
int QueryDeclaration::getNumber() const { return this->number_; }
std::unordered_set<Entity *,
                   EntityHashFunction,
                   EntityPointerEquality> QueryDeclaration::getContext() const {
  return this->context_;
}
void QueryDeclaration::setContext(std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> context) {
  this->context_ = std::move(context);
}

