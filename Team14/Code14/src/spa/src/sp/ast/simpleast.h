#pragma once

#include <vector>
#include <string>

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
class ExprNode;
class TermNode;
class FactorNode;
class ReferenceNode;
class VariableNode;
class ConstantNode;


class SimpleAstNode {};

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
  StatementNode();
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

class CondExprNode : public SimpleAstNode {};

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

class RelExprNode : public CondExprNode {};

class GreaterThanNode : public RelExprNode {
 private:
  ExprNode* leftFactor;
  ExprNode* rightFactor;

 public:
  GreaterThanNode(ExprNode* leftFactor, ExprNode* rightFactor);
};

class GreaterThanEqualNode : public RelExprNode {
 private:
  ExprNode* leftFactor;
  ExprNode* rightFactor;

 public:
  GreaterThanEqualNode(ExprNode* leftFactor, ExprNode* rightFactor);
};

class LessThanNode : public RelExprNode {
 private:
  ExprNode* leftFactor;
  ExprNode* rightFactor;

 public:
  LessThanNode(ExprNode* leftFactor, ExprNode* rightFactor);
};

class LessThanEqualNode : public RelExprNode {
 private:
  ExprNode* leftFactor;
  ExprNode* rightFactor;

 public:
  LessThanEqualNode(ExprNode* leftFactor, ExprNode* rightFactor);
};

class EqualNode : public RelExprNode {
 private:
  ExprNode* leftFactor;
  ExprNode* rightFactor;

 public:
  EqualNode(ExprNode* leftFactor, ExprNode* rightFactor);
};

class NotEqualNode : public RelExprNode {
 private:
  ExprNode* leftFactor;
  ExprNode* rightFactor;

 public:
  NotEqualNode(ExprNode* leftFactor, ExprNode* rightFactor);
};

class ExprNode : public SimpleAstNode {};

class PlusNode : public ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;

 public:
  PlusNode(ExprNode* leftExpression, ExprNode* rightExpression);
};

class MinusNode : public ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;

 public:;
  MinusNode(ExprNode* leftExpression, ExprNode* rightExpression);
};

class TimesNode : public ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;

 public:
  TimesNode(ExprNode* leftExpression, ExprNode* rightExpression);
};

class DivNode : public ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;

 public:
  DivNode(ExprNode* leftExpression, ExprNode* rightExpression);
};

class ModNode : public ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;

 public:
  ModNode(ExprNode* leftExpression, ExprNode* rightExpression);
};

class ReferenceNode : public SimpleAstNode {};

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