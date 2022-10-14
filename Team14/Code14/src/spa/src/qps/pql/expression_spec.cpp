#include "expression_spec.h"

std::string ExpressionSpec::toString() const {
  return this->expr_;
}
bool ExactExpression::isWild() const {
  return false;
}
bool ExactExpression::operator==(const ExpressionSpec &other) const {
  return !other.isWild() && other.toString() == this->toString();
}
bool WildExpression::isWild() const {
  return true;
}
bool WildExpression::operator==(const ExpressionSpec &other) const {
  return other.isWild() && other.toString() == this->toString();
}

