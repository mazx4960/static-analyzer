#include "simpleast.h"

ProgramNode::ProgramNode(std::vector<ProcedureNode*> procedures)
    : procedures(procedures) {}

ProcedureNode::ProcedureNode(std::string procName, StatementListNode* statementList)
    : procName(procName),
      statementList(statementList) {}

StatementListNode::StatementListNode(std::vector<StatementNode*> statements)
    : statements(statements) {}

StatementNode::StatementNode() 
    : stmtNo(0) {}

ReadNode::ReadNode(VariableNode* variable)
    : variable(variable) {}

PrintNode::PrintNode(VariableNode* variable)
    : variable(variable) {}

CallNode::CallNode(std::string procedureName)
    : procedureName(procedureName) {}

WhileNode::WhileNode(CondExprNode* conditional, StatementListNode* statementList)
    : conditional(conditional),
      statementList(statementList) {}

IfNode::IfNode(CondExprNode* conditional, StatementListNode* thenStatementList, StatementListNode* elseStatementList)
    : conditional(conditional),
      thenStatementList(thenStatementList),
      elseStatementList(elseStatementList) {}

AssignNode::AssignNode(VariableNode* variable, ExprNode* expression)
    : variable(variable),
      expression(expression) {}

NotExprNode::NotExprNode(CondExprNode* negatedConditional)
    : negatedConditional(negatedConditional) {}

AndExprNode::AndExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : firstConditional(firstConditional),
      secondConditional(secondConditional) {}

OrExprNode::OrExprNode(CondExprNode* firstConditional, CondExprNode* secondConditional)
    : firstConditional(firstConditional),
      secondConditional(secondConditional) {}

RelExprNode::RelExprNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : leftFactor(leftFactor),
      rightFactor(rightFactor) {}

GreaterThanNode::GreaterThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(leftFactor, rightFactor) {}

GreaterThanEqualNode::GreaterThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(leftFactor, rightFactor) {}

LessThanNode::LessThanNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(leftFactor, rightFactor) {}

LessThanEqualNode::LessThanEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(leftFactor, rightFactor) {}

EqualNode::EqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(leftFactor, rightFactor) {}

NotEqualNode::NotEqualNode(RelFactorNode* leftFactor, RelFactorNode* rightFactor)
    : RelExprNode(leftFactor, rightFactor) {}

ExprNode::ExprNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : leftExpression(leftExpression),
      rightExpression(rightExpression) {}

PlusNode::PlusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(leftExpression, rightExpression) {}

MinusNode::MinusNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(leftExpression, rightExpression) {}

TimesNode::TimesNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(leftExpression, rightExpression) {}

DivNode::DivNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(leftExpression, rightExpression) {}

ModNode::ModNode(RelFactorNode* leftExpression, RelFactorNode* rightExpression)
    : ExprNode(leftExpression, rightExpression) {}

VariableNode::VariableNode(std::string variableName)
    : variableName(variableName) {}

ConstantNode::ConstantNode(int value)
    : value(value) {}