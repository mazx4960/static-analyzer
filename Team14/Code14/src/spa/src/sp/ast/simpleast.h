#pragma once

#include <vector>
#include <string>
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
class TermNode;
class FactorNode;
class ReferenceNode;
class VariableNode;
class ConstantNode;


class SimpleAstNode {
 private:
  SimpleNodeType nodeType;

 public:
  SimpleAstNode(SimpleNodeType nodeType);
};

class ProgramNode : public SimpleAstNode {
 private:
  std::vector<ProcedureNode*> procedures;

 public:
  ProgramNode(std::vector<ProcedureNode*> procedures);
};

class ProcedureNode : public SimpleAstNode {
 private:
  std::string procName;
  StatementListNode* statementList;

 public:
  ProcedureNode(std::string procName, StatementListNode* statementList);
};

class StatementListNode : public SimpleAstNode {
 private:
  std::vector<StatementNode*> statements;

 public:
  StatementListNode(std::vector<StatementNode*> statements);
};

class StatementNode : public SimpleAstNode {
 private:
  int stmtNo;

 public:
  StatementNode(SimpleNodeType nodeType);
};

class ReadNode : public StatementNode {
 private:
  VariableNode* variable;

 public:
  ReadNode(VariableNode* variable);
};

class PrintNode : public StatementNode {
 private:
  VariableNode* variable;

 public:
  PrintNode(VariableNode* variable);
};

class CallNode : public StatementNode {
 private:
  std::string procedureName;

 public:
  CallNode(std::string procedureName);
};

class WhileNode : public StatementNode {
 private:
  CondExprNode* conditional;
  StatementListNode* statementList;

 public:
  WhileNode(CondExprNode* conditional, StatementListNode* statementList);
};

class IfNode : public StatementNode {
 private:
  CondExprNode* conditional;
  StatementListNode* thenStatementList;
  StatementListNode* elseStatementList;

 public:
  IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList);
};

class AssignNode : public StatementNode {
 private:
  VariableNode* variable;
  ExprNode* expression;

 public:
  AssignNode(VariableNode* variable, ExprNode* expression);
};

class CondExprNode : public SimpleAstNode {
 public:
  CondExprNode(SimpleNodeType nodeType);
};

class NotExprNode : public CondExprNode {
 private:
  CondExprNode* negatedConditional;

 public:
  NotExprNode(CondExprNode* negatedConditional);
};

class AndExprNode : public CondExprNode {
 private:
  CondExprNode* firstConditional;
  CondExprNode* secondConditional;

 public:
  AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional);
};

class OrExprNode : public CondExprNode {
 private:
  CondExprNode* firstConditional;
  CondExprNode* secondConditional;

 public:
  OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional);
};

class RelExprNode : public CondExprNode {
 private:
  RelFactorNode* leftFactor;
  RelFactorNode* rightFactor;

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
  RelFactorNode(SimpleNodeType nodeType);
};

class ExprNode : public RelFactorNode {
 private:
  RelFactorNode* leftExpression;
  RelFactorNode* rightExpression;

 public:
  ExprNode(SimpleNodeType nodeType, RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class PlusNode : public ExprNode {

 public:
  PlusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression);
};

class MinusNode : public ExprNode {
 public:;
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
  ReferenceNode(SimpleNodeType nodeType);
};

class VariableNode : public ReferenceNode {
 private:
  std::string variableName;

 public:
  VariableNode(std::string variableName);
};

class ConstantNode : public ReferenceNode {
 private:
  int value;

 public:
  ConstantNode(int value);
};