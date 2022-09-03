#include "compositegrammar.h"
#include "commons/parser/parser_exceptions.h"

#include <utility>

TokenRuleFragment::TokenRuleFragment(Token* token) : tokenToMatch_(std::move(token)) {}

void TokenRuleFragment::parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) {
  if (!(**stream == *tokenToMatch_)) {
    throw ParseSyntaxError("Expected " + tokenToMatch_->value + ", got " + (*stream)->value);
  }
  ++stream;
}

NodeRuleFragment::NodeRuleFragment(SimpleGrammarRule* rule) : nextRule_(rule) {}

void NodeRuleFragment::parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) {
  childNodes.push_back(nextRule_->parseNode(stream));
}

CompositeGrammarRule::CompositeGrammarRule(std::vector<RuleFragment*> ruleFragments)
    : ruleFragments_(std::move(ruleFragments)) {}