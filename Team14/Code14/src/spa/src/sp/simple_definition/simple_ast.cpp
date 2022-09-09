#include "simple_ast.h"

#include <utility>

#include "commons/entity.h"
#include "commons/parser/node/node_type.h"

ProgramNode::ProgramNode(std::vector<ProcedureNode*> procedures)
    : Node(NodeType::kProgram),
      procedures_(std::move(procedures)) {
  int stmt_no = 1;
  for (auto* procedure : procedures_) { stmt_no = procedure->GetStatementList()->SetStatementNos(stmt_no); }
}

std::vector<Node*> ProgramNode::GetChildren() { return std::vector<Node*>(procedures_.begin(), procedures_.end()); }
std::string ProgramNode::ToString() { return "program"; }

ProcedureNode::ProcedureNode(std::string procName, StatementListNode* statementList)
    : Node(NodeType::kProcedure),
      procName_(std::move(procName)),
      statementList_(statementList) {}

std::string ProcedureNode::GetProcName() const { return this->procName_; }

StatementListNode* ProcedureNode::GetStatementList() { return this->statementList_; }

std::vector<Node*> ProcedureNode::GetChildren() { return std::vector<Node*>{statementList_}; }
std::string ProcedureNode::ToString() { return "procedure"; }

StatementListNode::StatementListNode(std::vector<StatementNode*> statements)
    : Node(NodeType::kStatementList),
      statements_(std::move(statements)) {}
std::vector<StatementNode*> StatementListNode::GetStatements() { return this->statements_; }

int StatementListNode::SetStatementNos(int first_stmt_no) {
  int stmt_no = first_stmt_no;
  for (auto *statement : statements_) {stmt_no = statement->SetStatementNo(stmt_no);}
  return stmt_no;
}

std::vector<Node*> StatementListNode::GetChildren() {
  return std::vector<Node*>(statements_.begin(), statements_.end());
}
std::string StatementListNode::ToString() { return std::string(); }

StatementNode::StatementNode(StmtType stmtType) : Node(NodeType::kStatement), stmtNo_(0) { this->stmtType_ = stmtType; }
int StatementNode::GetStmtNo() const { return this->stmtNo_; }
int StatementNode::SetStatementNo(int next_stmt_no) {
  this->stmtNo_ = next_stmt_no;
  return next_stmt_no + 1;
}
StmtType StatementNode::GetStmtType() { return this->stmtType_; }

ReadNode::ReadNode(VariableNode* variable) : StatementNode(StmtType::kRead), variable_(variable) {}

std::vector<Node*> ReadNode::GetChildren() { return std::vector<Node*>{variable_}; }
std::string ReadNode::ToString() { return "read"; }

PrintNode::PrintNode(VariableNode* variable) : StatementNode(StmtType::kPrint), variable_(variable) {}

std::vector<Node*> PrintNode::GetChildren() { return std::vector<Node*>{variable_}; }
std::string PrintNode::ToString() { return "print"; }

CallNode::CallNode(std::string procedureName)
    : StatementNode(StmtType::kCall),
      procedureName_(std::move(procedureName)) {}

std::vector<Node*> CallNode::GetChildren() { return std::vector<Node*>{}; }
std::string CallNode::ToString() { return "call"; }

WhileNode::WhileNode(CondExprNode* conditional, StatementListNode* statementList)
    : StatementNode(StmtType::kWhile),
      conditional_(conditional),
      statementList_(statementList) {}
StatementListNode* WhileNode::GetStatementList() { return this->statementList_; }

int WhileNode::SetStatementNo(int next_stmt_no) {
  int stmt_no = StatementNode::SetStatementNo(next_stmt_no);
  return statementList_->SetStatementNos(stmt_no);
}

std::vector<Node*> WhileNode::GetChildren() { return std::vector<Node*>{conditional_, statementList_}; }
std::string WhileNode::ToString() { return "while"; }

IfNode::IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList)
    : StatementNode(StmtType::kIf),
      conditional_(conditional),
      thenStatementList_(thenStatementList),
      elseStatementList_(elseStatementList) {}
StatementListNode* IfNode::GetThenStatementList() { return this->thenStatementList_; }
StatementListNode* IfNode::GetElseStatementList() { return this->elseStatementList_; }
int IfNode::SetStatementNo(int next_stmt_no) {
  int then_stmt_no = StatementNode::SetStatementNo(next_stmt_no);
  int else_stmt_no = thenStatementList_->SetStatementNos(then_stmt_no);
  return elseStatementList_->SetStatementNos(else_stmt_no);
}

std::vector<Node*> IfNode::GetChildren() {
  return std::vector<Node*>{conditional_, thenStatementList_, elseStatementList_};
}
std::string IfNode::ToString() { return "if"; }

AssignNode::AssignNode(VariableNode* variable, RelFactorNode* expression)
    : StatementNode(StmtType::kAssign),
      variable_(variable),
      expression_(expression) {}

std::vector<Node*> AssignNode::GetChildren() { return std::vector<Node*>{variable_, expression_}; }
std::string AssignNode::ToString() { return "assign"; }

CondExprNode::CondExprNode(CondExprType cond_expr_type) : Node(NodeType::kCondExpr) {
  this->cond_expr_type_ = cond_expr_type;
}
std::string CondExprNode::ToString() { return ""; }

NotExprNode::NotExprNode(CondExprNode* negatedConditional)
    : CondExprNode(CondExprType::kNot),
      negatedConditional_(negatedConditional) {}

std::vector<Node*> NotExprNode::GetChildren() { return std::vector<Node*>{negatedConditional_}; }

BinaryCondExprNode::BinaryCondExprNode(CondExprType cond_expr_type, CondExprNode* firstConditional,
                                       CondExprNode* secondConditional)
    : CondExprNode(cond_expr_type),
      firstConditional_(firstConditional),
      secondConditional_(secondConditional) {}

std::vector<Node*> BinaryCondExprNode::GetChildren() {
  return std::vector<Node*>{firstConditional_, secondConditional_};
}

AndExprNode::AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : BinaryCondExprNode(CondExprType::kAnd, firstConditional, secondConditional) {}

OrExprNode::OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : BinaryCondExprNode(CondExprType::kOr, firstConditional, secondConditional) {}

RelExprNode::RelExprNode(CondExprType cond_expr_type, RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : CondExprNode(cond_expr_type),
      leftFactor_(leftFactor),
      rightFactor_(rightFactor) {}

std::vector<Node*> RelExprNode::GetChildren() { return std::vector<Node*>{leftFactor_, rightFactor_}; }
std::string RelExprNode::ToString() { return std::string(); }

GreaterThanNode::GreaterThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(CondExprType::kGt, leftFactor, rightFactor) {}

GreaterThanEqualNode::GreaterThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(CondExprType::kGte, leftFactor, rightFactor) {}

LessThanNode::LessThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(CondExprType::kLt, leftFactor, rightFactor) {}

LessThanEqualNode::LessThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(CondExprType::kLte, leftFactor, rightFactor) {}

EqualNode::EqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(CondExprType::kEq, leftFactor, rightFactor) {}

NotEqualNode::NotEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(CondExprType::kNeq, leftFactor, rightFactor) {}
