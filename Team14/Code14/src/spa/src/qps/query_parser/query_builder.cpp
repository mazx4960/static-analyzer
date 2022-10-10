// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

#include <utility>

#include "qps/exceptions.h"
#include "spdlog/spdlog.h"

QueryBuilder::QueryBuilder(Query *query_blueprint) {
  if (query_blueprint == nullptr) {
    throw ParseSemanticError("No query given");
  }
  this->query_ = query_blueprint;
}

Query *QueryBuilder::build() {
  spdlog::info("Building query...");
  spdlog::info("Building declarations...");
  this->declarations_ = buildDeclarations(query_->getSynonymDeclarations());
  spdlog::info("Declarations built successfully");

  if (query_->getQueryCall() == nullptr) {
    throw ParseSemanticError("Bad Select creation");
  }
  if (query_->getQueryCall()->getReference() == nullptr) {
    throw ParseSemanticError("Synonym in Select is not declared!");
  }
  spdlog::info("Building clauses...");
  buildClauses(query_->getQueryCall()->getClauseVector());
  spdlog::info("Clauses built successfully");
  spdlog::info("Query built successfully");
  return query_;
}

std::vector<SynonymReference *> QueryBuilder::buildDeclarations(const std::vector<SynonymReference *>& maybe_declarations) {
  for (auto *maybe_declaration: maybe_declarations) {
    buildDeclaration(maybe_declaration);
  }
  return maybe_declarations;
}

SynonymReference *QueryBuilder::buildDeclaration(SynonymReference *declaration_blueprint) {
  if (declaration_blueprint == nullptr) {
    throw ParseSemanticError("Missing declaration");
  }
  if (!synonyms_.insert(declaration_blueprint->toString()).second) {
    throw ParseSemanticError("Synonym already declared: " + declaration_blueprint->toString());
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
      case ClauseType::kPattern:
        buildAssignPattern(static_cast<PatternClause *>(clause));
        break;
      case ClauseType::kSuchThat:
        buildSuchThat(static_cast<SuchThatClause *>(clause));
        break;
      default:
        throw ParseSemanticError("Unsupported clause type!");
    }
  }
  return clauses_blueprint;
}

PatternClause *QueryBuilder::buildAssignPattern(PatternClause *clause_blueprint) {
  SynonymReference *syn_assign = clause_blueprint->getSynonymDeclaration();
  QueryReference *ent_ref = clause_blueprint->getEntRef();
  ExpressionSpec *expression_spec = clause_blueprint->getExpression();
  if (syn_assign == nullptr || ent_ref == nullptr || expression_spec == nullptr) {
    throw ParseSemanticError("Missing parameters");
  }
  clause_blueprint->setSynonymDeclaration(getDeclaration(syn_assign->getSynonym()));

  return clause_blueprint;
}

SuchThatClause *QueryBuilder::buildSuchThat(SuchThatClause *maybe_clause) {
  RsType type = maybe_clause->getSuchThatType();
  QueryReference *first = maybe_clause->getFirst();
  QueryReference *second = maybe_clause->getSecond();
  if (first == nullptr || second == nullptr) {
    throw ParseSemanticError("Missing parameters!");
  }
  if (first->getRefType() == ReferenceType::kSynonym) {
    maybe_clause->setFirst(getDeclaration(static_cast<SynonymReference *>(first)->getSynonym()));
  }
  if (second->getRefType() == ReferenceType::kSynonym) {
    maybe_clause->setSecond(getDeclaration(static_cast<SynonymReference *>(second)->getSynonym()));
  }
  auto st_rules = such_that_semantic_rules;
  if (st_rules.find(type) == st_rules.end()) {
    throw ParseSemanticError("Unsupported relationship type: " + RsTypeToString(type));
  }
  auto relationship_rules = st_rules[type];
  checkParamSemantics(relationship_rules, first, 0);
  checkParamSemantics(relationship_rules, second, 1);
  return maybe_clause;
}
