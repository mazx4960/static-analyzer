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
std::string WildcardReference::getReferenceValue() const {
  return "";
}
bool WildcardReference::isStmtRef() const {
  return true;
}
bool WildcardReference::isEntRef() const {
  return true;
}
bool WildcardReference::isSyntacticallyCorrect() const {
  return true;
}
std::string WildcardReference::toString() const {
  return EntityTypeToString(this->getEntityType()) + ":_";
}

// IdentDeclaration
bool IdentReference::operator==(const QueryReference &other) const {
  return other.getRefType() == ReferenceType::kIdent && this->getReferenceValue() == other.getReferenceValue();
}
bool IdentReference::operator==(const QueryReference *other) const {
  return other->getRefType() == ReferenceType::kIdent && this->getReferenceValue() == other->getReferenceValue();
}
std::string IdentReference::getReferenceValue() const {
  return this->value_;
}
bool IdentReference::isStmtRef() const {
  return false;
}
bool IdentReference::isEntRef() const {
  return true;
}
bool IdentReference::isSyntacticallyCorrect() const {
  return true;
}
std::string IdentReference::toString() const {
  return "Ident:" + this->value_;
}

// IntegerDeclaration
bool IntegerReference::operator==(const QueryReference &other) const {
  return other.getRefType() == ReferenceType::kInteger && this->getReferenceValue() == other.getReferenceValue();
}
bool IntegerReference::operator==(const QueryReference *other) const {
  return other->getRefType() == ReferenceType::kInteger && this->getReferenceValue() == other->getReferenceValue();
}
std::string IntegerReference::getReferenceValue() const {
  return this->value_;
}
bool IntegerReference::isStmtRef() const {
  return true;
}
bool IntegerReference::isEntRef() const {
  return false;
}
bool IntegerReference::isSyntacticallyCorrect() const {
  return this->value_.length() <= 1 || value_[0] != '0';
}
std::string IntegerReference::toString() const {
  return "Integer" + this->value_;
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
std::string SynonymReference::getReferenceValue() const {
  return this->getSynonym()->toString();
}
bool SynonymReference::isStmtRef() const {
  return true;
}
bool SynonymReference::isEntRef() const {
  return true;
}
bool SynonymReference::isSyntacticallyCorrect() const {
  return true;
}
std::string SynonymReference::toString() const {
  return EntityTypeToString(this->getEntityType()) + ":" + this->getSynonym()->toString();
}


