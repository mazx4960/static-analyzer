// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

#include <utility>

#include "qps/exceptions.h"

QuerySynonym *QueryDeclaration::getSynonym() const { return this->query_synonym_; }
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  return this->query_synonym_ == other.getSynonym()
      && this->type_ == other.getType()
      && this->string_ == other.getString()
      && this->number_ == other.getNumber();
}
DeclarationType QueryDeclaration::getType() const { return this->type_; }
std::string QueryDeclaration::getString() const { return this->string_; }
int QueryDeclaration::getNumber() const { return this->number_; }
std::unordered_set<Entity *> QueryDeclaration::getContext() const { return this->context_; }
void QueryDeclaration::setContext(std::unordered_set<Entity *> context) { this->context_ = std::move(context); }

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

StmtType DeclarationTypeAdaptor::toStatementType(DeclarationType declaration_type) {
  switch (declaration_type) {
    case DeclarationType::kAssign: return StmtType::kAssign;
    case DeclarationType::kCall: return StmtType::kCall;
    case DeclarationType::kIf: return StmtType::kIf;
    case DeclarationType::kWhile: return StmtType::kWhile;
    case DeclarationType::kPrint: return StmtType::kPrint;
    case DeclarationType::kRead: return StmtType::kRead;
    default: throw DeclarationTypeAdaptError("DeclarationType cannot be adapted to StmtType");
  }
}

DeclarationType DeclarationTypeAdaptor::toDeclarationType(EntityType entity_type) {
  switch (entity_type) {
    case EntityType::kProcedure: return DeclarationType::kProcedure;
    case EntityType::kStatement: return DeclarationType::kStatement;
    case EntityType::kVariable: return DeclarationType::kVariable;
    case EntityType::kConstant: return DeclarationType::kConstant;
    default: throw DeclarationTypeAdaptError("EntityType cannot be adapted to DeclarationType");
  }
}

DeclarationType DeclarationTypeAdaptor::toDeclarationType(StmtType stmt_type) {
  switch (stmt_type) {
    case StmtType::kAssign: return DeclarationType::kAssign;
    case StmtType::kCall: return DeclarationType::kCall;
    case StmtType::kIf: return DeclarationType::kIf;
    case StmtType::kWhile: return DeclarationType::kWhile;
    case StmtType::kPrint: return DeclarationType::kPrint;
    case StmtType::kRead: return DeclarationType::kRead;
    default: throw DeclarationTypeAdaptError("StmtType cannot be adapted to DeclarationType");
  }
}

size_t QueryDeclarationHashFunction::operator()(const QueryDeclaration &query_declaration) const {
  return QuerySynonymHashFunction().operator()(query_declaration.getSynonym());
}
