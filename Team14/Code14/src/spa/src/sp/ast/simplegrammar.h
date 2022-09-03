#pragma once

#include "compositegrammar.h"
#include "sp/ast/simpleast.h"

class ProgramGrammarRule : public SimpleGrammarRule {
 public:
  ProgramNode* parseNode(TokenIterator& tokenStream);
};

class ProcedureGrammarRule : public CompositeGrammarRule {
 private:
  ProcedureNode* assembleNode(std::vector<SimpleAstNode*>);

 public:
  ProcedureGrammarRule();
  ProcedureNode* parseNode(TokenIterator& tokenStream);
};

class StatementListGrammarRule : public SimpleGrammarRule {
 public:
  StatementListNode* parseNode(TokenIterator& tokenStream);
};

class StatementGrammarRule : public SimpleGrammarRule {
 public:
  StatementNode* parseNode(TokenIterator& tokenStream);
};

class ReadGrammarRule : public SimpleGrammarRule {
 public:
  ReadNode* parseNode(TokenIterator& tokenStream);
};

class PrintGrammarRule : public SimpleGrammarRule {
 public:
  PrintNode* parseNode(TokenIterator& tokenStream);
};

class CallGrammarRule : public SimpleGrammarRule {
 public:
  CallNode* parseNode(TokenIterator& tokenStream);
};

class WhileGrammarRule : public SimpleGrammarRule {
 public:
  WhileNode* parseNode(TokenIterator& tokenStream);
};

class IfGrammarRule : public SimpleGrammarRule {
 public:
  IfNode* parseNode(TokenIterator& tokenStream);
};

class AssignGrammarRule : public SimpleGrammarRule {
 public:
  AssignNode* parseNode(TokenIterator& tokenStream);
};

class CondExprGrammarRule : public SimpleGrammarRule {
 public:
  CondExprNode* parseNode(TokenIterator& tokenStream);
};

class RelExprGrammarRule : public SimpleGrammarRule {
 public:
  RelExprNode* parseNode(TokenIterator& tokenStream);
};

class ExprGrammarRule : public SimpleGrammarRule {
 public:
  ExprNode* parseNode(TokenIterator& tokenStream);
};

class TermGrammarRule : public SimpleGrammarRule {
 public:
  ExprNode* parseNode(TokenIterator& tokenStream);
};

class FactorGrammarRule : public SimpleGrammarRule {
 public:
  ExprNode* parseNode(TokenIterator& tokenStream);
};

class ReferenceGrammarRule : public SimpleGrammarRule {
 public:
  ReferenceNode* parseNode(TokenIterator& tokenStream);
};

class VariableGrammarRule : public SimpleGrammarRule {
 public:
  VariableNode* parseNode(TokenIterator& tokenStream);
};

class ConstantGrammarRule : public SimpleGrammarRule {
 public:
  ConstantNode* parseNode(TokenIterator& tokenStream);
};