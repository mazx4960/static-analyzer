#pragma once

#include "compositegrammar.h"
#include "sp/ast/node/simpleast.h"

class ProgramGrammarRule : public SimpleGrammarRule {
 public:
  ProgramNode* parseNode(TokenIterator& tokenStream);
};

class ProcedureGrammarRule : public CompositeGrammarRule {
 private:
  ProcedureNode* assembleNode(std::vector<SimpleAstNode*>) override;

 public:
  ProcedureGrammarRule();
};

class StatementListGrammarRule : public SimpleGrammarRule {
 public:
  StatementListNode* parseNode(TokenIterator& tokenStream);
};

class StatementGrammarRule : public SimpleGrammarRule {
 public:
  StatementNode* parseNode(TokenIterator& tokenStream);
};

class ReadGrammarRule : public CompositeGrammarRule {
 private:
  ReadNode* assembleNode(std::vector<SimpleAstNode*>) override;

 public:
  ReadGrammarRule();
};

class PrintGrammarRule : public CompositeGrammarRule {
 private:
  PrintNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  PrintGrammarRule();
};

class CallGrammarRule : public CompositeGrammarRule {
 private:
  CallNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  CallGrammarRule();
};

class WhileGrammarRule : public CompositeGrammarRule {
 private:
  WhileNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  WhileGrammarRule();
};

class IfGrammarRule : public CompositeGrammarRule {
 private:
  IfNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  IfGrammarRule();
};

class AssignGrammarRule : public CompositeGrammarRule {
 private:
  AssignNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  AssignGrammarRule();
};

class CondExprGrammarRule : public SimpleGrammarRule {
 public:
  CondExprNode* parseNode(TokenIterator& tokenStream);
};

class RelExprGrammarRule : public SimpleGrammarRule {
 public:
  RelExprNode* parseNode(TokenIterator& tokenStream);
};

class RelFactorGrammarRule : public SimpleGrammarRule {
 public:
  RelFactorNode* parseNode(TokenIterator& tokenStream);
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