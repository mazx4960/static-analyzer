// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>

#include "commons/relationship.h"
#include "pattern.h"

enum class ClauseType {
  kSuchThat,
  kPattern,
};

class QueryClause {
 protected:
  QueryClause(ClauseType clauseType, Relationship relationship) : clause_type_(std::move(clauseType)) {};
  QueryClause(ClauseType clauseType, Pattern pattern) : clause_type_(std::move(clauseType)) {};

  ClauseType clause_type_;

 public:
  ClauseType getClauseType();
};

class SuchThatClause : public QueryClause {
 public:
  explicit SuchThatClause(Relationship relationship) : QueryClause(ClauseType::kSuchThat, std::move(relationship)),
                                                       relationship_(relationship) {}

  Relationship relationship_;

  [[nodiscard]] Relationship getRelationship() const;
};

class PatternClause : public QueryClause {
 public:
  explicit PatternClause(Pattern pattern) : QueryClause(ClauseType::kPattern, std::move(pattern)), pattern_(pattern) {}

  Pattern pattern_;

  [[nodiscard]] Pattern getPattern() const;
};
