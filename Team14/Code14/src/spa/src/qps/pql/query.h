// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_call.h"
#include "query_declaration.h"

class Query {
 private:
  std::vector<SynonymDeclaration *> synonym_declarations_;

  QueryCall *query_call_;

 public:
  Query(std::vector<SynonymDeclaration *> synonym_declarations, QueryCall *query_call)
      : synonym_declarations_(std::move(synonym_declarations)),
        query_call_(query_call) {
  }

  [[nodiscard]] std::vector<SynonymDeclaration *> getSynonymDeclarations() const;
  [[nodiscard]] QueryCall *getQueryCall() const;
  [[nodiscard]] bool hasSubClauses() const;
};
