// Copyright 2022 CS3203 Team14. All rights reserved.

#include "parser.h"

#include <utility>

#include "commons/parser/expr_definition/expr_grammar.h"

Node* Parser::Parse(GrammarRule* grammar_rule, std::vector<Token*> tokens) {
  auto token_stream = tokens.begin();
  return grammar_rule->parseNode(token_stream);
}
Node* Parser::ParseExpression(const std::vector<Token*>& tokens) {
  return Parse(new RelFactorGrammarRule(), std::move(tokens));
}
