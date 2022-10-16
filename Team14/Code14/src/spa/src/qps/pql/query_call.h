// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>

#include "query_clause.h"
#include "query_reference.h"

enum class CallType {
  kSelect,
};

class QueryCall {
 private:
  CallType type_;
  std::vector<ElemReference *> elem_references_;
 public:
  QueryCall(CallType call_type, ElemReference *elem_reference)
      : type_(call_type), elem_references_({std::move(elem_reference)}) {
  };
  QueryCall(CallType call_type, std::vector<ElemReference *> elem_reference)
      : type_(call_type), elem_references_(std::move(elem_reference)) {
  };

  void setReferences(std::vector<ElemReference *> synonym_references);
  [[nodiscard]] CallType getCallType() const;
  [[nodiscard]] std::vector<ElemReference *> getReferences() const;
  [[nodiscard]] virtual std::string toString() const = 0;
};

class SelectCall : public QueryCall {
 public:
  explicit SelectCall(ElemReference *elem_reference)
      : QueryCall(CallType::kSelect, std::move(elem_reference)) {
  };
  explicit SelectCall(std::vector<ElemReference *> elem_reference)
      : QueryCall(CallType::kSelect, std::move(elem_reference)) {
  };
  [[nodiscard]] std::string toString() const override;
};
