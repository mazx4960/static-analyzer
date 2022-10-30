// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

#include "commons/parser/node/node.h"
#include "commons/parser/node/node_type.h"

class RelFactorNode;
class ExprNode;
class ReferenceNode;
class VariableNode;
class ConstantNode;

class RelFactorNode : public Node {
 public:
  explicit RelFactorNode(NodeType nodeType);
};

class ExprNode : public RelFactorNode {
 private:
  ExprType expr_type_;

  RelFactorNode *leftExpression_;

  RelFactorNode *rightExpression_;

 public:
  ExprNode(ExprType expr_type, RelFactorNode *leftExpression, RelFactorNode *rightExpression);
  std::vector<Node *> GetChildren() override;
  std::string ToString() override;
  ExprType GetExprType();
};

class PlusNode : public ExprNode {

 public:
  PlusNode(RelFactorNode *leftExpression, RelFactorNode *rightExpression)
      : ExprNode(ExprType::kPlus, leftExpression, rightExpression) {
  };
};

class MinusNode : public ExprNode {
 public:
  MinusNode(RelFactorNode *leftExpression, RelFactorNode *rightExpression)
      : ExprNode(ExprType::kMinus, leftExpression, rightExpression) {
  };
};

class TimesNode : public ExprNode {
 public:
  TimesNode(RelFactorNode *leftExpression, RelFactorNode *rightExpression)
      : ExprNode(ExprType::kTimes, leftExpression, rightExpression) {
  };
};

class DivNode : public ExprNode {
 public:
  DivNode(RelFactorNode *leftExpression, RelFactorNode *rightExpression)
      : ExprNode(ExprType::kDiv, leftExpression, rightExpression) {
  };
};

class ModNode : public ExprNode {
 public:
  ModNode(RelFactorNode *leftExpression, RelFactorNode *rightExpression)
      : ExprNode(ExprType::kMod, leftExpression, rightExpression) {
  };
};

class ReferenceNode : public RelFactorNode {
 public:
  explicit ReferenceNode(NodeType nodeType);
  std::vector<Node *> GetChildren() override;
};

class VariableNode : public ReferenceNode {
 private:
  std::string variableName_;

 public:
  explicit VariableNode(std::string variableName);
  std::string GetVariableName();
  std::string ToString() override;
};

class ConstantNode : public ReferenceNode {
 private:
  std::string value_;

 public:
  explicit ConstantNode(std::string value);
  [[nodiscard]] std::string GetValue() const;
  std::string ToString() override;
};
