// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

#include <utility>

bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  if (this->entity_type_ == EntityType::kWildcard || other.getEntityType() == EntityType::kWildcard) { return true; }
  return this->declaration_type_ == other.declaration_type_ && this->entity_type_ == other.entity_type_;
}
bool QueryDeclaration::operator==(const QueryDeclaration *other) const {
  if (this->entity_type_ == EntityType::kWildcard || other->getEntityType() == EntityType::kWildcard) { return true; }
  return this->declaration_type_ == other->declaration_type_ && this->entity_type_ == other->entity_type_;
}
DeclarationType QueryDeclaration::getDeclarationType() const {
  return this->declaration_type_;
}
EntityType QueryDeclaration::getEntityType() const {
  return this->entity_type_;
}
EntityPointerUnorderedSet QueryDeclaration::getContext() const {
  return this->context_;
}
void QueryDeclaration::setContext(EntityPointerUnorderedSet context) {
  this->context_ = std::move(context);
}
std::string QueryDeclaration::toString() const {
  return EntityTypeToString(this->entity_type_);
}
bool SynonymDeclaration::operator==(const QueryDeclaration &other) const {
  if (other.getDeclarationType() == this->getDeclarationType()) {
    const auto &synonym_other = static_cast<const SynonymDeclaration &>(other);
    return synonym_other.getEntityType() == this->getEntityType()
        && *synonym_other.getSynonym() == *this->getSynonym();
  }
  return false;
}
bool SynonymDeclaration::operator==(const QueryDeclaration *other) const {
  if (other->getDeclarationType() == this->getDeclarationType()) {
    const auto &synonym_other = static_cast<const SynonymDeclaration *>(other);
    return synonym_other->getEntityType() == this->getEntityType()
        && *synonym_other->getSynonym() == *this->getSynonym();
  }
  return false;
}
std::string SynonymDeclaration::toString() const {
  return this->query_synonym_->toString();
}
QuerySynonym *SynonymDeclaration::getSynonym() const {
  return this->query_synonym_;
}
bool StaticDeclaration::operator==(const QueryDeclaration &other) const {
  if (other.getDeclarationType() == this->getDeclarationType()) {
    const auto &static_other = static_cast<const StaticDeclaration &>(other);
    return static_other.getEntityType() == this->getEntityType()
        && static_other.toString() == this->toString();
  }
  return false;
}
bool StaticDeclaration::operator==(const QueryDeclaration *other) const {
  if (other->getDeclarationType() == this->getDeclarationType()) {
    const auto &static_other = static_cast<const StaticDeclaration *>(other);
    return static_other->getEntityType() == this->getEntityType()
        && static_other->toString() == this->toString();
  }
  return false;
}
std::string StaticDeclaration::toString() const {
  return this->value_;
}

std::unordered_set<EntityType> WildcardDeclaration::getWildcardType() {
  return this->wildcard_types_;
}

void WildcardDeclaration::setWildcardType(std::unordered_set<EntityType> types) {
  this->wildcard_types_ = std::move(types);
}
