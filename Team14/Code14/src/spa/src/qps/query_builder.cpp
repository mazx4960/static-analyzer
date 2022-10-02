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
StringDeclaration *QueryBuilder::buildString(const std::string &str) {
  return new StringDeclaration(str);
}
SelectCall *QueryBuilder::buildSelectCall(QueryDeclaration *synonym_declaration, std::vector<QueryClause *> clause_vector) {
  this->query_call_ = new SelectCall(synonym_declaration, std::move(clause_vector));
  return this->query_call_;
}
PatternClause *QueryBuilder::buildAssignPattern(QueryDeclaration *pattern_synonym,
                                                QueryDeclaration *first_param,
                                                QueryDeclaration *second_param) {
  return new AssignPatternClause(pattern_synonym, first_param, second_param);
}

QueryDeclaration *QueryBuilder::buildWildcardExpression(std::string expression) {
  return new WildCardExpressionDeclaration(std::move(expression));
}
QueryDeclaration *QueryBuilder::buildExpression(std::string expression) {
  return new ExpressionDeclaration(std::move(expression));
}
QueryDeclaration *QueryBuilder::getStmtDeclaration(const std::string& synonym) {
  QueryDeclaration *declaration = getDeclaration(synonym);
  if ((GetStmtRefTypes().count(declaration->getType()) == 0U)) {
    throw ParseSemanticError("Synonym given is not a stmtref: " + synonym);
  }
  return declaration;
}
QueryDeclaration *QueryBuilder::getEntDeclaration(const std::string& synonym) {
  QueryDeclaration *declaration = getDeclaration(synonym);
  if ((GetEntRefTypes().count(declaration->getType()) == 0U)) {
    throw ParseSemanticError("Synonym given is not aa entref: " + synonym);
  }
  return declaration;
}
SuchThatClause *QueryBuilder::buildSuchThat(RsType type, QueryDeclaration *first, QueryDeclaration *second) {
  return new SuchThatClause(type, first, second);
}
std::vector<QueryDeclaration *> QueryBuilder::getDeclarations() {
  return this->query_declarations_;
}
QueryCall *QueryBuilder::getQueryCall() {
  return this->query_call_;
}

