// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "commons/entity.h"
#include "qps/pql/query_synonym/query_synonym.h"

class QueryDeclaration {
 private:
  Entity entity_;

  QuerySynonym synonym_;

  std::unordered_set<std::string> context_;

 public:
  static QueryDeclaration empty();
  static QueryDeclaration *emptyPtr();

  QueryDeclaration(Entity &entity, QuerySynonym &synonym)
      : entity_(std::move(entity)), synonym_(std::move(synonym)) {};

  QueryDeclaration(Entity &entity, QuerySynonym &synonym, std::unordered_set<std::string> &context)
      : entity_(std::move(entity)), synonym_(std::move(synonym)), context_(std::move(context)) {};

  [[nodiscard]] Entity getEntity() const;
  [[nodiscard]] QuerySynonym getSynonym() const;
  [[nodiscard]] std::unordered_set<std::string> getContext() const;

  void setContext(std::unordered_set<std::string> &);

  bool operator==(const QueryDeclaration &) const;
};

class QueryDeclarationHashFunction {
 public:
  size_t operator()(const QueryDeclaration &) const;
};
