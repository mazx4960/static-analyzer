// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_call.h"
#include "query_reference.h"
#include "query_clause.h"

using SynonymReferences = std::vector<SynonymReference *>;
using Clauses = std::vector<QueryClause *>;

class Query {
 private:
  SynonymReferences synonym_declarations_;

  QueryCall *query_call_;

  Clauses query_clauses_;

 public:
  Query(SynonymReferences synonym_declarations, QueryCall *query_call, Clauses query_clauses)
      : synonym_declarations_(std::move(synonym_declarations)),
        query_call_(query_call),
        query_clauses_(std::move(query_clauses)) {
  }

  [[nodiscard]] SynonymReferences getSynonymDeclarations() const;
  [[nodiscard]] QueryCall *getQueryCall() const;
  [[nodiscard]] Clauses getClauses() const;
  [[nodiscard]] std::string toString() const;
};
