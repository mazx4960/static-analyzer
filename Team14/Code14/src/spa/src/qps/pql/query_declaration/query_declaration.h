// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>

#include "commons/entity.h"
#include "qps/pql/query_synonym/query_synonym.h"

class QueryDeclaration {
 private:
  VariableEntity entity_;

  QuerySynonym synonym_;

 public:
  QueryDeclaration(VariableEntity entity,
                   QuerySynonym synonym) : entity_(std::move(entity)), synonym_(std::move(synonym)) {};

  [[nodiscard]] VariableEntity getEntity() const;
  [[nodiscard]] QuerySynonym getSynonym() const;

  bool operator==(const QueryDeclaration &other) const;
};

class QueryDeclarationHashFunction {
 public:
  size_t operator()(const QueryDeclaration &) const;
};
