// Copyright 2022 CS3203 Team14. All rights reserved.

#include "expr_node.h"

RelFactorNode::RelFactorNode(NodeType nodeType) : Node(nodeType) {
}

ExprNode::ExprNode(ExprType expr_type, RelFactorNode *leftExpression, RelFactorNode *rightExpression)
    : RelFactorNode(NodeType::kExpr),
      expr_type_(expr_type),
      leftExpression_(leftExpression),
      rightExpression_(rightExpression) {
}
std::vector<Node *> ExprNode::GetChildren() {
  return std::vector<Node *>{leftExpression_, rightExpression_};
}
std::string ExprNode::ToString() {
  if (this->expr_type_ == ExprType::kPlus) {
    return "(" + leftExpression_->ToString() + "+" + rightExpression_->ToString() + ")";
  }
  if (this->expr_type_ == ExprType::kMinus) {
    return "(" + leftExpression_->ToString() + "-" + rightExpression_->ToString() + ")";
  }
  if (this->expr_type_ == ExprType::kTimes) {
    return "(" + leftExpression_->ToString() + "*" + rightExpression_->ToString() + ")";
  }
  if (this->expr_type_ == ExprType::kDiv) {
    return "(" + leftExpression_->ToString() + "/" + rightExpression_->ToString() + ")";
  }
  if (this->expr_type_ == ExprType::kMod) {
    return "(" + leftExpression_->ToString() + "%" + rightExpression_->ToString() + ")";
  }
  return "";
}
ExprType ExprNode::GetExprType() {
  return expr_type_;
}
ReferenceNode::ReferenceNode(NodeType nodeType) : RelFactorNode(nodeType) {
}
std::vector<Node *> ReferenceNode::GetChildren() {
  return std::vector<Node *>{};
}

VariableNode::VariableNode(std::string variableName)
    : ReferenceNode(NodeType::kVariable),
      variableName_(std::move(variableName)) {
}
std::string VariableNode::GetVariableName() {
  return this->variableName_;
}
std::string VariableNode::ToString() {
  return "(" + this->variableName_ + ")";
}

ConstantNode::ConstantNode(int64_t value) : ReferenceNode(NodeType::kConstant), value_(value) {
}
int64_t ConstantNode::GetValue() const {
  return this->value_;
}
std::string ConstantNode::ToString() {
  return "(" + std::to_string(this->value_) + ")";
}
