#pragma once

#include "simplegrammar.h"

class ParenthesizedGrammarProducer : public GrammarRuleProducer {
 private:
  GrammarRuleProducer* parenthesized_rule_;
 public:
  explicit ParenthesizedGrammarProducer(GrammarRuleProducer* producer);
  ParenthesizedGrammarRule* produce() override;
};

class BracedGrammarProducer : public GrammarRuleProducer {
 private:
  GrammarRuleProducer* braced_rule_;
 public:
  explicit BracedGrammarProducer(GrammarRuleProducer* producer);
  BracedGrammarRule* produce() override;
};

class ProgramGrammarProducer : public GrammarRuleProducer {
 public:
  ProgramGrammarRule* produce() override;
};

class ProcedureGrammarProducer : public GrammarRuleProducer {
 public:
  ProcedureGrammarRule* produce() override;
};

class StatementListGrammarProducer : public GrammarRuleProducer {
 public:
  StatementListGrammarRule* produce() override;
};

class StatementGrammarProducer : public GrammarRuleProducer {
 public:
  StatementGrammarRule* produce() override;
};

class ReadGrammarProducer : public GrammarRuleProducer {
 public:
  ReadGrammarRule* produce() override;
};

class PrintGrammarProducer : public GrammarRuleProducer {
 public:
  PrintGrammarRule* produce() override;
};

class CallGrammarProducer : public GrammarRuleProducer {
 public:
  CallGrammarRule* produce() override;
};

class WhileGrammarProducer : public GrammarRuleProducer {
 public:
  WhileGrammarRule* produce() override;
};

class IfGrammarProducer : public GrammarRuleProducer {
 public:
  IfGrammarRule* produce() override;
};

class AssignGrammarProducer : public GrammarRuleProducer {
 public:
  AssignGrammarRule* produce() override;
};

class CondExprGrammarProducer : public GrammarRuleProducer {
 public:
  CondExprGrammarRule* produce() override;
};

class NotExprGrammarProducer : public GrammarRuleProducer {
 public:
  NotExprGrammarRule* produce() override;
};

class BinaryCondGrammarProducer : public GrammarRuleProducer {
 public:
  BinaryCondGrammarRule* produce() override;
};

class RelExprGrammarProducer : public GrammarRuleProducer {
 public:
  RelExprGrammarRule* produce() override;
};

class RelFactorGrammarProducer : public GrammarRuleProducer {
 public:
  RelFactorGrammarRule* produce() override;
};

class ExprGrammarProducer : public GrammarRuleProducer {
 public:
  ExprGrammarRule* produce() override;
};

class TermGrammarProducer : public GrammarRuleProducer {
 public:
  TermGrammarRule* produce() override;
};

class FactorGrammarProducer : public GrammarRuleProducer {
 public:
  FactorGrammarRule* produce() override;
};

class ReferenceGrammarProducer : public GrammarRuleProducer {
 public:
  ReferenceGrammarRule* produce() override;
};

class VariableGrammarProducer : public GrammarRuleProducer {
 public:
  VariableGrammarRule* produce() override;
};

class ConstantGrammarProducer : public GrammarRuleProducer {
 public:
  ConstantGrammarRule* produce() override;
};