#include "simpleast.h"

SimpleAstNode::SimpleAstNode(SimpleNodeType nodeType)
    : nodeType(nodeType) {}

ProgramNode::ProgramNode(std::vector<ProcedureNode*> procedures)
    : SimpleAstNode(SimpleNodeType::PROGRAM), 
      procedures(procedures) {}

ProcedureNode::ProcedureNode(std::string procName, StatementListNode* statementList)
    : SimpleAstNode(SimpleNodeType::PROCEDURE),
      procName(procName),
      statementList(statementList) {}

StatementListNode::StatementListNode(std::vector<StatementNode*> statements)
    : SimpleAstNode(SimpleNodeType::STATEMENT_LIST),
      statements(statements) {}

StatementNode::StatementNode(SimpleNodeType nodeType) 
    : SimpleAstNode(nodeType),
      stmtNo(0) {}

ReadNode::ReadNode(VariableNode* variable)
    : StatementNode(SimpleNodeType::READ),
      variable(variable) {}

PrintNode::PrintNode(VariableNode* variable)
    : StatementNode(SimpleNodeType::PRINT),
      variable(variable) {}

CallNode::CallNode(std::string procedureName)
    : StatementNode(SimpleNodeType::CALL),
      procedureName(procedureName) {}

WhileNode::WhileNode(CondExprNode* conditional, StatementListNode* statementList)
    : StatementNode(SimpleNodeType::WHILE),
      conditional(conditional),
      statementList(statementList) {}

IfNode::IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList)
    : StatementNode(SimpleNodeType::IF),
      conditional(conditional),
      thenStatementList(thenStatementList),
      elseStatementList(elseStatementList) {}

AssignNode::AssignNode(VariableNode* variable, ExprNode* expression)
    : StatementNode(SimpleNodeType::ASSIGN),
      variable(variable),
      expression(expression) {}

CondExprNode::CondExprNode(SimpleNodeType nodeType)
    : SimpleAstNode(nodeType) {}

NotExprNode::NotExprNode(CondExprNode* negatedConditional)
    : CondExprNode(SimpleNodeType::NOT), 
      negatedConditional(negatedConditional) {}

AndExprNode::AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : CondExprNode(SimpleNodeType::NOT),
      firstConditional(firstConditional),
      secondConditional(secondConditional) {}

OrExprNode::OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : CondExprNode(SimpleNodeType::NOT),
      firstConditional(firstConditional),
      secondConditional(secondConditional) {}

RelExprNode::RelExprNode(SimpleNodeType nodeType, RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : CondExprNode(nodeType),
      leftFactor(leftFactor),
      rightFactor(rightFactor) {}

GreaterThanNode::GreaterThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::GT, leftFactor, rightFactor) {}

GreaterThanEqualNode::GreaterThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::GTE, leftFactor, rightFactor) {}

LessThanNode::LessThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::LT, leftFactor, rightFactor) {}

LessThanEqualNode::LessThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::LTE, leftFactor, rightFactor) {}

EqualNode::EqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::EQ, leftFactor, rightFactor) {}

NotEqualNode::NotEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(SimpleNodeType::NEQ, leftFactor, rightFactor) {}

RelFactorNode::RelFactorNode(SimpleNodeType nodeType)
    : SimpleAstNode(nodeType) {}

ExprNode::ExprNode(SimpleNodeType nodeType, RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : RelFactorNode(nodeType),
      leftExpression(leftExpression),
      rightExpression(rightExpression) {}

PlusNode::PlusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::PLUS, leftExpression, rightExpression) {}

MinusNode::MinusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::MINUS, leftExpression, rightExpression) {}

TimesNode::TimesNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::TIMES, leftExpression, rightExpression) {}

DivNode::DivNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::DIV, leftExpression, rightExpression) {}

ModNode::ModNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(SimpleNodeType::MOD, leftExpression, rightExpression) {}

ReferenceNode::ReferenceNode(SimpleNodeType nodeType)
    : RelFactorNode(nodeType) {}

VariableNode::VariableNode(std::string variableName)
    : ReferenceNode(SimpleNodeType::VARIABLE),
      variableName(variableName) {}

ConstantNode::ConstantNode(int value)
    : ReferenceNode(SimpleNodeType::CONSTANT),
      value(value) {}

