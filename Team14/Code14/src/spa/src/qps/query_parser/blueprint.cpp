#include "blueprint.h"
#include "commons/helper/vector_comparator.h"

ReferenceType ReferenceBlueprint::getReferenceType() const {
  return this->reference_type_;
}
std::string ReferenceBlueprint::getValue() const {
  return this->value_;
}
std::string ReferenceBlueprint::toString() const {
  return RefTypeToString(this->reference_type_) + ": " + this->value_;
}
bool ReferenceBlueprint::operator==(const ReferenceBlueprint &other) const {
  return this->getReferenceType() == other.getReferenceType() && this->getValue() == other.getValue();
}
bool ExprBlueprint::isExact() const {
  return this->is_exact_;
}
bool ExprBlueprint::operator==(const ReferenceBlueprint &other) const {
  return this->getReferenceType() == other.getReferenceType() && this->getValue() == other.getValue()
      && this->isExact() == static_cast<const ExprBlueprint &>(other).isExact();
}
AttributeType ElemBlueprint::getAttributeType() const {
  return this->attribute_type_;
}
bool ElemBlueprint::operator==(const ReferenceBlueprint &other) const {
  return this->getReferenceType() == other.getReferenceType() && this->getValue() == other.getValue()
      && this->getAttributeType() == static_cast<const ElemBlueprint &>(other).getAttributeType();
}
std::string DeclarationBlueprint::getName() const {
  return synonym_->getValue();
}
EntityType DeclarationBlueprint::getEntityType() const {
  return this->entity_type_;
}
bool DeclarationBlueprint::operator==(const DeclarationBlueprint &other) const {
  return this->getEntityType() == other.getEntityType() && this->getName() == other.getName();
}
std::vector<ElemBlueprint *> SelectBlueprint::getBlueprintReferences() const {
  return this->blueprint_references_;
}
ElemBlueprint *SelectBlueprint::getSingleReference() const {
  return this->single_reference_;
}
bool SelectBlueprint::operator==(const SelectBlueprint &other) const {
  auto this_references = this->getBlueprintReferences();
  auto other_references = other.getBlueprintReferences();
  return VectorComparator<ElemBlueprint>::CompareVectors(this_references, other_references)
      && (*this->getSingleReference()) == (*other.getSingleReference());
}
RsType SuchThatBlueprint::getRsType() const {
  return this->rs_type_;
}
ReferenceBlueprint *SuchThatBlueprint::getFirst() const {
  return this->first_;
}
ReferenceBlueprint *SuchThatBlueprint::getSecond() const {
  return this->second_;
}
bool SuchThatBlueprint::checkSyntax() const {
  return CheckSuchThat(this->rs_type_, this->first_->getReferenceType(), this->second_->getReferenceType());
}
std::string SuchThatBlueprint::toString() const {
  return "SuchThatBlueprint: " + RsTypeToString(rs_type_) + " " + first_->toString() + " " + second_->toString();
}
bool SuchThatBlueprint::operator==(const ClauseBlueprint &other) const {
  return this->getClauseType() == other.getClauseType()
      && *this->getFirst() == *static_cast<const SuchThatBlueprint &>(other).getFirst()
      && *this->getSecond() == *static_cast<const SuchThatBlueprint &>(other).getSecond()
      && this->getRsType() == static_cast<const SuchThatBlueprint &>(other).getRsType();
}
SynonymBlueprint *PatternBlueprint::getStmt() const {
  return this->stmt_;
}
ReferenceBlueprint *PatternBlueprint::getVar() const {
  return this->var_;
}
ExprBlueprint *PatternBlueprint::getExpr() const {
  return this->expr_;
}
ExprBlueprint *PatternBlueprint::getExpr2() const {
  return this->expr_2_;
}
bool PatternBlueprint::checkSyntax() const {
  return CheckPattern(this->var_->getReferenceType());
}
std::string PatternBlueprint::toString() const {
  return "PatternBlueprint: " + stmt_->toString() + " " + var_->toString() + " " + expr_->toString();
}
bool PatternBlueprint::operator==(const ClauseBlueprint &other) const {
  return this->getClauseType() == other.getClauseType()
      && *this->getStmt() == *static_cast<const PatternBlueprint &>(other).getStmt()
      && *this->getVar() == *static_cast<const PatternBlueprint &>(other).getVar()
      && *this->getExpr() == *static_cast<const PatternBlueprint &>(other).getExpr()
      && *this->getExpr2() == *static_cast<const PatternBlueprint &>(other).getExpr2();
}
Comparator WithBlueprint::getComparator() const {
  return this->comparator_;
};
ReferenceBlueprint *WithBlueprint::getFirst() const {
  return this->first_;
};
ReferenceBlueprint *WithBlueprint::getSecond() const {
  return this->second_;
};
bool WithBlueprint::checkSyntax() const {
  return CheckWith(this->first_->getReferenceType(), this->second_->getReferenceType());
}
std::string WithBlueprint::toString() const {
  return "WithBlueprint: " + first_->toString() + " " + second_->toString();
}
bool WithBlueprint::operator==(const ClauseBlueprint &other) const {
  return this->getClauseType() == other.getClauseType()
      && *this->getFirst() == *static_cast<const WithBlueprint &>(other).getFirst()
      && *this->getSecond() == *static_cast<const WithBlueprint &>(other).getSecond()
      && this->getComparator() == static_cast<const WithBlueprint &>(other).getComparator();
};
bool SynonymBlueprint::operator==(const ReferenceBlueprint &other) const {
  return this->getReferenceType() == other.getReferenceType() && this->getValue() == other.getValue();
}
