#pragma once

#include "simplegrammar.h"
#include "token.h"

class RuleFragment {
 public:
  virtual void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) = 0;
};

class TokenRuleFragment : public RuleFragment {
 private:
  Token tokenToMatch;

 public:
  TokenRuleFragment(Token token);
  void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes);
};

class NodeRuleFragment : public RuleFragment {
 private:
  SimpleGrammarRule* nextRule;

 public:
  NodeRuleFragment(SimpleGrammarRule* rule);
  void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes);
};