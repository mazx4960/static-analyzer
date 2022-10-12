// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

QueryBuilder::QueryBuilder(Query *query_blueprint) {
  if (query_blueprint == nullptr) {
    throw ParseSemanticError("No query given");
  }
  this->query_ = query_blueprint;
}

Query *QueryBuilder::build() {
  this->declarations_ = buildDeclarations(query_->getSynonymDeclarations());
  buildQueryCall(query_->getQueryCall());
  buildClauses(query_->getClauses());
  return query_;
}

std::vector<SynonymReference *> QueryBuilder::buildDeclarations(const std::vector<SynonymReference *> &declaration_blueprints) {
  for (auto *declaration_blueprint : declaration_blueprints) {
    buildDeclaration(declaration_blueprint);
  }
  return declaration_blueprints;
}

SynonymReference *QueryBuilder::buildDeclaration(SynonymReference *declaration_blueprint) {
  if (declaration_blueprint == nullptr) {
    throw ParseSemanticError("Missing declaration");
  }
  if (!synonyms_.insert(declaration_blueprint->getReferenceValue()).second) {
    throw ParseSemanticError("Synonym already declared: " + declaration_blueprint->getReferenceValue());
  }
  return declaration_blueprint;
}

QueryCall *QueryBuilder::buildQueryCall(QueryCall *query_call_blueprint) {
  if (query_call_blueprint == nullptr) {
    throw ParseSemanticError("Missing Select call");
  }
  query_call_blueprint->setReference(this->getDeclaration(query_call_blueprint->getReference()->getSynonym()));
  return query_call_blueprint;
}

SynonymReference *QueryBuilder::getDeclaration(const QuerySynonym *synonym) {
  for (auto *declaration : declarations_) {
    QuerySynonym *declaration_synonym = declaration->getSynonym();
    if (*declaration_synonym == *synonym) {
      return declaration;
    }
  }
  throw ParseSemanticError("Synonym is not declared: " + synonym->toString());
}

std::vector<QueryClause *> QueryBuilder::buildClauses(const std::vector<QueryClause *> &clauses_blueprint) {
  for (auto *clause : clauses_blueprint) {
    switch (clause->getClauseType()) {
      case ClauseType::kPattern:buildPattern(static_cast<PatternClause *>(clause));
        break;
      case ClauseType::kSuchThat:buildSuchThat(static_cast<SuchThatClause *>(clause));
        break;
      default:throw ParseSemanticError("Unsupported clause type!");
    }
  }
  return clauses_blueprint;
}

PatternClause *QueryBuilder::buildPattern(PatternClause *clause_blueprint) {
  SynonymReference *syn_assign = clause_blueprint->getSynonymDeclaration();
  QueryReference *ent_ref = clause_blueprint->getEntRef();
  ExpressionSpec *expression_spec = clause_blueprint->getExpression();
  if (syn_assign == nullptr || ent_ref == nullptr || expression_spec == nullptr) {
    throw ParseSemanticError("Missing parameters");
  }
  clause_blueprint->setSynonymReference(getDeclaration(syn_assign->getSynonym()));
  if (ent_ref->getRefType() == ReferenceType::kSynonym) {
    clause_blueprint->setEntReference(getDeclaration(static_cast<SynonymReference *>(ent_ref)->getSynonym()));
  }
  if (!clause_blueprint->IsSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid pattern parameter type");
  }
  return clause_blueprint;
}

SuchThatClause *QueryBuilder::buildSuchThat(SuchThatClause *clause_blueprint) {
  QueryReference *first = clause_blueprint->getFirst();
  QueryReference *second = clause_blueprint->getSecond();
  if (first == nullptr || second == nullptr) {
    throw ParseSemanticError("Missing parameters!");
  }
  if (first->getRefType() == ReferenceType::kSynonym) {
    clause_blueprint->setFirst(getDeclaration(static_cast<SynonymReference *>(first)->getSynonym()));
  }
  if (second->getRefType() == ReferenceType::kSynonym) {
    clause_blueprint->setSecond(getDeclaration(static_cast<SynonymReference *>(second)->getSynonym()));
  }
  if (!clause_blueprint->IsSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid such that parameter type");
  }
  return clause_blueprint;
}
