#include "simpleast.h"

#include <utility>

SimpleAstNode::SimpleAstNode(SimpleNodeType nodeType)
    : nodeType_(nodeType) {}
SimpleNodeType SimpleAstNode::GetNodeType() { return nodeType_; }

ProgramNode::ProgramNode(std::vector<ProcedureNode*> procedures)
    : SimpleAstNode(SimpleNodeType::kProgram),
      procedures_(std::move(procedures)) {}
std::vector<ProcedureNode*> ProgramNode::GetProcedures() {
  return this->procedures_;
}

ProcedureNode::ProcedureNode(std::string procName, StatementListNode* statementList)
    : SimpleAstNode(SimpleNodeType::kProcedure),
      procName_(std::move(procName)),
      statementList_(statementList) {}
StatementListNode* ProcedureNode::GetStatementList() {
  return this->statementList_;
}

StatementListNode::StatementListNode(std::vector<StatementNode*> statements)
    : SimpleAstNode(SimpleNodeType::kStatementList),
      statements_(std::move(statements)) {}
std::vector<StatementNode*> StatementListNode::GetStatements() {
  return this->statements_;
}

StatementNode::StatementNode(SimpleNodeType nodeType)
    : SimpleAstNode(nodeType),
      stmtNo_(0) {}

ReadNode::ReadNode(VariableNode* variable)
    : StatementNode(SimpleNodeType::kRead),
      variable_(variable) {}

PrintNode::PrintNode(VariableNode* variable)
    : StatementNode(SimpleNodeType::kPrint),
      variable_(variable) {}

CallNode::CallNode(std::string procedureName)
    : StatementNode(SimpleNodeType::kCall),
      procedureName_(std::move(procedureName)) {}

WhileNode::WhileNode(CondExprNode* conditional, StatementListNode* statementList)
    : StatementNode(SimpleNodeType::kWhile),
      conditional_(conditional),
      statementList_(statementList) {}

IfNode::IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList)
    : StatementNode(SimpleNodeType::kIf),
      conditional_(conditional),
      thenStatementList_(thenStatementList),
      elseStatementList_(elseStatementList) {}

AssignNode::AssignNode(VariableNode* variable, ExprNode* expression)
    : StatementNode(SimpleNodeType::kAssign),
      variable_(variable),
      expression_(expression) {}

CondExprNode::CondExprNode(SimpleNodeType nodeType)
    : SimpleAstNode(nodeType) {}

NotExprNode::NotExprNode(CondExprNode* negatedConditional)
    : CondExprNode(SimpleNodeType::kNot),
      negatedConditional_(negatedConditional) {}

AndExprNode::AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : CondExprNode(SimpleNodeType::kAnd),
      firstConditional_(firstConditional),
      secondConditional_(secondConditional) {}

OrExprNode::OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : CondExprNode(SimpleNodeType::kOr),
      firstConditional_(firstConditional),
      secondConditional_(secondConditional) {}

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
