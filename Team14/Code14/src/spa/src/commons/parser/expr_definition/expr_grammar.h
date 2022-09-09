// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/parser/grammar/early_choice_grammar.h"
#include "commons/parser/grammar/recursive_grammar.h"
#include "expr_node.h"

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

class VariableGrammarRule : public GrammarRule {
 public:
  VariableNode* parseNode(TokenIterator& tokenStream) override;
};

class ConstantGrammarRule : public GrammarRule {
 public:
  ConstantNode* parseNode(TokenIterator& tokenStream) override;
};
