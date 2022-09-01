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

class ProgramNode : SimpleAstNode {
 private:
  std::vector<ProcedureNode*> procedures;
};

class ProcedureNode : SimpleAstNode {
 private:
  std::string procName;
  StatementListNode statementList;
};

class StatementListNode : SimpleAstNode {
 private:
  std::vector<StatementNode*> statements;
};

class StatementNode : SimpleAstNode {
 private:
  int stmtNo;
};

class ReadNode : StatementNode {
 private:
  VariableNode* variable;
};

class PrintNode : StatementNode {
 private:
  VariableNode* variable;
};

class CallNode : StatementNode {
 private:
  std::string procedureName;
};

class WhileNode : StatementNode {
 private:
  CondExprNode* conditional;
  StatementListNode* statementList;
};

class IfNode : StatementNode {
 private:
  CondExprNode* conditional;
  StatementListNode* thenStatementList;
  StatementListNode* elseStatementList;
};

class AssignNode : StatementNode {
 private:
  std::string variableName;
  ExprNode* expression;
};

class CondExprNode : SimpleAstNode {};

class NotExprNode : CondExprNode {
 private:
  CondExprNode* negatedConditional;
};

class AndExprNode : CondExprNode {
 private:
  CondExprNode* firstConditional;
  CondExprNode* secondConditional;
};

class OrExprNode : CondExprNode {
 private:
  CondExprNode* firstConditional;
  CondExprNode* secondConditional;
};

class RelExprNode : CondExprNode {};

class GreaterThanNode : RelExprNode {
 private:
  RelFactorNode* leftFactor;
  RelFactorNode* rightFactor;
};

class GreaterThanEqualNode : RelExprNode {
 private:
  RelFactorNode* leftFactor;
  RelFactorNode* rightFactor;
};

class LessThanNode : RelExprNode {
 private:
  RelFactorNode* leftFactor;
  RelFactorNode* rightFactor;
};

class LessThanEqualNode : RelExprNode {
 private:
  RelFactorNode* leftFactor;
  RelFactorNode* rightFactor;
};

class EqualNode : RelExprNode {
 private:
  RelFactorNode* leftFactor;
  RelFactorNode* rightFactor;
};

class NotEqualNode : RelExprNode {
 private:
  ExprNode* leftFactor;
  ExprNode* rightFactor;
};

class ExprNode : SimpleAstNode {};

class PlusNode : ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;
};

class MinusNode : ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;
};

class TimesNode : ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;
};

class DivNode : ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;
};

class ModNode : ExprNode {
 private:
  ExprNode* leftExpression;
  ExprNode* rightExpression;
};

class ReferenceNode : SimpleAstNode {};

class VariableNode : ReferenceNode {
 private:
  std::string variableName;
};

class ConstantNode : ReferenceNode {
 private:
  int value;
};