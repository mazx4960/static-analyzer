// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_clause.h"

ClauseType QueryClause::getClauseType() {
  return this->clause_type_;
}
RsType SuchThatClause::getSuchThatType() const {
  return this->type_;
}
QueryDeclaration *SuchThatClause::getFirst() const {
  return this->first_;
}
QueryDeclaration *SuchThatClause::getSecond() const {
  return this->second_;
}

RsType PatternClause::getPatternType() const {
  return this->type_;
}
QueryDeclaration *PatternClause::getFirst() const {
  return this->first_;
}
QueryDeclaration *PatternClause::getSecond() const {
  return this->second_;
}
QueryDeclaration *PatternClause::getThird() const {
  return this->third_;
}
