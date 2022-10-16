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
bool QueryReference::isStmtRef() const {
  return false;
}
bool QueryReference::isEntRef() const {
  return false;
}
bool QueryReference::isAttrCompareRef() const {
  return false;
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
bool IdentReference::isEntRef() const {
  return true;
}
bool IdentReference::isSyntacticallyCorrect() const {
  return true;
}
std::string IdentReference::toString() const {
  return "Ident:" + this->value_;
}
bool IdentReference::isAttrCompareRef() const {
  return true;
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
bool IntegerReference::isSyntacticallyCorrect() const {
  return this->value_.length() <= 1 || value_[0] != '0';
}
std::string IntegerReference::toString() const {
  return "Integer:" + this->value_;
}
bool IntegerReference::isAttrCompareRef() const {
  return true;
}

// SynonymDeclaration
QuerySynonym *SynonymReference::getSynonym() const {
  return this->query_synonym_;
}

ReferenceType SynonymReference::getRefType() const {
  return this->getRefType();
}

bool SynonymReference::operator==(const QueryReference &other) const {
  if (other.getRefType() == this->getRefType()) {
    const auto &synonym_other = static_cast<const SynonymReference &>(other);
    return synonym_other.getEntityType() == this->getEntityType()
        && (*synonym_other.getSynonym()) == (*this->getSynonym());
  }
  return false;
}
bool SynonymReference::operator==(const QueryReference *other) const {
  if (other->getRefType() == this->getRefType()) {
    const auto &synonym_other = static_cast<const SynonymReference *>(other);
    return synonym_other->getEntityType() == this->getEntityType()
        && (*synonym_other->getSynonym()) == (*this->getSynonym());
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
void SynonymReference::setBooleanRef(bool is_boolean_Ref) {
  this->is_boolean_ref_ = is_boolean_Ref;
}
bool SynonymReference::isBooleanRef() const {
  return this->is_boolean_ref_;
}
EntityPointerUnorderedSet SynonymReference::getContext() const {
  return QueryReference::getContext();
}

bool AttrReference::operator==(const QueryReference &other) const {
  if (other.getRefType() == this->getRefType()) {
    const auto &elem_other = static_cast<const AttrReference &>(other);
    return (*elem_other.getSynonymReference()) == (*this->getSynonymReference())
        && (elem_other.getAttribute()) == (this->getAttribute());
  }
  return false;
}
bool AttrReference::operator==(const QueryReference *other) const {
  if (other->getRefType() == this->getRefType()) {
    const auto &elem_other = static_cast<const AttrReference *>(other);
    return (*elem_other->getSynonymReference()) == (*this->getSynonymReference())
        && (elem_other->getAttribute()) == (this->getAttribute());
  }
  return false;
}
SynonymReference *AttrReference::getSynonymReference() const {
  return this->synonym_reference_;
}

ReferenceType AttrReference::getRefType() const {
  return this->getRefType();
}

AttributeType AttrReference::getAttribute() const {
  return this->attribute_type_;
}
std::string AttrReference::toString() const {
  std::string str = this->getSynonymReference()->toString();
  str += ".";
  str += AttrToString(this->getAttribute());
  return str;
}
EntityType AttrReference::getEntityType() const {
  return this->getSynonymReference()->getEntityType();
}
EntityPointerUnorderedSet AttrReference::getContext() const {
  return this->getSynonymReference()->getContext();
}
void AttrReference::setContext(EntityPointerUnorderedSet context) {
  this->getSynonymReference()->setContext(context);
}
void AttrReference::setEntityType(EntityType entity_type) {
  this->getSynonymReference()->setEntityType(entity_type);
}
void AttrReference::setSynonymReference(SynonymReference *synonym_reference) {
  this->synonym_reference_ = synonym_reference;
}
std::string AttrReference::getReferenceValue() const {
  return this->getSynonymReference()->getReferenceValue();
}
bool AttrReference::isStmtRef() const {
  return this->getSynonymReference()->isStmtRef();
}
bool AttrReference::isEntRef() const {
  return this->getSynonymReference()->isEntRef();
}
bool AttrReference::isSyntacticallyCorrect() const {
  return this->getSynonymReference()->isSyntacticallyCorrect();
}
bool AttrReference::isAttrCompareRef() const {
  return true;
}
QuerySynonym *AttrReference::getSynonym() const {
  return getSynonymReference()->getSynonym();
}

