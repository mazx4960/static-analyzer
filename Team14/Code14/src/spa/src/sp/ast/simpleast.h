#pragma once

#include <string>
#include <vector>

#include "simplenodetype.h"

class SimpleAstNode;
class ProgramNode;
class ProcedureNode;
class StatementListNode;
class StatementNode;
class ReadNode;
class PrintNode;
class CallNode;
class WhileNode;
class IfNode;
class AssignNode;
class CondExprNode;
class RelExprNode;
class RelFactorNode;
class ExprNode;
class ReferenceNode;
class VariableNode;
class ConstantNode;

class SimpleAstNode {
 private:
  SimpleNodeType nodeType_;

 public:
  explicit SimpleAstNode(SimpleNodeType nodeType);
  SimpleNodeType GetNodeType();
};

class ProgramNode : public SimpleAstNode {
 private:
  std::vector<ProcedureNode*> procedures_;

 public:
  explicit ProgramNode(std::vector<ProcedureNode*> procedures);
  std::vector<ProcedureNode*> GetProcedures();
};

class ProcedureNode : public SimpleAstNode {
 private:
  std::string procName_;
  StatementListNode* statementList_;

 public:
  ProcedureNode(std::string procName, StatementListNode* statementList);
  StatementListNode* GetStatementList();
};

class StatementListNode : public SimpleAstNode {
 private:
  std::vector<StatementNode*> statements_;

 public:
  explicit StatementListNode(std::vector<StatementNode*> statements);
  std::vector<StatementNode*> GetStatements();
};

class StatementNode : public SimpleAstNode {
 private:
  int stmtNo_;

 public:
  explicit StatementNode(SimpleNodeType nodeType);
};

class ReadNode : public StatementNode {
 private:
  VariableNode* variable_;

 public:
  explicit ReadNode(VariableNode* variable);
};

class PrintNode : public StatementNode {
 private:
  VariableNode* variable_;

 public:
  explicit PrintNode(VariableNode* variable);
};

class CallNode : public StatementNode {
 private:
  std::string procedureName_;

 public:
  explicit CallNode(std::string procedureName);
};

class WhileNode : public StatementNode {
 private:
  CondExprNode* conditional_;
  StatementListNode* statementList_;

 public:
  WhileNode(CondExprNode* conditional, StatementListNode* statementList);
};

class IfNode : public StatementNode {
 private:
  CondExprNode* conditional_;
  StatementListNode* thenStatementList_;
  StatementListNode* elseStatementList_;

 public:
  IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList);
};

class AssignNode : public StatementNode {
 private:
  VariableNode* variable_;
  ExprNode* expression_;

 public:
  AssignNode(VariableNode* variable, ExprNode* expression);
};

class CondExprNode : public SimpleAstNode {
 public:
  explicit CondExprNode(SimpleNodeType nodeType);
};

class NotExprNode : public CondExprNode {
 private:
  CondExprNode* negatedConditional_;

 public:
  explicit NotExprNode(CondExprNode* negatedConditional);
};

class AndExprNode : public CondExprNode {
 private:
  CondExprNode* firstConditional_;
  CondExprNode* secondConditional_;

 public:
  AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional);
};

class OrExprNode : public CondExprNode {
 private:
  CondExprNode* firstConditional_;
  CondExprNode* secondConditional_;

 public:
  OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional);
};

class RelExprNode : public CondExprNode {
 private:
  RelFactorNode* leftFactor_;
  RelFactorNode* rightFactor_;

 public:
  RelExprNode(SimpleNodeType nodeType, RelFactorNode* leftFactor, RelFactorNode* rightFactor);
};

class GreaterThanNode : public RelExprNode {
 public:
  GreaterThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor);
};

class GreaterThanEqualNode : public RelExprNode {
 public:
  GreaterThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor);
};

class LessThanNode : public RelExprNode {
 public:
  LessThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor);
};

class LessThanEqualNode : public RelExprNode {
 public:
  LessThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor);
};

class EqualNode : public RelExprNode {
 public:
  EqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor);
};

class NotEqualNode : public RelExprNode {
 public:
  NotEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor);
};

class RelFactorNode : public SimpleAstNode {
 public:
  explicit RelFactorNode(SimpleNodeType nodeType);
};

class ExprNode : public RelFactorNode {
 private:
  RelFactorNode* leftExpression_;
  RelFactorNode* rightExpression_;

 public:
  ExprNode(SimpleNodeType nodeType, RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class PlusNode : public ExprNode {

 public:
  PlusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class MinusNode : public ExprNode {
 public:
  MinusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class TimesNode : public ExprNode {
 public:
  TimesNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class DivNode : public ExprNode {
 public:
  DivNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class ModNode : public ExprNode {
 public:
  ModNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class ReferenceNode : public RelFactorNode {
 public:
  explicit ReferenceNode(SimpleNodeType nodeType);
};

class VariableNode : public ReferenceNode {
 private:
  std::string variableName_;

 public:
  explicit VariableNode(std::string variableName);
};

class ConstantNode : public ReferenceNode {
 private:
  int value_;

 public:
  explicit ConstantNode(int value);
};
