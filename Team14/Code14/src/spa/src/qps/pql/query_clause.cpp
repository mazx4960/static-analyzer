// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_clause.h"

ClauseType QueryClause::getClauseType() {
  return this->clause_type_;
}
RsType SuchThatClause::getSuchThatType() const {
  return this->type_;
}
QueryReference *SuchThatClause::getFirst() const {
  return this->first_;
}
QueryReference *SuchThatClause::getSecond() const {
  return this->second_;
}
void SuchThatClause::setFirst(SynonymReference *synonym_declaration) {
  this->first_ = synonym_declaration;
}
void SuchThatClause::setSecond(SynonymReference *synonym_declaration) {
  this->second_ = synonym_declaration;
}
bool ParentClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool ParentTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool FollowsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool FollowsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool UsesClause::isSyntacticallyCorrect() const {
  return (this->getFirst()->isStmtRef() || this->getFirst()->isEntRef()) && this->getSecond()->isEntRef();
}
bool ModifiesClause::isSyntacticallyCorrect() const {
  return (this->getFirst()->isStmtRef() || this->getFirst()->isEntRef()) && this->getSecond()->isEntRef();
}
bool CallsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isEntRef() && this->getSecond()->isEntRef();
}
bool CallsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isEntRef() && this->getSecond()->isEntRef();
}
bool NextClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool NextTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool AffectsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool AffectsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}


SynonymReference *PatternClause::getSynonymDeclaration() const {
  return this->synonym_declaration_;
}
QueryReference *PatternClause::getEntRef() const {
  return this->ent_ref_;
}
ExpressionSpec *PatternClause::getExpression() const {
  return this->expression_;
}
void PatternClause::setSynonymDeclaration(SynonymReference *synonym_declaration) {
  this->synonym_declaration_ = synonym_declaration;
}
bool PatternClause::isSyntacticallyCorrect() const {
  return true;
}
