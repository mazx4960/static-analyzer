// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

QueryDeclaration QueryDeclaration::empty() {
  Entity entity = VariableEntity("");
  QuerySynonym synonym = QuerySynonym("");
  return QueryDeclaration(entity, synonym);
}
QueryDeclaration *QueryDeclaration::emptyPtr() {
  Entity entity = VariableEntity("");
  QuerySynonym synonym = QuerySynonym("");
  return new QueryDeclaration(entity, synonym);
}
Entity QueryDeclaration::getEntity() const {
  return this->entity_;
}
QuerySynonym QueryDeclaration::getSynonym() const {
  return this->synonym_;
}
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  return this->getSynonym() == other.getSynonym();
}
std::unordered_set<std::string> QueryDeclaration::getContext() const {
  return this->context_;
}
void QueryDeclaration::setContext(std::unordered_set<std::string> &context) {
  std::unordered_set<std::string> new_context(context);
  this->context_ = std::move(new_context);
}

size_t QueryDeclarationHashFunction::operator()(const QueryDeclaration &query_declaration) const {
  // Hash using QuerySynonymHashFunction
  return QuerySynonymHashFunction().operator()(query_declaration.getSynonym());
}
