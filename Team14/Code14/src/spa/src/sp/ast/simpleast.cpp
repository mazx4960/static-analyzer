#include "simpleast.h"

#include <utility>

SimpleAstNode::SimpleAstNode(SimpleNodeType nodeType)
    : nodeType_(nodeType) {}
SimpleNodeType SimpleAstNode::GetNodeType() {
  return this->nodeType_;
}

ProgramNode::ProgramNode(std::vector<ProcedureNode*> procedures)
    : SimpleAstNode(SimpleNodeType::kProgram),
      procedures_(std::move(procedures)) {}

ProcedureNode::ProcedureNode(std::string procName, StatementListNode* statementList)
    : SimpleAstNode(SimpleNodeType::kProcedure),
      procName_(std::move(procName)),
      statementList_(statementList) {}

StatementListNode::StatementListNode(std::vector<StatementNode*> statements)
    : SimpleAstNode(SimpleNodeType::kStatementList),
      statements_(std::move(statements)) {}

StatementNode::StatementNode(SimpleStmtNodeType stmtType)
    : SimpleAstNode(SimpleNodeType::kStatement),
      stmtNo_(0) {
  this->stmtType_ = stmtType;
}
int StatementNode::GetStmtNo() {
  return this->stmtNo_;
}
SimpleStmtNodeType StatementNode::GetStmtType() {
  return this->stmtType_;
}

ReadNode::ReadNode(VariableNode* variable)
    : StatementNode(SimpleStmtNodeType::kRead),
      variable_(variable) {}

PrintNode::PrintNode(VariableNode* variable)
    : StatementNode(SimpleStmtNodeType::kPrint),
      variable_(variable) {}

CallNode::CallNode(std::string procedureName)
    : StatementNode(SimpleStmtNodeType::kCall),
      procedureName_(std::move(procedureName)) {}

WhileNode::WhileNode(CondExprNode* conditional, StatementListNode* statementList)
    : StatementNode(SimpleStmtNodeType::kWhile),
      conditional_(conditional),
      statementList_(statementList) {}

IfNode::IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList)
    : StatementNode(SimpleStmtNodeType::kIf),
      conditional_(conditional),
      thenStatementList_(thenStatementList),
      elseStatementList_(elseStatementList) {}

AssignNode::AssignNode(VariableNode* variable, ExprNode* expression)
    : StatementNode(SimpleStmtNodeType::kAssign),
      variable_(variable),
      expression_(expression) {}

CondExprNode::CondExprNode(SimpleNodeType nodeType)
    : SimpleAstNode(nodeType) {}

NotExprNode::NotExprNode(CondExprNode* negatedConditional)
    : CondExprNode(SimpleNodeType::kNot),
      negatedConditional_(negatedConditional) {}

BinaryCondExprNode::BinaryCondExprNode(SimpleNodeType nodeType, CondExprNode* firstConditional, CondExprNode* secondConditional)
    : CondExprNode(nodeType),
      firstConditional_(firstConditional),
      secondConditional_(secondConditional) {}

AndExprNode::AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : BinaryCondExprNode(SimpleNodeType::kAnd, firstConditional, secondConditional) {}

OrExprNode::OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : BinaryCondExprNode(SimpleNodeType::kOr, firstConditional, secondConditional) {}

RelExprNode::RelExprNode(SimpleNodeType nodeType, RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : CondExprNode(nodeType),
      leftFactor_(leftFactor),
      rightFactor_(rightFactor) {}

GreaterThanNode::GreaterThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::kGt, leftFactor, rightFactor) {}

GreaterThanEqualNode::GreaterThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::kGte, leftFactor, rightFactor) {}

LessThanNode::LessThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::kLt, leftFactor, rightFactor) {}

LessThanEqualNode::LessThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::kLte, leftFactor, rightFactor) {}

EqualNode::EqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::kEq, leftFactor, rightFactor) {}

NotEqualNode::NotEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::kNeq, leftFactor, rightFactor) {}

RelFactorNode::RelFactorNode(SimpleNodeType nodeType)
    : SimpleAstNode(nodeType) {}

ExprNode::ExprNode(SimpleNodeType nodeType, RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : RelFactorNode(nodeType),
      leftExpression_(leftExpression),
      rightExpression_(rightExpression) {}

PlusNode::PlusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::kPlus, leftExpression, rightExpression) {}

MinusNode::MinusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::kMinus, leftExpression, rightExpression) {}

TimesNode::TimesNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::kTimes, leftExpression, rightExpression) {}

DivNode::DivNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::kDiv, leftExpression, rightExpression) {}

ModNode::ModNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::kMod, leftExpression, rightExpression) {}

ReferenceNode::ReferenceNode(SimpleNodeType nodeType)
    : RelFactorNode(nodeType) {}

VariableNode::VariableNode(std::string variableName)
    : ReferenceNode(SimpleNodeType::kVariable),
      variableName_(std::move(variableName)) {}

ConstantNode::ConstantNode(int value)
    : ReferenceNode(SimpleNodeType::kConstant),
      value_(value) {}
