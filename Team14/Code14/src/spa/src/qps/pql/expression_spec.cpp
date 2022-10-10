#include "expression_spec.h"

std::string ExpressionSpec::getExpr() {
  return this->expr_;
}
bool ExactExpression::isWild() {
  return false;
}
bool WildExpression::isWild() {
  return true;
}

