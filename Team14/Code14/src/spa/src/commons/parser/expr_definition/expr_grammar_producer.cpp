// Copyright 2022 CS3203 Team14. All rights reserved.

#include "expr_grammar_producer.h"

ParenthesizedGrammarProducer::ParenthesizedGrammarProducer(GrammarRuleProducer* producer)
    : parenthesized_rule_(producer) {}

ParenthesizedGrammarRule* ParenthesizedGrammarProducer::produce() {
  return new ParenthesizedGrammarRule(parenthesized_rule_);
}

RelFactorGrammarRule* RelFactorGrammarProducer::produce() { return new RelFactorGrammarRule(); }

ExprGrammarRule* ExprGrammarProducer::produce() { return new ExprGrammarRule(); }

TermGrammarRule* TermGrammarProducer::produce() { return new TermGrammarRule(); }

FactorGrammarRule* FactorGrammarProducer::produce() { return new FactorGrammarRule(); }

ReferenceGrammarRule* ReferenceGrammarProducer::produce() { return new ReferenceGrammarRule(); }

VariableGrammarRule* VariableGrammarProducer::produce() { return new VariableGrammarRule(); }

ConstantGrammarRule* ConstantGrammarProducer::produce() { return new ConstantGrammarRule(); }