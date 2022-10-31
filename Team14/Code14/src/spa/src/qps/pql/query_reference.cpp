// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_reference.h"

#include <spdlog/spdlog.h>

ReferenceType QueryReference::getRefType() const {
  return this->reference_type_;
}
bool QueryReference::operator==(const QueryReference &other) const {
  return this->reference_type_ == other.reference_type_;
}
bool QueryReference::operator==(const QueryReference *other) const {
  return this->reference_type_ == other->reference_type_;
}

// WildcardDeclaration
EntityType WildcardReference::getEntityType() const {
  return this->entity_type_;
}
std::string WildcardReference::toString() const {
  return EntityTypeToString(this->getEntityType()) + ":_";
}
bool WildcardReference::operator==(const QueryReference &other) const {
  return QueryReference::operator==(other);
}
bool WildcardReference::operator==(const QueryReference *other) const {
  return QueryReference::operator==(other);
}
void WildcardReference::setEntityType(EntityType entity_type) {
  this->entity_type_ = entity_type;
}

// IdentDeclaration
std::string IdentReference::getValue() const {
  return this->value_;
}
std::string IdentReference::toString() const {
  return "Ident:" + this->value_;
}
bool IdentReference::operator==(const QueryReference &other) const {
  return QueryReference::operator==(other) && this->value_ == (static_cast<const IdentReference &>(other)).value_;
}
bool IdentReference::operator==(const QueryReference *other) const {
  return QueryReference::operator==(other) && this->value_ == (static_cast<const IdentReference *>(other))->value_;
}

// IntegerDeclaration
std::string IntegerReference::getValue() const {
  return this->value_;
}
std::string IntegerReference::toString() const {
  return "Integer:" + this->value_;
}
bool IntegerReference::operator==(const QueryReference &other) const {
  return QueryReference::operator==(other) && this->value_ == (static_cast<const IntegerReference &>(other)).value_;
}
bool IntegerReference::operator==(const QueryReference *other) const {
  return QueryReference::operator==(other) && this->value_ == (static_cast<const IntegerReference *>(other))->value_;
}

// SynonymDeclaration
QuerySynonym *SynonymReference::getSynonym() const {
  return this->query_synonym_;
}

int SynonymReference::getUsage() const {
  return this->getSynonym()->GetUsage();
}
std::string SynonymReference::toString() const {
  return this->getSynonym()->ToString();
}
bool SynonymReference::operator==(const QueryReference &other) const {
  return QueryReference::operator==(other)
      && this->query_synonym_ == (static_cast<const SynonymReference &>(other)).query_synonym_;
}
bool SynonymReference::operator==(const QueryReference *other) const {
  return QueryReference::operator==(other)
      && this->query_synonym_ == (static_cast<const SynonymReference *>(other))->query_synonym_;
}

// AttributeReference
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
    default:
      return entity->GetValue();
  }
}
std::string AttrReference::toString() const {
  std::string str = this->synonym_->GetName();
  str += ".";
  str += AttrToString(this->getAttributeType());
  return str;
}
bool AttrReference::isSemanticallyCorrect() const {
  switch (getAttributeType()) {
    case AttributeType::kProcName: {
      switch (this->synonym_->GetEntityType()) {
        case EntityType::kProcedure:
        case EntityType::kCallStmt:
          break;
        default:
          return false;
      }
      break;
    }
    case AttributeType::kVarName: {
      switch (this->synonym_->GetEntityType()) {
        case EntityType::kVariable:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:
          break;
        default:
          return false;
      }
      break;
    }
    case AttributeType::kValue: {
      switch (this->synonym_->GetEntityType()) {
        case EntityType::kConstant:
          break;
        default:
          return false;
      }
      break;
    }
    case AttributeType::kStmtNo: {
      switch (this->synonym_->GetEntityType()) {
        case EntityType::kStatement:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kIfStmt:
        case EntityType::kWhileStmt:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:
          break;
        default:
          return false;
      }
      break;
    }
    default:
      break;
  }
  return true;
}
QuerySynonym *AttrReference::getSynonym() const {
  return this->synonym_;
}
bool AttrReference::operator==(const QueryReference &other) const {
  return QueryReference::operator==(other) && this->synonym_ == (static_cast<const AttrReference &>(other)).synonym_
      && this->attribute_type_ == (static_cast<const AttrReference &>(other)).attribute_type_;
}
bool AttrReference::operator==(const QueryReference *other) const {
  return QueryReference::operator==(other) && this->synonym_ == (static_cast<const AttrReference *>(other))->synonym_
      && this->attribute_type_ == (static_cast<const AttrReference *>(other))->attribute_type_;
}
