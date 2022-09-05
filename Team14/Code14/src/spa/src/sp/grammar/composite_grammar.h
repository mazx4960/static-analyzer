#pragma once

#include "base_simple_grammar.h"
#include "commons/lexer/token.h"

class RuleFragment {
 public:
  virtual void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) = 0;
};

class TokenRuleFragment : public RuleFragment {
 private:
  Token* token_to_match_;

 public:
  explicit TokenRuleFragment(Token* token);
  void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) override;
};

class NodeRuleFragment : public RuleFragment {
 private:
  GrammarRuleProducer* next_rule_producer_;

 public:
  void parseStream(TokenIterator& stream, std::vector<SimpleAstNode*>& childNodes) override;
  explicit NodeRuleFragment(GrammarRuleProducer* ruleProducer);
};

// Defines a rule that reads multiple tokens and/or other AST nodes
class CompositeGrammarRule : public SimpleGrammarRule {
 private:
  std::vector<RuleFragment*> rule_fragments_;
  virtual SimpleAstNode* assembleNode(std::vector<SimpleAstNode*>) = 0;

 public:
  explicit CompositeGrammarRule(std::vector<RuleFragment*> ruleFragments);
  SimpleAstNode* parseNode(TokenIterator& tokenStream) override;
};

class ParenthesizedGrammarRule : public CompositeGrammarRule {
 private:
  GrammarRuleProducer* parenthesized_node_;
  SimpleAstNode* assembleNode(std::vector<SimpleAstNode*>) override;

 public:
  explicit ParenthesizedGrammarRule(GrammarRuleProducer* parenthesizedNode);
};

class BracedGrammarRule : public CompositeGrammarRule {
 private:
  GrammarRuleProducer* parenthesized_node_;
  SimpleAstNode* assembleNode(std::vector<SimpleAstNode*>) override;

 public:
  explicit BracedGrammarRule(GrammarRuleProducer* parenthesizedNode);
};