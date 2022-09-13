// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_clause.h"
#include "query_declaration.h"

enum class CallType {
  kSelect,
};

class QueryCall {
 protected:
  CallType type_;

  QueryDeclaration *query_declaration_;

  std::vector<QueryClause *> clause_vector_;

 public:
  QueryCall(CallType call_type, QueryDeclaration *query_declaration, std::vector<QueryClause *> clause_vector)
      : type_(call_type),
        query_declaration_(std::move(query_declaration)),
        clause_vector_(std::move(clause_vector)) {};

  [[nodiscard]] CallType getType() const;
  [[nodiscard]] QueryDeclaration *getDeclaration() const;
  [[nodiscard]] bool hasSubClauses() const;
  [[nodiscard]] std::vector<QueryClause *> getClauseVector() const;
};

class SelectCall : public QueryCall {
 public:
  SelectCall(QueryDeclaration *query_declaration, std::vector<QueryClause *> clause_vector)
      : QueryCall(CallType::kSelect, std::move(query_declaration), std::move(clause_vector)) {};
};
