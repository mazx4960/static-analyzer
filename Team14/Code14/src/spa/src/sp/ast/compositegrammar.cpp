#include "compositegrammar.h"
#include "exceptions.h"

TokenRuleFragment::TokenRuleFragment(Token token) : tokenToMatch(token) {}

void TokenRuleFragment::parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) {
  if (*stream != token) {
    throw ParseSyntaxError("Expected " + token->value + ", got " + stream->value);
  }
  ++stream;
}

NodeRuleFragment::NodeRuleFragment(SimpleGrammarRule* rule) : nextRule(rule) {}

void NodeRuleFragment::parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) {
  childNodes.push_back(nextRule->parseNode(stream));
}