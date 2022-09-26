#pragma once

#include "commons/lexer/token.h"
#include "commons/parser/grammar/grammar.h"

class RuleFragment {
 public:
  virtual void parseStream(TokenIterator &stream, std::vector<Node *> &childNodes) = 0;
};

class TokenRuleFragment : public RuleFragment {
 private:
  Token *token_to_match_;

 public:
  explicit TokenRuleFragment(Token *token);
  void parseStream(TokenIterator &stream, std::vector<Node *> &childNodes) override;
};

class NodeRuleFragment : public RuleFragment {
 private:
  GrammarRuleProducer *next_rule_producer_;

 public:
  void parseStream(TokenIterator &stream, std::vector<Node *> &childNodes) override;
  explicit NodeRuleFragment(GrammarRuleProducer *ruleProducer);
};

// Defines a rule that reads multiple tokens and/or other AST nodes
class CompositeGrammarRule : public GrammarRule {
 private:
  std::vector<RuleFragment *> rule_fragments_;

  virtual Node *assembleNode(std::vector<Node *>) = 0;

 public:
  explicit CompositeGrammarRule(std::vector<RuleFragment *> ruleFragments);
  Node *parseNode(TokenIterator &tokenStream) override;
};

class ParenthesizedGrammarRule : public CompositeGrammarRule {
 private:
  GrammarRuleProducer *parenthesized_node_;

  Node *assembleNode(std::vector<Node *>) override;

 public:
  explicit ParenthesizedGrammarRule(GrammarRuleProducer *parenthesizedNode);
};

class BracedGrammarRule : public CompositeGrammarRule {
 private:
  GrammarRuleProducer *parenthesized_node_;

  Node *assembleNode(std::vector<Node *>) override;

 public:
  explicit BracedGrammarRule(GrammarRuleProducer *parenthesizedNode);
};