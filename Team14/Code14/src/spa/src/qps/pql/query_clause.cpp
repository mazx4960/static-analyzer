// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_clause.h"
#include "qps/exceptions.h"

ClauseType QueryClause::getClauseType() const {
  return this->clause_type_;
}
SuchThatClause *SuchThatClause::Create(RsType type, QueryReference *first, QueryReference *second) {
  switch (type) {
    case RsType::kParent:
      return new ParentClause(first, second);
    case RsType::kFollows:
      return new FollowsClause(first, second);
    case RsType::kModifies:
      return new ModifiesClause(first, second);
    case RsType::kUses:
      return new UsesClause(first, second);
    case RsType::kCalls:
      return new CallsClause(first, second);
    case RsType::kCallsT:
      return new CallsTClause(first, second);
    case RsType::kNext:
      return new NextClause(first, second);
    case RsType::kNextT:
      return new NextTClause(first, second);
    case RsType::kAffects:
      return new AffectsClause(first, second);
    case RsType::kAffectsT:
      return new AffectsTClause(first, second);
    case RsType::kParentT:
      return new ParentTClause(first, second);
    case RsType::kFollowsT:
      return new FollowsTClause(first, second);
    default:
      throw BuilderError("Invalid such that clause type");
  }
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

bool ParentClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (first->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (second->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool ParentTClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool FollowsClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool FollowsTClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool UsesClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kVariable);
      break;
    case ReferenceType::kSynonym:
      return static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType() == EntityType::kVariable;
    default:
      return false;
  }
  return true;
}
bool ModifiesClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kVariable);
      break;
    case ReferenceType::kSynonym:
      return static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType() == EntityType::kVariable;
    default:
      return false;
  }
  return true;
}
bool CallsClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
        case EntityType::kProcedure:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool CallsTClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
        case EntityType::kProcedure:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kProcedure);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool NextClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool NextTClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kStatement);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool AffectsClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
bool AffectsTClause::isSemanticallyCorrect() const {
  auto *first = this->getFirst();
  switch (getFirst()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(first)->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(first)->getSynonym()->GetEntityType()) {
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
  auto *second = this->getSecond();
  switch (getSecond()->getRefType()) {
    case ReferenceType::kInteger:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(second)->setEntityType(EntityType::kAssignStmt);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(second)->getSynonym()->GetEntityType()) {
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
SynonymReference *PatternClause::getStmtRef() const {
  return this->stmt_ref_;
}
QueryReference *PatternClause::getEntRef() const {
  return this->ent_ref_;
}
ExpressionSpec *PatternClause::getFirstExpression() const {
  return this->first_expression_;
}
ExpressionSpec *PatternClause::getSecondExpression() const {
  return this->second_expression_;
}
void PatternClause::setSynonymReference(SynonymReference *stmt_ref) {
  this->stmt_ref_ = stmt_ref;
}
void PatternClause::setEntReference(QueryReference *ent_ref) {
  this->ent_ref_ = ent_ref;
}
bool PatternClause::isSemanticallyCorrect() const {
  auto *ent_ref = this->getEntRef();
  switch (getEntRef()->getRefType()) {
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kWildcard:
      static_cast<WildcardReference *>(ent_ref)->setEntityType(EntityType::kVariable);
      break;
    case ReferenceType::kSynonym:
      switch (static_cast<SynonymReference *>(ent_ref)->getSynonym()->GetEntityType()) {
        case EntityType::kVariable:
          break;
        default:
          return false;
      }
      break;
    default:
      return false;
  }
  auto *stmt_ref = this->getStmtRef();
  switch (static_cast<SynonymReference *>(stmt_ref)->getSynonym()->GetEntityType()) {
    case EntityType::kAssignStmt:
      return getFirstExpression() != nullptr && getSecondExpression() == nullptr;
    case EntityType::kIfStmt:
      return getFirstExpression() != nullptr && getSecondExpression() != nullptr
          && *getFirstExpression() == WildExpression() && *getSecondExpression() == WildExpression();
    case EntityType::kWhileStmt:
      return getFirstExpression() != nullptr && getSecondExpression() == nullptr
          && *getFirstExpression() == WildExpression();
    default:
      return false;
  }
}
std::string PatternClause::toString() const {
  std::string str;
  str.append("pattern " + this->stmt_ref_->toString() + "(" + this->getEntRef()->toString() + ", "
                 + this->first_expression_->toString() + ")");
  return str;
}
bool PatternClause::operator==(const QueryClause &other) const {
  if (other.getClauseType() != this->getClauseType()) {
    return false;
  }
  auto other_clause = static_cast<const PatternClause &>(other);
  return (*other_clause.getStmtRef()) == (*this->getStmtRef()) && (*other_clause.getEntRef()) == (*this->getEntRef())
      && (*other_clause.getFirstExpression()) == (*this->getFirstExpression());
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
bool WithClause::isSemanticallyCorrect() const {
  auto first_type = getFirst()->getRefType();
  auto second_type = getSecond()->getRefType();
  switch (first_type) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kAttr:
      first_type = AttrToRefType(static_cast<AttrReference *>(getFirst())->getAttributeType());
      if (!static_cast<AttrReference *>(getFirst())->isSemanticallyCorrect()) {
        return false;
      }
      break;
    default:
      return false;
  }
  switch (second_type) {
    case ReferenceType::kInteger:
    case ReferenceType::kIdent:
      break;
    case ReferenceType::kAttr:
      second_type = AttrToRefType(static_cast<AttrReference *>(getSecond())->getAttributeType());
      if (!static_cast<AttrReference *>(getSecond())->isSemanticallyCorrect()) {
        return false;
      }
      break;
    default:
      return false;
  }

  return first_type == second_type && first_type != ReferenceType::kWildcard;
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
