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
  return ReferenceType::kSynonym;
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
        && (elem_other.getAttributeType()) == (this->getAttributeType());
  }
  return false;
}
bool AttrReference::operator==(const QueryReference *other) const {
  if (other->getRefType() == this->getRefType()) {
    const auto &elem_other = static_cast<const AttrReference *>(other);
    return (*elem_other->getSynonymReference()) == (*this->getSynonymReference())
        && (elem_other->getAttributeType()) == (this->getAttributeType());
  }
  return false;
}
SynonymReference *AttrReference::getSynonymReference() const {
  return this->synonym_reference_;
}

ReferenceType AttrReference::getRefType() const {
  return ReferenceType::kAttr;
}

AttributeType AttrReference::getAttributeType() const {
  return this->attribute_type_;
}
std::string AttrReference::getAttribute(Entity *entity) const {
  auto entity_type = entity->GetType();
  switch (attribute_type_) {
    case AttributeType::kProcName:
      if (entity_type == EntityType::kProcedure) {
        return entity->GetValue();
      } else if (entity_type == EntityType::kCallStmt) {
        auto *call_stmt = static_cast<CallStmtEntity *>(entity);
        return call_stmt->GetAttr();
      }
    case AttributeType::kVarName:
      if (entity_type == EntityType::kVariable) {
        return entity->GetValue();
      } else if (entity_type == EntityType::kReadStmt || entity_type == EntityType::kPrintStmt) {
        auto *stmt = static_cast<StmtEntity *>(entity);
        return stmt->GetAttr();
      }
    case AttributeType::kValue:
      if (entity_type == EntityType::kConstant) {
        return entity->GetValue();
      }
    case AttributeType::kStmtNo:
      if (entity_type == EntityType::kAssignStmt || entity_type == EntityType::kCallStmt
          || entity_type == EntityType::kWhileStmt || entity_type == EntityType::kIfStmt
          || entity_type == EntityType::kPrintStmt || entity_type == EntityType::kReadStmt) {
        return entity->GetValue();
      }
    default:return entity->GetValue();
  }
}
std::string AttrReference::toString() const {
  std::string str = this->getSynonymReference()->toString();
  str += ".";
  str += AttrToString(this->getAttributeType());
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
bool AttrReference::isSemanticallyCorrect() const {
  switch (getAttributeType()) {
    case AttributeType::kProcName: {
      switch (getEntityType()) {
        case EntityType::kProcedure:
        case EntityType::kCallStmt:break;
        default:return false;
      }
      break;
    }
    case AttributeType::kVarName: {
      switch (getEntityType()) {
        case EntityType::kVariable:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:break;
        default:return false;
      }
      break;
    }
    case AttributeType::kValue: {
      switch (getEntityType()) {
        case EntityType::kConstant:break;
        default:return false;
      }
      break;
    }
    case AttributeType::kStmtNo: {
      switch (getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kIfStmt:
        case EntityType::kWhileStmt:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:break;
        default:return false;
      }
      break;
    }
    default: break;
  }
  return true;
}
bool AttrReference::isAttrCompareRef() const {
  return true;
}
QuerySynonym *AttrReference::getSynonym() const {
  return getSynonymReference()->getSynonym();
}

