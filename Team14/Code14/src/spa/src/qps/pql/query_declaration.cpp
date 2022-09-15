// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

QuerySynonym *QueryDeclaration::getSynonym() const { return this->query_synonym_; }
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  if (this->type_ == DeclarationType::kWildcard || other.getType() == DeclarationType::kWildcard) {
    return true;
  }
  return this->type_ == other.getType()
      && *(this->query_synonym_) == *(other.getSynonym())
      && this->string_ == other.getString()
      && this->number_ == other.getNumber();
}
bool QueryDeclaration::operator==(const QueryDeclaration *other) const {
  if (this->type_ == DeclarationType::kWildcard || other->getType() == DeclarationType::kWildcard) {
    return true;
  }
  return this->type_ == other->getType()
      && this->query_synonym_ == other->getSynonym()
      && this->string_ == other->getString()
      && this->number_ == other->getNumber();
}
DeclarationType QueryDeclaration::getType() const { return this->type_; }
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

bool DeclarationTypeAdaptor::canConvertToEntityType(DeclarationType declaration_type) {
  switch (declaration_type) {
    case DeclarationType::kProcedure:// fallthrough
    case DeclarationType::kStatement:// fallthrough
    case DeclarationType::kVariable: // fallthrough
    case DeclarationType::kConstant: // fallthrough
      return true;
    default: return false;
  }
}

bool DeclarationTypeAdaptor::canConvertToStatementType(DeclarationType declaration_type) {
  switch (declaration_type) {
    case DeclarationType::kAssign:// fallthrough
    case DeclarationType::kCall:  // fallthrough
    case DeclarationType::kIf:    // fallthrough
    case DeclarationType::kWhile: // fallthrough
    case DeclarationType::kPrint: // fallthrough
    case DeclarationType::kRead:  // fallthrough
      return true;
    default: return false;
  }
}

EntityType DeclarationTypeAdaptor::toEntityType(DeclarationType declaration_type) {
  switch (declaration_type) {
    case DeclarationType::kProcedure: return EntityType::kProcedure;
    case DeclarationType::kStatement: return EntityType::kStatement;
    case DeclarationType::kVariable: return EntityType::kVariable;
    case DeclarationType::kConstant: return EntityType::kConstant;
    default: throw DeclarationTypeAdaptError("DeclarationType cannot be adapted to EntityType");
  }
}

EntityType DeclarationTypeAdaptor::toStatementType(DeclarationType declaration_type) {
  switch (declaration_type) {
    case DeclarationType::kAssign: return EntityType::kAssignStmt;
    case DeclarationType::kCall: return EntityType::kCallStmt;
    case DeclarationType::kIf: return EntityType::kIfStmt;
    case DeclarationType::kWhile: return EntityType::kWhileStmt;
    case DeclarationType::kPrint: return EntityType::kPrintStmt;
    case DeclarationType::kRead: return EntityType::kReadStmt;
    default: throw DeclarationTypeAdaptError("DeclarationType cannot be adapted to EntityType");
  }
}

DeclarationType DeclarationTypeAdaptor::toDeclarationType(EntityType entity_type) {
  switch (entity_type) {
    case EntityType::kProcedure: return DeclarationType::kProcedure;
    case EntityType::kStatement: return DeclarationType::kStatement;
    case EntityType::kVariable: return DeclarationType::kVariable;
    case EntityType::kConstant: return DeclarationType::kConstant;
    case EntityType::kAssignStmt: return DeclarationType::kAssign;
    case EntityType::kCallStmt: return DeclarationType::kCall;
    case EntityType::kIfStmt: return DeclarationType::kIf;
    case EntityType::kWhileStmt: return DeclarationType::kWhile;
    case EntityType::kPrintStmt: return DeclarationType::kPrint;
    case EntityType::kReadStmt: return DeclarationType::kRead;
    default: throw DeclarationTypeAdaptError("EntityType cannot be adapted to DeclarationType");
  }
}
