// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>

#include "commons/entity.h"
#include "qps/pql/query_synonym/query_synonym.h"

class QueryDeclaration {
 public:
  QueryDeclaration(VariableEntity entity,
                   QuerySynonym synonym) : entity_(std::move(entity)), synonym_(std::move(synonym)) {};
  VariableEntity entity_;
  QuerySynonym synonym_;
  VariableEntity getEntity() const;
  QuerySynonym getSynonym() const;
};
