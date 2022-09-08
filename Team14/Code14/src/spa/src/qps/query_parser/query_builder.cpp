// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

QueryBuilder::QueryBuilder() {
}

Query QueryBuilder::build() {
  return Query(query_declarations_, *query_calls_[0]);
}

void QueryBuilder::withDeclarations(std::vector<QueryDeclaration *> query_declarations) {
  this->query_declarations_ = query_declarations;

}
void QueryBuilder::withQueryCalls(std::vector<QueryCall *> query_calls) {
  this->query_calls_ = query_calls;

}
