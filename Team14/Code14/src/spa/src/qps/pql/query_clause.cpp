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
bool ParentClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  return true;
}
bool ParentTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool ParentTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  return true;
}
bool FollowsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool FollowsClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  return true;
}
bool FollowsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool FollowsTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  return true;
}
bool UsesClause::isSyntacticallyCorrect() const {
  return (this->getFirst()->isStmtRef() || this->getFirst()->isEntRef()) && this->getSecond()->isEntRef();
}
bool UsesClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kAssignStmt:
    case EntityType::kProcedure:
    case EntityType::kStatement:
    case EntityType::kCallStmt:
    case EntityType::kIfStmt:
    case EntityType::kWhileStmt:
    case EntityType::kPrintStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kVariable:break;
    default:
      return false;
  }
  return true;
}
bool ModifiesClause::isSyntacticallyCorrect() const {
  return (this->getFirst()->isStmtRef() || this->getFirst()->isEntRef()) && this->getSecond()->isEntRef();
}
bool ModifiesClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kAssignStmt:
    case EntityType::kProcedure:
    case EntityType::kStatement:
    case EntityType::kCallStmt:
    case EntityType::kIfStmt:
    case EntityType::kWhileStmt:
    case EntityType::kReadStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kVariable:break;
    default:
      return false;
  }
  return true;
}
bool CallsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isEntRef() && this->getSecond()->isEntRef();
}
bool CallsClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kProcedure:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kProcedure:break;
    default:
      return false;
  }
  return true;
}
bool CallsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isEntRef() && this->getSecond()->isEntRef();
}
bool CallsTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kProcedure:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kProcedure:break;
    default:
      return false;
  }
  return true;
}
bool NextClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool NextClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  return true;
}
bool NextTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool NextTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kPrintStmt:
    case EntityType::kAssignStmt:
    case EntityType::kCallStmt:
    case EntityType::kWhileStmt:
    case EntityType::kIfStmt:break;
    default:
      return false;
  }
  return true;
}
bool AffectsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool AffectsClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kAssignStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kAssignStmt:break;
    default:
      return false;
  }
  return true;
}
bool AffectsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool AffectsTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kAssignStmt:break;
    default:
      return false;
  }
  switch (getSecond()->getEntityType()) {
    case EntityType::kWildcard:
    case EntityType::kStatement:
    case EntityType::kAssignStmt:break;
    default:
      return false;
  }
  return true;
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
bool PatternClause::IsSemanticallyCorrect() const {
  return true;
}
