#pragma once

#include "commons/parser/grammar/composite_grammar.h"
#include "commons/parser/grammar/early_choice_grammar.h"
#include "commons/parser/grammar/late_choice_grammar.h"
#include "commons/parser/grammar/list_grammar.h"
#include "commons/parser/grammar/recursive_grammar.h"
#include "simple_ast.h"

class ProgramGrammarRule : public ListGrammarRule {
 private:
  bool shouldStop(TokenIterator &token_stream) override;
  ProgramNode *assembleNode(std::vector<Node *> &children) override;

 public:
  ProgramGrammarRule();
};

class ProcedureGrammarRule : public CompositeGrammarRule {
 private:
  ProcedureNode *assembleNode(std::vector<Node *> &) override;

 public:
  ProcedureGrammarRule();
};

class StatementListGrammarRule : public ListGrammarRule {
 private:
  bool shouldStop(TokenIterator &token_stream) override;
  StatementListNode *assembleNode(std::vector<Node *> &children) override;

 public:
  StatementListGrammarRule();
};

class StatementGrammarRule : public EarlyChoiceGrammarRule {
 public:
  StatementGrammarRule();
};

class ReadGrammarRule : public CompositeGrammarRule {
 private:
  ReadNode *assembleNode(std::vector<Node *> &) override;

 public:
  ReadGrammarRule();
};

class PrintGrammarRule : public CompositeGrammarRule {
 private:
  PrintNode *assembleNode(std::vector<Node *> &) override;

 public:
  PrintGrammarRule();
};

class CallGrammarRule : public CompositeGrammarRule {
 private:
  CallNode *assembleNode(std::vector<Node *> &) override;

 public:
  CallGrammarRule();
};

class WhileGrammarRule : public CompositeGrammarRule {
 private:
  WhileNode *assembleNode(std::vector<Node *> &) override;

 public:
  WhileGrammarRule();
};

class IfGrammarRule : public CompositeGrammarRule {
 private:
  IfNode *assembleNode(std::vector<Node *> &) override;

 public:
  IfGrammarRule();
};

class AssignGrammarRule : public CompositeGrammarRule {
 private:
  AssignNode *assembleNode(std::vector<Node *> &) override;

 public:
  AssignGrammarRule();
};

class CondExprGrammarRule : public EarlyChoiceGrammarRule {
 public:
  CondExprGrammarRule();
};

class NotExprGrammarRule : public CompositeGrammarRule {
 private:
  NotExprNode *assembleNode(std::vector<Node *> &) override;

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
