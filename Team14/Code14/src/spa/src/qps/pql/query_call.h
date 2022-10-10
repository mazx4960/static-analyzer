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

  SynonymReference *synonym_reference_;
 public:
  QueryCall(CallType call_type, SynonymReference *synonym_reference)
      : type_(call_type), synonym_reference_(std::move(synonym_reference)) {
  };

  [[nodiscard]] SynonymReference *getReference() const;
  [[nodiscard]] void setReference(SynonymReference *synonym_reference);
};

class SelectCall : public QueryCall {
 public:
  explicit SelectCall(SynonymReference *query_declaration)
      : QueryCall(CallType::kSelect, std::move(query_declaration)) {
  };
};
