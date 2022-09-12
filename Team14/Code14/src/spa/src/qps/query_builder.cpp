// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"

#include <utility>

#include "qps/exceptions.h"

QueryBuilder::QueryBuilder() = default;

Query QueryBuilder::build() {
  if (query_declarations_.empty() || query_calls_.empty()) { throw BuilderError("failed to build query"); }
  return Query(query_declarations_, *query_calls_[0]);
}

void QueryBuilder::withDeclarations(std::vector<QueryDeclaration *> query_declarations) {
  this->query_declarations_ = std::move(query_declarations);
}
void QueryBuilder::withQueryCalls(std::vector<QueryCall *> query_calls) { this->query_calls_ = std::move(query_calls); }
