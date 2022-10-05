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
void SuchThatClause::setFirst(SynonymDeclaration *synonym_declaration) {
  this->first_ = synonym_declaration;
}
void SuchThatClause::setSecond(SynonymDeclaration *synonym_declaration) {
  this->second_ = synonym_declaration;
}

RsType PatternClause::getPatternType() const {
  return this->type_;
}
SynonymDeclaration *PatternClause::getSynonymDeclaration() const {
  return this->synonym_declaration_;
}
QueryDeclaration *PatternClause::getEntRef() const {
  return this->ent_ref_;
}
StaticDeclaration *PatternClause::getExpression() const {
  return this->expression_;
}
void PatternClause::setSynonymDeclaration(SynonymDeclaration *synonym_declaration) {
  this->synonym_declaration_ = synonym_declaration;
}
