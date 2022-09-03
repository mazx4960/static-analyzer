// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "qps/pql/query_clause/query_clause.h"
#include "qps/pql/query_declaration/query_declaration.h"

enum class CallType {
  kSelect,
};

class QueryCall {
 protected:
  QueryCall(CallType call_type, QueryDeclaration query_declaration, std::vector<QueryClause> clause_vector)
  : type_(call_type), query_declaration_(std::move(query_declaration)), clause_vector_(std::move(clause_vector)) {};
  CallType type_;
  QueryDeclaration query_declaration_;
  std::vector<QueryClause> clause_vector_;
 public:
  CallType getType();
  QueryDeclaration getDeclaration();
  std::vector<QueryClause> getClauseVector();

};

class SelectCall : public QueryCall {
 public:
  SelectCall(QueryDeclaration query_declaration, std::vector<QueryClause> clause_vector)
  : QueryCall(CallType::kSelect, std::move(query_declaration), std::move(clause_vector)) {};
};
