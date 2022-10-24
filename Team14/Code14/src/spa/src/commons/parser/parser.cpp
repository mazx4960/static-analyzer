// Copyright 2022 CS3203 Team14. All rights reserved.

#include "parser.h"

Node *Parser::Parse(GrammarRule *grammar_rule, std::vector<Token *> &tokens) {
  auto *eof = new EndOfFileToken();
  if (tokens.back() != eof) {
    tokens.push_back(eof);
  }
  auto token_stream = tokens.begin();
  return grammar_rule->parseNode(token_stream);
}
Node *Parser::ParseExpression(std::vector<Token *> &tokens) {
  return Parse(new RelFactorGrammarRule(), tokens);
}
