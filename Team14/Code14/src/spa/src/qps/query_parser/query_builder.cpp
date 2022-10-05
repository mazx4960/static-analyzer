// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

#include <utility>

#include "qps/exceptions.h"

QueryBuilder::QueryBuilder() = default;
Query *QueryBuilder::build() {
  for (auto *clause : unchecked_clauses_) {
    switch (clause->getClauseType()) {
      case ClauseType::kPattern: {
        buildAssignPattern(static_cast<PatternClause *>(clause));
        break;
      }
      case ClauseType::kSuchThat: {
        buildSuchThat(static_cast<SuchThatClause *>(clause));
        break;
      }
      default: {
        throw ParseSemanticError("Unknown clause type!");
      }
    }
  }
  if (selected_declaration_ == nullptr) {
    throw ParseSemanticError("Synonym in Select not declared!");
  }
  query_call_ = new SelectCall(selected_declaration_, built_clauses_);
  return new Query(this->query_declarations_, query_call_);
}

QuerySynonym *QueryBuilder::buildSynonym(const std::string &synonym) {
  if (isDeclared(synonym)) {
    throw ParseSemanticError("Synonym already declared: " + synonym);
  }
  this->synonyms_.insert(synonym);
  return new QuerySynonym(synonym);
}

QueryDeclaration *QueryBuilder::buildDeclaration(EntityType type, QuerySynonym *synonym) {
  auto *declaration = new QueryDeclaration(type, synonym);
  this->query_declarations_.push_back(declaration);
  return declaration;
}

QueryDeclaration *QueryBuilder::getDeclaration(const std::string &synonym) {
  auto *to_check = new QuerySynonym(synonym);
  for (QueryDeclaration *declaration : query_declarations_) {
    QuerySynonym *declaration_synonym = declaration->getSynonym();
    if (*declaration_synonym == *to_check) {
      return declaration;
    }
  }
  return nullptr;
}

bool QueryBuilder::isDeclared(const std::string &synonym) {
  return this->synonyms_.count(synonym) != 0U;
}
WildCardStmtDeclaration *QueryBuilder::buildWildcardStmt() {
  return new WildCardStmtDeclaration();
}
WildCardEntDeclaration *QueryBuilder::buildWildcardEnt() {
  return new WildCardEntDeclaration();
}
WildCardProcedureDeclaration *QueryBuilder::buildWildcardProcedure() {
  return new WildCardProcedureDeclaration();
}
IntegerDeclaration *QueryBuilder::buildLiteral(const std::string &number) {
  return new IntegerDeclaration(number);
}
IdentDeclaration *QueryBuilder::buildIdent(const std::string &str) {
  return new IdentDeclaration(str);
}
QueryDeclaration *QueryBuilder::buildWildcardExpression(std::string expression) {
  return new WildCardExpressionDeclaration(std::move(expression));
}
QueryDeclaration *QueryBuilder::buildExpression(std::string expression) {
  return new ExpressionDeclaration(std::move(expression));
}
void QueryBuilder::withSelectCall(QueryDeclaration *synonym_declaration) {
  this->selected_declaration_ = synonym_declaration;
}

void QueryBuilder::withAssignPattern(QueryDeclaration *syn_assign,
                                     QueryDeclaration *ent_ref,
                                     QueryDeclaration *expression_spec) {
  unchecked_clauses_.push_back(new AssignPatternClause(syn_assign, ent_ref, expression_spec));
}

void QueryBuilder::withSuchThat(RsType type, QueryDeclaration *first, QueryDeclaration *second) {
  unchecked_clauses_.push_back(new SuchThatClause(type, first, second));
}

void QueryBuilder::buildAssignPattern(PatternClause *clause) {
  QueryDeclaration *syn_assign = clause->getFirst();
  QueryDeclaration *ent_ref = clause->getSecond();
  QueryDeclaration *expression_spec = clause->getThird();
  if (syn_assign == nullptr || ent_ref == nullptr || expression_spec == nullptr) {
    throw ParseSemanticError("Synonym not found");
  }

  auto pattern_rules = getPatternRules();
  if (pattern_rules[0].find(syn_assign->getType()) == pattern_rules[0].end()) {
    throw ParseSemanticError("Invalid syn_assign: " + EntityTypeToString(syn_assign->getType()));
  }
  if (pattern_rules[1].find(ent_ref->getType()) == pattern_rules[1].end()) {
    throw ParseSemanticError("Invalid ent_ref: " + EntityTypeToString(ent_ref->getType()));
  }
  if (pattern_rules[2].find(expression_spec->getType()) == pattern_rules[2].end()) {
    throw ParseSemanticError("Invalid expression: " + EntityTypeToString(expression_spec->getType()));
  }
  built_clauses_.push_back(clause);
}

void QueryBuilder::buildSuchThat(SuchThatClause *clause) {
  RsType type = clause->getSuchThatType();
  QueryDeclaration *first = clause->getFirst();
  QueryDeclaration *second = clause->getSecond();
  if (first == nullptr || second == nullptr) {
    throw ParseSemanticError("Synonym not found");
  }
  auto st_rules = getSuchThatRules();
  if (st_rules.find(type) == st_rules.end()) {
    throw ParseSemanticError("Unsupported relationship type: " + RsTypeToString(type));
  }
  auto relationship_rules = st_rules[type];
  if (relationship_rules[0].find(first->getType()) == relationship_rules[0].end()) {
    throw ParseSemanticError("Invalid first arg: " + EntityTypeToString(first->getType()));
  }
  if (relationship_rules[1].find(second->getType()) == relationship_rules[1].end()) {
    throw ParseSemanticError("Invalid second arg: " + EntityTypeToString(second->getType()));
  }
  built_clauses_.push_back(clause);
}
std::vector<QueryDeclaration *> QueryBuilder::getDeclarations() {
  return this->query_declarations_;
}
QueryCall *QueryBuilder::getQueryCall() {
  return this->query_call_;
}

