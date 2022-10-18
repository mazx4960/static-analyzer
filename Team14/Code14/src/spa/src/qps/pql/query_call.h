// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_reference.h"

/**
 * Represents a Select clause in a PQL query.
 */
class SelectCall {
 private:
  SelectType type_;
 public:
  explicit SelectCall(SelectType type_) : type_(type_) {
  };
  [[nodiscard]] SelectType getSelectType() const;
  [[nodiscard]] virtual std::string toString() const;
};

/**
 * Represents a selecting of elements in a PQL query.
 * Example: Select s1 such that Follows(s1, s2)
 *          Select <s1, s2, s3> pattern s1(s2, _)
 */
class ElemSelect : public SelectCall {
 private:
  std::vector<ElemReference *> elem_references_;
 public:
  explicit ElemSelect(ElemReference *elem_reference)
      : SelectCall(SelectType::kElem), elem_references_({elem_reference}) {
  };
  explicit ElemSelect(std::vector<ElemReference *> elem_reference)
      : SelectCall(SelectType::kElem), elem_references_(std::move(elem_reference)) {
  };
  void setReferences(std::vector<ElemReference *> elem_references);
  [[nodiscard]] std::vector<ElemReference *> getReferences() const;
  [[nodiscard]] std::string toString() const override;
};

/**
 * Represents a selecting of a boolean in a PQL query.
 * Example: Select BOOLEAN such that Follows(s1, s2)
 */
class BooleanSelect : public SelectCall {
 public:
  BooleanSelect() : SelectCall(SelectType::kBoolean) {
  };
};
