// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

#include <utility>

#include "qps/exceptions.h"

QueryBuilder::QueryBuilder() = default;
Query *QueryBuilder::build() {
  return new Query(this->query_declarations_, this->query_call_);
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
  throw ParseSemanticError("Missing declaration: " + synonym);
}

bool QueryBuilder::isDeclared(const std::string &synonym) { return this->synonyms_.count(synonym) != 0U; }
WildCardStmtDeclaration *QueryBuilder::buildWildcardStmt() {
  return new WildCardStmtDeclaration();
}
WildCardEntDeclaration * QueryBuilder::buildWildcardEnt() {
  return new WildCardEntDeclaration();
}
IntegerDeclaration *QueryBuilder::buildLiteral(const std::string &number) {
  return new IntegerDeclaration(number);
}
IdentDeclaration *QueryBuilder::buildIdent(const std::string &str) {
  return new IdentDeclaration(str);
}
SelectCall *QueryBuilder::buildSelectCall(QueryDeclaration *synonym_declaration, std::vector<QueryClause *> clause_vector) {
  this->query_call_ = new SelectCall(synonym_declaration, std::move(clause_vector));
  return this->query_call_;
}
PatternClause *QueryBuilder::buildAssignPattern(QueryDeclaration *syn_assign,
                                                QueryDeclaration *ent_ref,
                                                QueryDeclaration *expression_spec) {
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
  return new AssignPatternClause(syn_assign, ent_ref, expression_spec);
}

QueryDeclaration *QueryBuilder::buildWildcardExpression(std::string expression) {
  return new WildCardExpressionDeclaration(std::move(expression));
}
QueryDeclaration *QueryBuilder::buildExpression(std::string expression) {
  return new ExpressionDeclaration(std::move(expression));
}

SuchThatClause *QueryBuilder::buildSuchThat(RsType type, QueryDeclaration *first, QueryDeclaration *second) {
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


  return new SuchThatClause(type, first, second);
}
std::vector<QueryDeclaration *> QueryBuilder::getDeclarations() {
  return this->query_declarations_;
}
QueryCall *QueryBuilder::getQueryCall() {
  return this->query_call_;
}

