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
  [[nodiscard]] SelectType GetSelectType() const;
  [[nodiscard]] virtual std::string ToString() const;
  virtual bool operator==(const SelectCall &other) const = 0;
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
  explicit ElemSelect(std::vector<ElemReference *> elem_reference)
      : SelectCall(SelectType::kElem), elem_references_(std::move(elem_reference)) {
  };
  [[nodiscard]] std::vector<ElemReference *> GetReferences() const;
  [[nodiscard]] std::string ToString() const override;
  bool operator==(const SelectCall &other) const override;
};

/**
 * Represents a selecting of a boolean in a PQL query.
 * Example: Select BOOLEAN such that Follows(s1, s2)
 */
class BooleanSelect : public SelectCall {
 public:
  BooleanSelect() : SelectCall(SelectType::kBoolean) {
  };
  bool operator==(const SelectCall &other) const override;
};
