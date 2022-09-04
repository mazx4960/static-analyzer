#pragma once

#include "compositegrammar.h"
#include "earlychoicegrammar.h"
#include "latechoicegrammar.h"
#include "listgrammar.h"
#include "recursivegrammar.h"
#include "sp/ast/node/simpleast.h"

class ProgramGrammarRule : public ListGrammarRule {
 private:
  bool shouldStop(TokenIterator tokenStream) override;
  ProgramNode* assembleNode(std::vector<SimpleAstNode*> children) override;
 public:
  ProgramGrammarRule();
};

class ProcedureGrammarRule : public CompositeGrammarRule {
 private:
  ProcedureNode* assembleNode(std::vector<SimpleAstNode*>) override;

 public:
  ProcedureGrammarRule();
};

class StatementListGrammarRule : public ListGrammarRule {
 private:
  bool shouldStop(TokenIterator tokenStream) override;
  StatementListNode* assembleNode(std::vector<SimpleAstNode*> children) override;
 public:
  StatementListGrammarRule();
};

class StatementGrammarRule : public EarlyChoiceGrammarRule {
 public:
  StatementGrammarRule();
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

class CondExprGrammarRule : public EarlyChoiceGrammarRule {
 public:
  CondExprGrammarRule();
};

class NotExprGrammarRule : public CompositeGrammarRule {
 private:
  NotExprNode* assembleNode(std::vector<SimpleAstNode*>) override;
 public:
  NotExprGrammarRule();
};

class BinaryCondGrammarRule : public LateChoiceGrammarRule {
 public:
  BinaryCondGrammarRule();
};

class RelExprGrammarRule : public LateChoiceGrammarRule {
 public:
  RelExprGrammarRule();
};

class RelFactorGrammarRule : public EarlyChoiceGrammarRule {
 public:
  RelFactorGrammarRule();
};

class ExprGrammarRule : public RecursiveGrammarRule {
 public:
  ExprGrammarRule();
};

class TermGrammarRule : public RecursiveGrammarRule {
 public:
  TermGrammarRule();
};

class FactorGrammarRule : public EarlyChoiceGrammarRule {
 public:
  FactorGrammarRule();
};

class ReferenceGrammarRule : public EarlyChoiceGrammarRule {
 public:
  ReferenceGrammarRule();
};

class VariableGrammarRule : public SimpleGrammarRule {
 public:
  VariableNode* parseNode(TokenIterator& tokenStream);
};

class ConstantGrammarRule : public SimpleGrammarRule {
 public:
  ConstantNode* parseNode(TokenIterator& tokenStream);
};