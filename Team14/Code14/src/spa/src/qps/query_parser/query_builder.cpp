// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

#include <utility>

#include "qps/exceptions.h"
#include "spdlog/spdlog.h"

QueryBuilder::QueryBuilder(Query *maybe_query) {
  if (maybe_query == nullptr) {
    throw ParseSemanticError("No query given");
  }
  this->maybe_query_ = maybe_query;
}

Query *QueryBuilder::build() {
  spdlog::info("Building query...");
  spdlog::info("Building declarations...");
  this->declarations_ = buildDeclarations(maybe_query_->getSynonymDeclarations());
  spdlog::info("Declarations built successfully");

  if (maybe_query_->getQueryCall() == nullptr) {
    throw ParseSemanticError("Bad Select creation");
  }
  if (maybe_query_->getQueryCall()->getReference() == nullptr) {
    throw ParseSemanticError("Synonym in Select is not declared!");
  }
  spdlog::info("Building clauses...");
  buildClauses(maybe_query_->getQueryCall()->getClauseVector());
  spdlog::info("Clauses built successfully");
  spdlog::info("Query built successfully");
  return maybe_query_;
}

std::vector<SynonymReference *> QueryBuilder::buildDeclarations(const std::vector<SynonymReference *>& maybe_declarations) {
  for (auto *maybe_declaration: maybe_declarations) {
    buildDeclaration(maybe_declaration);
  }
  return maybe_declarations;
}

SynonymReference *QueryBuilder::buildDeclaration(SynonymReference *maybe_declaration) {
  if (maybe_declaration == nullptr) {
    throw ParseSemanticError("Missing declaration");
  }
  if (!synonyms_.insert(maybe_declaration->toString()).second) {
    throw ParseSemanticError("Synonym already declared: " + maybe_declaration->toString());
  }
  return maybe_declaration;
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

std::vector<QueryClause *> QueryBuilder::buildClauses(const std::vector<QueryClause *> &maybe_clauses) {
  for (auto *clause : maybe_clauses) {
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
  return maybe_clauses;
}

PatternClause *QueryBuilder::buildAssignPattern(PatternClause *maybe_clause) {
  SynonymReference *syn_assign = maybe_clause->getSynonymDeclaration();
  QueryReference *ent_ref = maybe_clause->getEntRef();
  StaticDeclaration *expression_spec = maybe_clause->getExpression();
  if (syn_assign == nullptr || ent_ref == nullptr || expression_spec == nullptr) {
    throw ParseSemanticError("Missing parameters!");
  }
  if (syn_assign->getRefType() == ReferenceType::kSynonym) {
    maybe_clause->setSynonymDeclaration(getDeclaration(syn_assign->getSynonym()));
  }
  auto pattern_rules = pattern_args_rule;
  checkParamSemantics(pattern_rules, syn_assign, 0);
  checkParamSemantics(pattern_rules, ent_ref, 1);
  checkParamSemantics(pattern_rules, expression_spec, 2);
  return maybe_clause;
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

void QueryBuilder::checkParamSemantics(std::vector<EntityTypeSet> semantic_rules,
                                       QueryReference *query_declaration,
                                       int rule_no) {
  if (semantic_rules[rule_no].find(query_declaration->getEntityType()) == semantic_rules[rule_no].end()) {
    throw ParseSemanticError("Invalid parameter type: " + EntityTypeToString(query_declaration->getEntityType()));
  }
  if (query_declaration->getEntityType() == EntityType::kWildcard) {
    auto *wildcard_declaration = static_cast<WildcardReference*>(query_declaration);
    wildcard_declaration->setWildcardType(semantic_rules[rule_no + 2]);
  }
}
