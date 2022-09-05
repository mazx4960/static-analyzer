// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

#include <utility>
QueryBuilder::QueryBuilder(std::vector<Token *> tokens) {
  this->tokens_ = std::move(tokens);

}

Query QueryBuilder::build() {
  std::vector<QueryDeclaration> query_declarations;
  std::vector<QueryClause> clause_vector;
  QueryDeclaration query_declaration = QueryDeclaration(VariableEntity(""), QuerySynonym(""));
  return Query(query_declarations, SelectCall(query_declaration, clause_vector));
}
