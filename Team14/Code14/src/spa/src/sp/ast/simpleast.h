#pragma once

#include <string>
#include <vector>

#include "commons/entity.h"
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
  virtual std::vector<SimpleAstNode*> GetChildren() = 0;
};

class ProgramNode : public SimpleAstNode {
 private:
  std::vector<ProcedureNode*> procedures_;

 public:
  explicit ProgramNode(std::vector<ProcedureNode*> procedures);
  std::vector<ProcedureNode*> GetProcedures();
  std::vector<SimpleAstNode*> GetChildren() override;
};

class ProcedureNode : public SimpleAstNode {
 private:
  std::string procName_;
  StatementListNode* statementList_;

 public:
  ProcedureNode(std::string procName, StatementListNode* statementList);
  [[nodiscard]] std::string GetProcName() const;
  StatementListNode* GetStatementList();
  std::vector<SimpleAstNode*> GetChildren() override;
};

class StatementListNode : public SimpleAstNode {
 private:
  std::vector<StatementNode*> statements_;

 public:
  explicit StatementListNode(std::vector<StatementNode*> statements);
  std::vector<StatementNode*> GetStatements();
  std::vector<SimpleAstNode*> GetChildren() override;
};

class StatementNode : public SimpleAstNode {
 private:
  int stmtNo_;
  StmtType stmtType_;

 public:
  explicit StatementNode(StmtType stmtType);
  [[nodiscard]] int GetStmtNo() const;
  StmtType GetStmtType();
};

class ReadNode : public StatementNode {
 private:
  VariableNode* variable_;

 public:
  explicit ReadNode(VariableNode* variable);
  std::vector<SimpleAstNode*> GetChildren() override;
};

class PrintNode : public StatementNode {
 private:
  VariableNode* variable_;

 public:
  explicit PrintNode(VariableNode* variable);
  std::vector<SimpleAstNode*> GetChildren() override;
};

class CallNode : public StatementNode {
 private:
  std::string procedureName_;

 public:
  explicit CallNode(std::string procedureName);
  std::vector<SimpleAstNode*> GetChildren() override;
};

class WhileNode : public StatementNode {
 private:
  CondExprNode* conditional_;
  StatementListNode* statementList_;

 public:
  WhileNode(CondExprNode* conditional, StatementListNode* statementList);
  StatementListNode* GetStatementList();
  std::vector<SimpleAstNode*> GetChildren() override;
};

class IfNode : public StatementNode {
 private:
  CondExprNode* conditional_;
  StatementListNode* thenStatementList_;
  StatementListNode* elseStatementList_;

 public:
  IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList);
  StatementListNode* GetThenStatementList();
  StatementListNode* GetElseStatementList();
  std::vector<SimpleAstNode*> GetChildren() override;
};

class AssignNode : public StatementNode {
 private:
  VariableNode* variable_;
  RelFactorNode* expression_;

 public:
  AssignNode(VariableNode* variable, RelFactorNode* expression);
  std::vector<SimpleAstNode*> GetChildren() override;
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
  std::vector<SimpleAstNode*> GetChildren() override;
};

class BinaryCondExprNode : public CondExprNode {
 private:
  CondExprNode* firstConditional_;
  CondExprNode* secondConditional_;

 public:
  BinaryCondExprNode(SimpleNodeType nodeType, CondExprNode* firstConditional, CondExprNode* secondConditional);
  std::vector<SimpleAstNode*> GetChildren() override;
};

class AndExprNode : public BinaryCondExprNode {
 public:
  AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional);
};

class OrExprNode : public BinaryCondExprNode {
 public:
  OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional);
};

class RelExprNode : public CondExprNode {
 private:
  RelFactorNode* leftFactor_;
  RelFactorNode* rightFactor_;

 public:
  RelExprNode(SimpleNodeType nodeType, RelFactorNode* leftFactor, RelFactorNode* rightFactor);
  std::vector<SimpleAstNode*> GetChildren() override;
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
  std::vector<SimpleAstNode*> GetChildren() override;
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
  std::vector<SimpleAstNode*> GetChildren() override;
};

class VariableNode : public ReferenceNode {
 private:
  std::string variableName_;

 public:
  explicit VariableNode(std::string variableName);
  std::string GetVariableName();
};

class ConstantNode : public ReferenceNode {
 private:
  int value_;

 public:
  explicit ConstantNode(int value);
  [[nodiscard]] int GetValue() const;
};
