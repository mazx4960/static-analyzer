#include "simplegrammar.h"
#include "commons/parser/parser_exceptions.h"
#include "commons/token/token.h"

ProgramGrammarRule::ProgramGrammarRule()
    : ListGrammarRule(new ProcedureGrammarRule()) {}



ProgramNode* ProgramGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  std::vector<ProcedureNode*> procedures{};
  for (auto* child : children) {
    procedures.push_back(static_cast<ProcedureNode*>(child));
  }
  return new ProgramNode(procedures);
}

bool ProgramGrammarRule::shouldStop(TokenIterator tokenStream) {
  return **tokenStream == EndOfFileToken();
}

ProcedureGrammarRule::ProcedureGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("procedure")),
        new NodeRuleFragment(new VariableGrammarRule()),
        new NodeRuleFragment(new BracedGrammarRule(new StatementGrammarRule()))}) {}

ProcedureNode* ProcedureGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new ProcedureNode(
      static_cast<VariableNode*>(childNodes[0])->getName(),
      static_cast<StatementListNode*>(childNodes[1]));
}

StatementListGrammarRule::StatementListGrammarRule()
    : ListGrammarRule(new StatementGrammarRule()) {}

StatementListNode* StatementListGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  std::vector<StatementNode*> statements{};
  for (auto* child : children) {
    statements.push_back(static_cast<StatementNode*>(child));
  }
  return new StatementListNode(statements);
}

bool StatementListGrammarRule::shouldStop(TokenIterator tokenStream) {
  return **tokenStream == EndOfFileToken() && !(
               **tokenStream == KeywordToken("read")
               || **tokenStream == KeywordToken("print")
               || **tokenStream == KeywordToken("call")
               || **tokenStream == KeywordToken("while")
               || **tokenStream == KeywordToken("print")
               || **(tokenStream + 1) == OperatorToken("=")
               );
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
        new NodeRuleFragment(new ParenthesizedGrammarRule(new CondExprGrammarRule())),
        new NodeRuleFragment(new BracedGrammarRule(new StatementListGrammarRule()))}) {}

WhileNode* WhileGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new WhileNode(
      static_cast<CondExprNode*>(childNodes[0]),
      static_cast<StatementListNode*>(childNodes[1]));
}

IfGrammarRule::IfGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("if")),
        new NodeRuleFragment(new ParenthesizedGrammarRule(new CondExprGrammarRule())),
        new TokenRuleFragment(new LiteralToken("then")),
        new NodeRuleFragment(new BracedGrammarRule(new StatementListGrammarRule())),
        new TokenRuleFragment(new LiteralToken("else")),
        new NodeRuleFragment(new BracedGrammarRule(new StatementListGrammarRule()))}) {}

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

NotExprGrammarRule::NotExprGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new OperatorToken("!")),
        new NodeRuleFragment(new ParenthesizedGrammarRule(new CondExprGrammarRule()))
    }) {}

NotExprNode* NotExprGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  return static_cast<NotExprNode*>(children[0]);
}

BinaryCondGrammarRule::BinaryCondGrammarRule()
    : LateChoiceGrammarRule(
        []() -> SimpleGrammarRule* { return new ParenthesizedGrammarRule(new CondExprGrammarRule()); },
        std::vector<std::pair<Token*, MergeFunction>>{
            std::make_pair(new OperatorToken("&&"), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new AndExprNode(static_cast<CondExprNode*>(first), static_cast<CondExprNode*>(second));
            }), std::make_pair(new OperatorToken("||"), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new OrExprNode(static_cast<CondExprNode*>(first), static_cast<CondExprNode*>(second));
            })
        }) {}

RelExprGrammarRule::RelExprGrammarRule()
    : LateChoiceGrammarRule(
        []() -> SimpleGrammarRule* { return new RelFactorGrammarRule(); },
        std::vector<std::pair<Token*, MergeFunction>>{
            std::make_pair(new OperatorToken(">"), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new GreaterThanNode(static_cast<RelFactorNode*>(first), static_cast<RelFactorNode*>(second));
            }),
            std::make_pair(new OperatorToken(">="), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new GreaterThanEqualNode(static_cast<RelFactorNode*>(first), static_cast<RelFactorNode*>(second));
            }),
            std::make_pair(new OperatorToken("<"), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new LessThanNode(static_cast<RelFactorNode*>(first), static_cast<RelFactorNode*>(second));
            }),
            std::make_pair(new OperatorToken("<="), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new LessThanEqualNode(static_cast<RelFactorNode*>(first), static_cast<RelFactorNode*>(second));
            }),
            std::make_pair(new OperatorToken("=="), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new EqualNode(static_cast<RelFactorNode*>(first), static_cast<RelFactorNode*>(second));
            }),
            std::make_pair(new OperatorToken("!="), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new NotEqualNode(static_cast<RelFactorNode*>(first), static_cast<RelFactorNode*>(second));
            }),
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

ExprGrammarRule::ExprGrammarRule()
    : RecursiveGrammarRule(
        []() -> SimpleGrammarRule* { return new TermGrammarRule(); },
        std::vector<std::pair<Token*, MergeFunction>>{
            std::make_pair(new OperatorToken("+"), [](SimpleAstNode* base, SimpleAstNode* next) -> SimpleAstNode* {
              return new PlusNode(static_cast<ExprNode*>(base), static_cast<ExprNode*>(next));
            }), std::make_pair(new OperatorToken("-"), [](SimpleAstNode* base, SimpleAstNode* next) -> SimpleAstNode* {
              return new MinusNode(static_cast<ExprNode*>(base), static_cast<ExprNode*>(next));
            })}) {}

TermGrammarRule::TermGrammarRule()
    : RecursiveGrammarRule(
        []() -> SimpleGrammarRule* { return new FactorGrammarRule(); },
        std::vector<std::pair<Token*, MergeFunction>>{
            std::make_pair(new OperatorToken("*"), [](SimpleAstNode* base, SimpleAstNode* next) -> SimpleAstNode* {
              return new TimesNode(static_cast<ExprNode*>(base), static_cast<ExprNode*>(next));
            }),std::make_pair(new OperatorToken("/"), [](SimpleAstNode* base, SimpleAstNode* next) -> SimpleAstNode* {
              return new DivNode(static_cast<ExprNode*>(base), static_cast<ExprNode*>(next));
            }),
            std::make_pair(new OperatorToken("%"), [](SimpleAstNode* base, SimpleAstNode* next) -> SimpleAstNode* {
              return new MinusNode(static_cast<ExprNode*>(base), static_cast<ExprNode*>(next));
            })}) {}

FactorGrammarRule::FactorGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return **tokenStream == RoundOpenBracketToken();
        }, new ParenthesizedGrammarRule(new ExprGrammarRule())),
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