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

/**
 * Abstract base class;
 */
class Result {
 protected:
  std::unordered_set<std::string> results_;

  Result() = default;

 public:
  /**
   * Creates empty result object.
   * @return empty result object.
   */
  static Result *empty();

  /**
   * Gets synonyms to be selected.
   * @return formatted string of synonyms.
   */
  [[nodiscard]] virtual std::string get_synonyms() const = 0;

  /**
   * Gets set of result strings.
   * @return set (unordered) of result strings.
   */
  [[nodiscard]] std::unordered_set<std::string> get_results_set() const;

  /**
   * Gets sorted vector of result strings.
   * Used for debugging.
   * @return vector of sorted result strings.
   */
  [[nodiscard]] std::vector<std::string> get_sorted_results_string_list() const;

  /**
   * Checks if result is empty.
   * @return true if result is empty.
   */
  [[nodiscard]] bool is_empty() const;

  /**
   * Number of elements in result.
   * @return number of elements in result.
   */
  [[nodiscard]] int size() const;

};

/**
 * Empty result class.
 */
class EmptyResult : public Result {
 public:
  EmptyResult() = default;

  [[nodiscard]] std::string get_synonyms() const override;
};

/*
 * Result for Select element clause.
 */
class ElemResult : public Result {
 private:
  std::vector<ElemReference *> elem_refs_;

 public:
  ElemResult(std::vector<ElemReference *> &, SubqueryResult &);

  [[nodiscard]] std::string get_synonyms() const override;
};

/**
 * Result for Select BOOLEAN clause.
 */
class BooleanResult : public Result {
 private:
  const std::string synonym_string_ = "BOOLEAN:BOOLEAN";

  const std::unordered_set<std::string> true_set_ = {"true"};

  const std::unordered_set<std::string> false_set_ = {"false"};

 public:
  explicit BooleanResult(bool has_results);

  [[nodiscard]] std::string get_synonyms() const override;
};