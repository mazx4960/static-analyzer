// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_clause.h"

ClauseType QueryClause::getClauseType() {
  return this->clause_type_;
}
Relationship SuchThatClause::getRelationship() const {
  return this->relationship_;
}
Pattern PatternClause::getPattern() const {
  return this->pattern_;
}