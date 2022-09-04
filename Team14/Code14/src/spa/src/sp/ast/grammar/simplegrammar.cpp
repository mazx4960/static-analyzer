#include "simplegrammar.h"
#include "commons/parser/parser_exceptions.h"
#include "commons/token/token.h"

SimpleAstNode* CompositeGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<SimpleAstNode*> child_nodes{};
  for (auto *rule_fragment : ruleFragments_) {
    rule_fragment->parseStream(tokenStream, child_nodes);
  }
  return assembleNode(child_nodes);
}

ProgramNode* ProgramGrammarRule::parseNode(TokenIterator& tokenStream) {
  std::vector<ProcedureNode*> procedures{};
  ProcedureGrammarRule procedure_parser{};
  do {
    procedures.push_back(static_cast<ProcedureNode*>(procedure_parser.parseNode(tokenStream)));
  } while (!(**tokenStream == EndOfFileToken()));
  return new ProgramNode(procedures);
}

ProcedureGrammarRule::ProcedureGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("procedure")),
        new NodeRuleFragment(new VariableGrammarRule()),
        new TokenRuleFragment(new RoundOpenBracketToken()),
        new NodeRuleFragment(new StatementGrammarRule()),
        new TokenRuleFragment(new RoundCloseBracketToken())}) {}

ProcedureNode* ProcedureGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new ProcedureNode(
      static_cast<VariableNode*>(childNodes[0])->getName(),
      static_cast<StatementListNode*>(childNodes[1]));
}

StatementGrammarRule::StatementGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return !(**tokenStream == EndOfFileToken()) && **(tokenStream + 1) == OperatorToken("=");
        }, new AssignGrammarRule()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("read");
        }, new ReadGrammarRule()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("print");
        }, new PrintGrammarRule()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("call");
        }, new CallGrammarRule()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("while");
        }, new WhileGrammarRule()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("if");
        }, new IfGrammarRule()), }) {}

ReadGrammarRule::ReadGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("read")),
        new NodeRuleFragment(new VariableGrammarRule()),
        new TokenRuleFragment(new SemicolonToken())}) {}

ReadNode* ReadGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new ReadNode(static_cast<VariableNode*>(childNodes[0]));
}

PrintGrammarRule::PrintGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("print")),
        new NodeRuleFragment(new VariableGrammarRule()),
        new TokenRuleFragment(new SemicolonToken())}) {}

PrintNode* PrintGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new PrintNode(static_cast<VariableNode*>(childNodes[0]));
}

CallGrammarRule::CallGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("call")),
        new NodeRuleFragment(new VariableGrammarRule()),
        new TokenRuleFragment(new SemicolonToken())}) {}

CallNode* CallGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new CallNode(static_cast<VariableNode*>(childNodes[0])->getName());
}

WhileGrammarRule::WhileGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("while")),
        new TokenRuleFragment(new RoundOpenBracketToken()),
        new NodeRuleFragment(new CondExprGrammarRule()),
        new TokenRuleFragment(new RoundCloseBracketToken()),
        new TokenRuleFragment(new CurlyOpenBracketToken()),
        new NodeRuleFragment(new StatementListGrammarRule()),
        new TokenRuleFragment(new CurlyCloseBracket())}) {}

WhileNode* WhileGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new WhileNode(
      static_cast<CondExprNode*>(childNodes[0]),
      static_cast<StatementListNode*>(childNodes[1]));
}

IfGrammarRule::IfGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("if")),
        new TokenRuleFragment(new RoundOpenBracketToken()),
        new NodeRuleFragment(new CondExprGrammarRule()),
        new TokenRuleFragment(new RoundCloseBracketToken()),
        new TokenRuleFragment(new LiteralToken("then")),
        new TokenRuleFragment(new CurlyOpenBracketToken()),
        new NodeRuleFragment(new StatementListGrammarRule()),
        new TokenRuleFragment(new CurlyCloseBracket()),
        new TokenRuleFragment(new LiteralToken("else")),
        new TokenRuleFragment(new CurlyOpenBracketToken()),
        new NodeRuleFragment(new StatementListGrammarRule()),
        new TokenRuleFragment(new CurlyCloseBracket())}) {}

IfNode* IfGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new IfNode(
      static_cast<CondExprNode*>(childNodes[0]),
      static_cast<StatementListNode*>(childNodes[1]),
      static_cast<StatementListNode*>(childNodes[2]));
}

AssignGrammarRule::AssignGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new NodeRuleFragment(new VariableGrammarRule()),
        new TokenRuleFragment(new OperatorToken("=")),
        new NodeRuleFragment(new RelFactorGrammarRule()),
        new TokenRuleFragment(new SemicolonToken())}) {}

AssignNode* AssignGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new AssignNode(
      static_cast<VariableNode*>(childNodes[0]),
      static_cast<RelFactorNode*>(childNodes[1]));
}

CondExprGrammarRule::CondExprGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return (**tokenStream).type == TokenType::kSymbol;
        }, new RelExprGrammarRule()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return **tokenStream == OperatorToken("!");
        }, new NotExprGrammarRule()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return **tokenStream == RoundOpenBracketToken();
        }, new BinaryCondGrammarRule()),
    }) {}

RelFactorGrammarRule::RelFactorGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream)-> bool {
          if (**tokenStream == RoundOpenBracketToken()) return true;
          if (**tokenStream == EndOfFileToken()) return false;
          return **(tokenStream) == OperatorToken("+")
              || **(tokenStream) == OperatorToken("-")
              || **(tokenStream) == OperatorToken("*")
              || **(tokenStream) == OperatorToken("/")
              || **(tokenStream) == OperatorToken("%");
        }, new ExprGrammarRule()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return true;
        }, new ReferenceGrammarRule()),
    }) {}

FactorGrammarRule::FactorGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return **tokenStream == RoundOpenBracketToken();
        }, new ExprGrammarRule()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return true;
        }, new ReferenceGrammarRule()),
    }) {}

ReferenceGrammarRule::ReferenceGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return (*tokenStream)->type == TokenType::kSymbol;
        }, new VariableGrammarRule()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return (*tokenStream)->type == TokenType::kLiteral;
        }, new ConstantGrammarRule()),
    }) {}

VariableNode* VariableGrammarRule::parseNode(TokenIterator& tokenStream) {
  if ((*tokenStream)->type == TokenType::kSymbol) {
    return new VariableNode((*tokenStream++)->value);
  }
  throw ParseSyntaxError("Expected variable, got " + (*tokenStream++)->value);
}

ConstantNode* ConstantGrammarRule::parseNode(TokenIterator& tokenStream) {
  if ((*tokenStream)->type == TokenType::kLiteral) {
    return new ConstantNode(std::stoi((*tokenStream++)->value));
  }
  throw ParseSyntaxError("Expected constant, got " + (*tokenStream++)->value);
}