// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_call.h"
#include "query_reference.h"
#include "query_clause.h"

using Declarations = std::vector<SynonymReference *>;
using Clauses = std::vector<QueryClause *>;

class Query {
 private:
  Declarations synonym_declarations_;

  QueryCall *query_call_;

  Clauses query_clauses_;

 public:
  Query(Declarations synonym_declarations, QueryCall *query_call, Clauses query_clauses)
      : synonym_declarations_(std::move(synonym_declarations)),
        query_call_(query_call),
        query_clauses_(query_clauses) {
  }

  [[nodiscard]] Declarations getSynonymDeclarations() const;
  [[nodiscard]] QueryCall *getQueryCall() const;
  [[nodiscard]] Clauses getClauses() const;
};
