#pragma once

#include <string>
#include <vector>

#include "commons/entity.h"
#include "commons/parser/expr_definition/expr_node.h"
#include "commons/parser/node/node.h"

class Node;
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

class ProgramNode : public Node {
 private:
  std::vector<ProcedureNode*> procedures_;

 public:
  explicit ProgramNode(std::vector<ProcedureNode*> procedures);
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class ProcedureNode : public Node {
 private:
  std::string procName_;
  StatementListNode* statementList_;

 public:
  ProcedureNode(std::string procName, StatementListNode* statementList);
  [[nodiscard]] std::string GetProcName() const;
  StatementListNode* GetStatementList();
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class StatementListNode : public Node {
 private:
  std::vector<StatementNode*> statements_;

 public:
  explicit StatementListNode(std::vector<StatementNode*> statements);
  std::vector<StatementNode*> GetStatements();
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class StatementNode : public Node {
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
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class PrintNode : public StatementNode {
 private:
  VariableNode* variable_;

 public:
  explicit PrintNode(VariableNode* variable);
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class CallNode : public StatementNode {
 private:
  std::string procedureName_;

 public:
  explicit CallNode(std::string procedureName);
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class WhileNode : public StatementNode {
 private:
  CondExprNode* conditional_;
  StatementListNode* statementList_;

 public:
  WhileNode(CondExprNode* conditional, StatementListNode* statementList);
  StatementListNode* GetStatementList();
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
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
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class AssignNode : public StatementNode {
 private:
  VariableNode* variable_;
  RelFactorNode* expression_;

 public:
  AssignNode(VariableNode* variable, RelFactorNode* expression);
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
};

class CondExprNode : public Node {
 private:
  CondExprType cond_expr_type_;

 public:
  explicit CondExprNode(CondExprType cond_expr_type);
  std::string ToString() override;
};

class NotExprNode : public CondExprNode {
 private:
  CondExprNode* negatedConditional_;

 public:
  explicit NotExprNode(CondExprNode* negatedConditional);
  std::vector<Node*> GetChildren() override;
};

class BinaryCondExprNode : public CondExprNode {
 private:
  CondExprNode* firstConditional_;
  CondExprNode* secondConditional_;

 public:
  BinaryCondExprNode(CondExprType cond_expr_type, CondExprNode* firstConditional, CondExprNode* secondConditional);
  std::vector<Node*> GetChildren() override;
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
  RelExprNode(CondExprType cond_expr_type, RelFactorNode* leftFactor, RelFactorNode* rightFactor);
  std::vector<Node*> GetChildren() override;
  std::string ToString() override;
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
