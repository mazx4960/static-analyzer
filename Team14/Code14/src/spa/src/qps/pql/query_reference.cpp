// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_reference.h"

#include <utility>


ReferenceType QueryReference::getRefType() const {
  return this->reference_type_;
}
EntityType QueryReference::getEntityType() const {
  return this->entity_type_;
}
void QueryReference::setEntityType(EntityType entity_type) {
  this->entity_type_ = entity_type;
}
EntityPointerUnorderedSet QueryReference::getContext() const {
  return this->context_;
}
void QueryReference::setContext(EntityPointerUnorderedSet context) {
  this->context_ = std::move(context);
}

// WildcardDeclaration
bool WildcardReference::operator==(const QueryReference &other) const {
  return other.getRefType() == ReferenceType::kWildcard;
}
bool WildcardReference::operator==(const QueryReference *other) const {
  return other->getRefType() == ReferenceType::kWildcard;
}
std::string WildcardReference::toString() const {
  return "_";
}
bool WildcardReference::isStmtRef() const {
  return true;
}
bool WildcardReference::isEntRef() const {
  return true;
}

// IdentDeclaration
bool IdentReference::operator==(const QueryReference &other) const {
  return other.getRefType() == ReferenceType::kIdent && this->toString() == other.toString();
}
bool IdentReference::operator==(const QueryReference *other) const {
  return other->getRefType() == ReferenceType::kIdent && this->toString() == other->toString();
}
std::string IdentReference::toString() const {
  return this->value_;
}
bool IdentReference::isStmtRef() const {
  return false;
}
bool IdentReference::isEntRef() const {
  return true;
}

// IntegerDeclaration
bool IntegerReference::operator==(const QueryReference &other) const {
  return other.getRefType() == ReferenceType::kInteger && this->toString() == other.toString();
}
bool IntegerReference::operator==(const QueryReference *other) const {
  return other->getRefType() == ReferenceType::kInteger && this->toString() == other->toString();
}
std::string IntegerReference::toString() const {
  return this->value_;
}
bool IntegerReference::isStmtRef() const {
  return true;
}
bool IntegerReference::isEntRef() const {
  return false;
}

// SynonymDeclaration
QuerySynonym *SynonymReference::getSynonym() const {
  return this->query_synonym_;
}
bool SynonymReference::operator==(const QueryReference &other) const {
  if (other.getRefType() == this->getRefType()) {
    const auto &synonym_other = static_cast<const SynonymReference &>(other);
    return synonym_other.getEntityType() == this->getEntityType()
        && *synonym_other.getSynonym() == *this->getSynonym();
  }
  return false;
}
bool SynonymReference::operator==(const QueryReference *other) const {
  if (other->getRefType() == this->getRefType()) {
    const auto &synonym_other = static_cast<const SynonymReference *>(other);
    return synonym_other->getEntityType() == this->getEntityType()
        && *synonym_other->getSynonym() == *this->getSynonym();
  }
  return false;
}
std::string SynonymReference::toString() const {
  return this->query_synonym_->toString();
}
bool SynonymReference::isStmtRef() const {
  return true;
}
bool SynonymReference::isEntRef() const {
  return true;
}


