// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "commons/entity.h"
#include "qps/pql/query_synonym.h"
#include "qps/query_evaluator/subquery_result.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class Result {
 protected:
  std::unordered_set<std::string> results_{};

  Result() = default;

 public:

  static Result *empty();

  [[nodiscard]] virtual std::string get_synonyms() const = 0;
  [[nodiscard]] std::unordered_set<std::string> get_results_set() const;
  [[nodiscard]] std::vector<std::string> get_sorted_results_string_list() const;
  [[nodiscard]] bool is_empty() const;
  [[nodiscard]] int size() const;

};

class EmptyResult : public Result {
 public:
  EmptyResult() = default;

  [[nodiscard]] std::string get_synonyms() const override;
};

class ElemResult : public Result {
 private:
  std::vector<ElemReference *> elem_refs_;

 public:
  ElemResult(std::vector<ElemReference *>, SubqueryResult &);

  [[nodiscard]] std::string get_synonyms() const override;
};

class BooleanResult : public Result {
 private:
  const std::string synonym_string_ = "BOOLEAN:BOOLEAN";

  const std::unordered_set<std::string> true_set_ = {"true"};

  const std::unordered_set<std::string> false_set_ = {"false"};

 public:
  explicit BooleanResult(bool has_results);

  [[nodiscard]] std::string get_synonyms() const override;
};