// Copyright 2022 CS3203 Team14. All rights reserved.

#include "expr_grammar.h"

#include "commons/parser/parser_exceptions.h"
#include "expr_grammar_producer.h"

RelFactorGrammarRule::RelFactorGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
    std::make_pair(
        [](TokenIterator tokenStream) -> bool {
          if (**tokenStream == RoundOpenBracketToken())
            return true;
          if (**tokenStream == EndOfFileToken())
            return false;
          return **(tokenStream + 1) == OperatorToken("+") || **(tokenStream + 1) == OperatorToken("-")
              || **(tokenStream + 1) == OperatorToken("*") || **(tokenStream + 1) == OperatorToken("/")
              || **(tokenStream + 1) == OperatorToken("%");
        },
        new ExprGrammarProducer()),
    std::make_pair([](TokenIterator /*tokenStream*/) -> bool {
      return true;
    }, new ReferenceGrammarProducer()),
}) {
}

ExprGrammarRule::ExprGrammarRule()
    : RecursiveGrammarRule(new TermGrammarProducer(),
                           std::vector<std::pair<Token *, MergeFunction>>{
                               std::make_pair(new OperatorToken("+"),
                                              [](Node *base, Node *next) -> Node * {
                                                return new PlusNode(static_cast<ExprNode *>(base),
                                                                    static_cast<ExprNode *>(next));
                                              }),
                               std::make_pair(new OperatorToken("-"), [](Node *base, Node *next) -> Node * {
                                 return new MinusNode(static_cast<ExprNode *>(base), static_cast<ExprNode *>(next));
                               })}) {
}

TermGrammarRule::TermGrammarRule()
    : RecursiveGrammarRule(
    new FactorGrammarProducer(),
    std::vector<std::pair<Token *, MergeFunction>>{
        std::make_pair(new OperatorToken("*"),
                       [](Node *base, Node *next) -> Node * {
                         return new TimesNode(static_cast<ExprNode *>(base), static_cast<ExprNode *>(next));
                       }),
        std::make_pair(new OperatorToken("/"),
                       [](Node *base, Node *next) -> Node * {
                         return new DivNode(static_cast<ExprNode *>(base), static_cast<ExprNode *>(next));
                       }),
        std::make_pair(new OperatorToken("%"), [](Node *base, Node *next) -> Node * {
          return new ModNode(static_cast<ExprNode *>(base), static_cast<ExprNode *>(next));
        })}) {
}

FactorGrammarRule::FactorGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
    std::make_pair([](TokenIterator tokenStream) -> bool {
                     return **tokenStream == RoundOpenBracketToken();
                   },
                   new ParenthesizedGrammarProducer(new ExprGrammarProducer())),
    std::make_pair([](TokenIterator /*tokenStream*/) -> bool {
      return true;
    }, new ReferenceGrammarProducer()),
}) {
}

ReferenceGrammarRule::ReferenceGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
    std::make_pair([](TokenIterator tokenStream) -> bool {
                     return (*tokenStream)->type == TokenType::kSymbol;
                   },
                   new VariableGrammarProducer()),
    std::make_pair([](TokenIterator tokenStream) -> bool {
                     return (*tokenStream)->type == TokenType::kLiteral;
                   },
                   new ConstantGrammarProducer()),
}) {
}

VariableNode *VariableGrammarRule::parseNode(TokenIterator &tokenStream) {
  if ((*tokenStream)->type == TokenType::kSymbol) {
    return new VariableNode((*tokenStream++)->value);
  }
  throw ParseSyntaxError("Expected variable, got " + (*tokenStream++)->value);
}

ConstantNode *ConstantGrammarRule::parseNode(TokenIterator &tokenStream) {
  if ((*tokenStream)->type == TokenType::kLiteral) {
    return new ConstantNode(((*tokenStream++)->value));
  }
  throw ParseSyntaxError("Expected constant, got " + (*tokenStream++)->value);
}