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

GreaterThanNode::GreaterThanNode(ExprNode* leftFactor, ExprNode* rightFactor)
    : leftFactor(leftFactor),
      rightFactor(rightFactor) {}

GreaterThanEqualNode::GreaterThanEqualNode(ExprNode* leftFactor, ExprNode* rightFactor)
    : leftFactor(leftFactor),
      rightFactor(rightFactor) {}

LessThanNode::LessThanNode(ExprNode* leftFactor, ExprNode* rightFactor)
    : leftFactor(leftFactor),
      rightFactor(rightFactor) {}

LessThanEqualNode::LessThanEqualNode(ExprNode* leftFactor, ExprNode* rightFactor)
    : leftFactor(leftFactor),
      rightFactor(rightFactor) {}

EqualNode::EqualNode(ExprNode* leftFactor, ExprNode* rightFactor)
    : leftFactor(leftFactor),
      rightFactor(rightFactor) {}

NotEqualNode::NotEqualNode(ExprNode* leftFactor, ExprNode* rightFactor)
    : leftFactor(leftFactor),
      rightFactor(rightFactor) {}

PlusNode::PlusNode(ExprNode* leftExpression, ExprNode* rightExpression)
    : leftExpression(leftExpression),
      rightExpression(rightExpression) {}

MinusNode::MinusNode(ExprNode* leftExpression, ExprNode* rightExpression)
    : leftExpression(leftExpression),
      rightExpression(rightExpression) {}

TimesNode::TimesNode(ExprNode* leftExpression, ExprNode* rightExpression)
    : leftExpression(leftExpression),
      rightExpression(rightExpression) {}

DivNode::DivNode(ExprNode* leftExpression, ExprNode* rightExpression)
    : leftExpression(leftExpression),
      rightExpression(rightExpression) {}

ModNode::ModNode(ExprNode* leftExpression, ExprNode* rightExpression)
    : leftExpression(leftExpression),
      rightExpression(rightExpression) {}

VariableNode::VariableNode(std::string variableName)
    : variableName(variableName) {}

std::string VariableNode::getName() {
  return variableName;
}

ConstantNode::ConstantNode(int value)
    : value(value) {}