// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/parser/grammar/composite_grammar.h"
#include "commons/parser/grammar/grammar.h"
#include "expr_grammar.h"

class ParenthesizedGrammarProducer : public GrammarRuleProducer {
 private:
  GrammarRuleProducer* parenthesized_rule_;

 public:
  explicit ParenthesizedGrammarProducer(GrammarRuleProducer* producer);
  ParenthesizedGrammarRule* produce() override;
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
