#include "blueprint.h"

ReferenceType BaseBlueprint::getReferenceType() const {
  return this->reference_type_;
}
std::string BaseBlueprint::getValue() const {
  return this->value_;
}
std::string BaseBlueprint::toString() const {
  return RefTypeToString(this->reference_type_) + ": " + this->value_;
}
bool ExprBlueprint::isExact() const {
  return this->is_exact_;
}
AttributeType ElemBlueprint::getAttributeType() const {
  return this->attribute_type_;
}
std::string DeclarationBlueprint::getName() const {
  return synonym_->getValue();
}
EntityType DeclarationBlueprint::getEntityType() const {
  return this->entity_type_;
}
std::vector<ElemBlueprint *> SelectBlueprint::getBlueprintReferences() const {
  return this->blueprint_references_;
}
RsType SuchThatBlueprint::getRsType() const {
  return this->rs_type_;
}
BaseBlueprint *SuchThatBlueprint::getFirst() const {
  return this->first_;
}
BaseBlueprint *SuchThatBlueprint::getSecond() const {
  return this->second_;
}
bool SuchThatBlueprint::checkSyntax() const {
  return CheckSuchThat(this->rs_type_, this->first_->getReferenceType(), this->second_->getReferenceType());
}
std::string SuchThatBlueprint::toString() const {
  return "SuchThatBlueprint: " + RsTypeToString(rs_type_) + " " + first_->toString() + " " + second_->toString();
}
SynonymBlueprint *PatternBlueprint::getStmt() const {
  return this->stmt_;
}
BaseBlueprint *PatternBlueprint::getVar() const {
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
Comparator WithBlueprint::getComparator() const {
  return this->comparator_;
};
BaseBlueprint *WithBlueprint::getFirst() const {
  return this->first_;
};
BaseBlueprint *WithBlueprint::getSecond() const {
  return this->second_;
};
bool WithBlueprint::checkSyntax() const {
  return CheckWith(this->first_->getReferenceType(), this->second_->getReferenceType());
}
std::string WithBlueprint::toString() const {
  return "WithBlueprint: " + first_->toString() + " " + second_->toString();
};
