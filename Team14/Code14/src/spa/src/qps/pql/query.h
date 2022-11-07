// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_call.h"
#include "query_reference.h"
#include "query_clause.h"

using SynonymSet = std::unordered_set<QuerySynonym *>;
using ClauseVector = std::vector<QueryClause *>;

class Query {
 private:
  SynonymSet synonym_declarations_;

  SelectCall *query_call_;

  ClauseVector query_clauses_;

 public:
  Query(SynonymSet synonym_declarations, SelectCall *query_call, ClauseVector query_clauses)
      : synonym_declarations_(std::move(synonym_declarations)),
        query_call_(query_call),
        query_clauses_(std::move(query_clauses)) {
  }

  [[nodiscard]] SynonymSet getSynonymDeclarations() const;
  [[nodiscard]] SelectCall *getQueryCall() const;
  [[nodiscard]] ClauseVector getClauses() const;
  [[nodiscard]] std::string toString() const;
  bool operator==(const Query &other) const;
};
