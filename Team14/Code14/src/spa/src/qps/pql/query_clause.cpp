// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_clause.h"
#include "spdlog/spdlog.h"

ClauseType QueryClause::getClauseType() const {
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
std::string SuchThatClause::toString() const {
  std::string str;
  str.append(RsTypeToString(this->getSuchThatType()) + "(" + this->getFirst()->toString() + ", "
                 + this->getSecond()->toString() + ")");
  return str;
}
bool SuchThatClause::operator==(const QueryClause &other) const {

  return other.getClauseType() == this->getClauseType()
      && (static_cast<const SuchThatClause &>(other).getSuchThatType()) == (this->getSuchThatType())
      && (*static_cast<const SuchThatClause &>(other).getFirst()) == (*this->getFirst())
      && (*static_cast<const SuchThatClause &>(other).getSecond()) == (*this->getSecond());
}

bool ParentClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool ParentClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool ParentTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool ParentTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool FollowsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool FollowsClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool FollowsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool FollowsTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kReadStmt:
        case EntityType::kPrintStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool UsesClause::isSyntacticallyCorrect() const {
  return (this->getFirst()->isStmtRef() || this->getFirst()->isEntRef()) && this->getSecond()->isEntRef();
}
bool UsesClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kAssignStmt:
        case EntityType::kProcedure:
        case EntityType::kStatement:
        case EntityType::kCallStmt:
        case EntityType::kIfStmt:
        case EntityType::kWhileStmt:
        case EntityType::kPrintStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kVariable);
      break;
    case ReferenceType::kSynonym:
      return getSecond()->getEntityType() == EntityType::kVariable;
    default:
      return false;
  }
  return true;
}
bool ModifiesClause::isSyntacticallyCorrect() const {
  return (this->getFirst()->isStmtRef() || this->getFirst()->isEntRef()) && this->getSecond()->isEntRef();
}
bool ModifiesClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kAssignStmt:
        case EntityType::kProcedure:
        case EntityType::kStatement:
        case EntityType::kCallStmt:
        case EntityType::kIfStmt:
        case EntityType::kWhileStmt:
        case EntityType::kReadStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kVariable);
      break;
    case ReferenceType::kSynonym:
      return getSecond()->getEntityType() == EntityType::kVariable;
    default:
      return false;
  }
  return true;
}
bool CallsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isEntRef() && this->getSecond()->isEntRef();
}
bool CallsClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kProcedure:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kProcedure:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool CallsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isEntRef() && this->getSecond()->isEntRef();
}
bool CallsTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kProcedure:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kProcedure:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool NextClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool NextClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool NextTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool NextTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kPrintStmt:
        case EntityType::kReadStmt:
        case EntityType::kAssignStmt:
        case EntityType::kCallStmt:
        case EntityType::kWhileStmt:
        case EntityType::kIfStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool AffectsClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool AffectsClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kAssignStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kAssignStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return true;
}
bool AffectsTClause::isSyntacticallyCorrect() const {
  return this->getFirst()->isStmtRef() && this->getSecond()->isStmtRef();
}
bool AffectsTClause::IsSemanticallyCorrect() const {
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getFirst()->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (getFirst()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kAssignStmt:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      getSecond()->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (getSecond()->getEntityType()) {
        case EntityType::kStatement:
        case EntityType::kAssignStmt:
          break;
        default:
          return false;
      }
      break;
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
void PatternClause::setSynonymReference(SynonymReference *synonym_declaration) {
  this->synonym_declaration_ = synonym_declaration;
}
void PatternClause::setEntReference(QueryReference *ent_ref) {
  this->ent_ref_ = ent_ref;
}
bool PatternClause::isSyntacticallyCorrect() const {
  return true;
}
bool PatternClause::IsSemanticallyCorrect() const {
  switch (getEntRef()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      getEntRef()->setEntityType(EntityType::kVariable);
      break;
    case ReferenceType::kSynonym:
      switch (getEntRef()->getEntityType()) {
        case EntityType::kVariable:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  return getSynonymDeclaration()->getEntityType() == EntityType::kAssignStmt;
}
std::string PatternClause::toString() const {
  std::string str;
  str.append("pattern " + this->synonym_declaration_->toString() + "(" + this->getEntRef()->toString() + ", "
                 + this->expression_->toString() + ")");
  return str;
}
bool PatternClause::operator==(const QueryClause &other) const {
  if (other.getClauseType() != this->getClauseType()) {
    return false;
  }
  auto other_clause = static_cast<const PatternClause &>(other);
  return (*other_clause.getSynonymDeclaration()) == (*this->getSynonymDeclaration())
      && (*other_clause.getEntRef()) == (*this->getEntRef())
      && (*other_clause.getExpression()) == (*this->getExpression());
}
Comparator WithClause::getComparator() const {
  return this->comparator_;
}
QueryReference *WithClause::getFirst() const {
  return this->first_;
}
QueryReference *WithClause::getSecond() const {
  return this->second_;
}
bool WithClause::isSyntacticallyCorrect() const {
  return getFirst()->isAttrCompareRef() && getSecond()->isAttrCompareRef();
}
bool WithClause::IsSemanticallyCorrect() const {
  ReferenceType first_type = getFirst()->getRefType();
  ReferenceType second_type = getSecond()->getRefType();
  switch (first_type) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kAttr:
      first_type = AttrToRefType(static_cast<AttrReference *>(getFirst())->getAttribute());
      break;
    default:
      return false;
  }
  switch (second_type) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kAttr:
      second_type = AttrToRefType(static_cast<AttrReference *>(getSecond())->getAttribute());
      break;
    default:
      return false;
  }
  return first_type == second_type;
}
std::string WithClause::toString() const {
  std::string str;
  str.append("with " + this->getFirst()->toString() + " = " + this->getSecond()->toString());
  return str;
}
bool WithClause::operator==(const QueryClause &other) const {
  if (other.getClauseType() != this->getClauseType()) {
    return false;
  }
  auto other_clause = static_cast<const WithClause &>(other);
  return (*other_clause.getFirst()) == (*this->getFirst()) && (*other_clause.getSecond()) == (*this->getSecond())
      && other_clause.getComparator() == this->getComparator();
}
void WithClause::setFirst(AttrReference *elem_reference) {
  this->first_ = elem_reference;
}
void WithClause::setSecond(AttrReference *elem_reference) {
  this->second_ = elem_reference;
}
