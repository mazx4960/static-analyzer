// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "qps/pql/query_declaration/query_declaration.h"
#include "qps/pql/query_call/query_call.h"

class Query {
 private:
  std::vector<QueryDeclaration *> query_declarations_;

  QueryCall query_call_;

 public:
  Query(std::vector<QueryDeclaration *> query_declarations, QueryCall query_call)
      : query_declarations_(std::move(query_declarations)), query_call_(std::move(query_call)) {}

  [[nodiscard]] std::vector<QueryDeclaration *> getDeclarations() const;
  [[nodiscard]] QueryCall getQueryCall() const;
  [[nodiscard]] bool hasSubClauses() const;
};

