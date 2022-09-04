#include "simplegrammar.h"
#include "commons/parser/parser_exceptions.h"
#include "commons/lexer/token.h"
#include "grammarproducer.h"

ProgramGrammarRule::ProgramGrammarRule()
    : ListGrammarRule(new ProcedureGrammarProducer()) {}



ProgramNode* ProgramGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  std::vector<ProcedureNode*> procedures{};
  procedures.reserve(children.size());
  for (auto* child : children) {
    procedures.push_back(static_cast<ProcedureNode*>(child));
  }
  return new ProgramNode(procedures);
}

bool ProgramGrammarRule::shouldStop(TokenIterator token_stream) {
  return **token_stream == EndOfFileToken();
}

ProcedureGrammarRule::ProcedureGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("procedure")),
        new NodeRuleFragment(new VariableGrammarProducer()),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer()))}) {}

ProcedureNode* ProcedureGrammarRule::assembleNode(std::vector<SimpleAstNode*> child_nodes) {
  return new ProcedureNode(
      static_cast<VariableNode*>(child_nodes[0])->getName(),
      static_cast<StatementListNode*>(child_nodes[1]));
}

StatementListGrammarRule::StatementListGrammarRule()
    : ListGrammarRule(new StatementGrammarProducer()) {}

StatementListNode* StatementListGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  std::vector<StatementNode*> statements{};
  statements.reserve(children.size());
  for (auto* child : children) {
    statements.push_back(static_cast<StatementNode*>(child));
  }
  return new StatementListNode(statements);
}

bool StatementListGrammarRule::shouldStop(TokenIterator token_stream) {
  return **token_stream == EndOfFileToken() && !(
               **token_stream == KeywordToken("read")
               || **token_stream == KeywordToken("print")
               || **token_stream == KeywordToken("call")
               || **token_stream == KeywordToken("while")
               || **token_stream == KeywordToken("print")
               || **(token_stream + 1) == OperatorToken("=")
               );
}

StatementGrammarRule::StatementGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return !(**tokenStream == EndOfFileToken()) && **(tokenStream + 1) == OperatorToken("=");
        }, new AssignGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("read");
        }, new ReadGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("print");
        }, new PrintGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("call");
        }, new CallGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("while");
        }, new WhileGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream) -> bool {
          return **tokenStream == KeywordToken("if");
        }, new IfGrammarProducer()), }) {}

ReadGrammarRule::ReadGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("read")),
        new NodeRuleFragment(new VariableGrammarProducer()),
        new TokenRuleFragment(new SemicolonToken())}) {}

ReadNode* ReadGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new ReadNode(static_cast<VariableNode*>(childNodes[0]));
}

PrintGrammarRule::PrintGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("print")),
        new NodeRuleFragment(new VariableGrammarProducer()),
        new TokenRuleFragment(new SemicolonToken())}) {}

PrintNode* PrintGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new PrintNode(static_cast<VariableNode*>(childNodes[0]));
}

CallGrammarRule::CallGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("call")),
        new NodeRuleFragment(new VariableGrammarProducer()),
        new TokenRuleFragment(new SemicolonToken())}) {}

CallNode* CallGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new CallNode(static_cast<VariableNode*>(childNodes[0])->getName());
}

WhileGrammarRule::WhileGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("while")),
        new NodeRuleFragment(new ParenthesizedGrammarProducer(new CondExprGrammarProducer())),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer()))}) {}

WhileNode* WhileGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new WhileNode(
      static_cast<CondExprNode*>(childNodes[0]),
      static_cast<StatementListNode*>(childNodes[1]));
}

IfGrammarRule::IfGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new LiteralToken("if")),
        new NodeRuleFragment(new ParenthesizedGrammarProducer(new CondExprGrammarProducer())),
        new TokenRuleFragment(new LiteralToken("then")),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer())),
        new TokenRuleFragment(new LiteralToken("else")),
        new NodeRuleFragment(new BracedGrammarProducer(new StatementListGrammarProducer()))}) {}

IfNode* IfGrammarRule::assembleNode(std::vector<SimpleAstNode*> childNodes) {
  return new IfNode(
      static_cast<CondExprNode*>(childNodes[0]),
      static_cast<StatementListNode*>(childNodes[1]),
      static_cast<StatementListNode*>(childNodes[2]));
}

AssignGrammarRule::AssignGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new NodeRuleFragment(new VariableGrammarProducer()),
        new TokenRuleFragment(new OperatorToken("=")),
        new NodeRuleFragment(new RelFactorGrammarProducer()),
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
        }, new RelExprGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return **tokenStream == OperatorToken("!");
        }, new NotExprGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return **tokenStream == RoundOpenBracketToken();
        }, new BinaryCondGrammarProducer()),
    }) {}

NotExprGrammarRule::NotExprGrammarRule()
    : CompositeGrammarRule(std::vector<RuleFragment*>{
        new TokenRuleFragment(new OperatorToken("!")),
        new NodeRuleFragment(new ParenthesizedGrammarProducer(new CondExprGrammarProducer()))
    }) {}

NotExprNode* NotExprGrammarRule::assembleNode(std::vector<SimpleAstNode*> children) {
  return static_cast<NotExprNode*>(children[0]);
}

BinaryCondGrammarRule::BinaryCondGrammarRule()
    : LateChoiceGrammarRule(
        new ParenthesizedGrammarProducer(new CondExprGrammarProducer()),
        std::vector<std::pair<Token*, MergeFunction>>{
            std::make_pair(new OperatorToken("&&"), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new AndExprNode(static_cast<CondExprNode*>(first), static_cast<CondExprNode*>(second));
            }), std::make_pair(new OperatorToken("||"), [](SimpleAstNode* first, SimpleAstNode* second) -> SimpleAstNode* {
              return new OrExprNode(static_cast<CondExprNode*>(first), static_cast<CondExprNode*>(second));
            })
        }) {}

RelExprGrammarRule::RelExprGrammarRule()
    : LateChoiceGrammarRule(
        new RelFactorGrammarProducer(),
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
        }, new ExprGrammarProducer()),
        std::make_pair([](TokenIterator  /*tokenStream*/)-> bool {
          return true;
        }, new ReferenceGrammarProducer()),
    }) {}

ExprGrammarRule::ExprGrammarRule()
    : RecursiveGrammarRule(
        new TermGrammarProducer(),
        std::vector<std::pair<Token*, MergeFunction>>{
            std::make_pair(new OperatorToken("+"), [](SimpleAstNode* base, SimpleAstNode* next) -> SimpleAstNode* {
              return new PlusNode(static_cast<ExprNode*>(base), static_cast<ExprNode*>(next));
            }), std::make_pair(new OperatorToken("-"), [](SimpleAstNode* base, SimpleAstNode* next) -> SimpleAstNode* {
              return new MinusNode(static_cast<ExprNode*>(base), static_cast<ExprNode*>(next));
            })}) {}

TermGrammarRule::TermGrammarRule()
    : RecursiveGrammarRule(
        new FactorGrammarProducer(),
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
        }, new ParenthesizedGrammarProducer(new ExprGrammarProducer())),
        std::make_pair([](TokenIterator  /*tokenStream*/)-> bool {
          return true;
        }, new ReferenceGrammarProducer()),
    }) {}

ReferenceGrammarRule::ReferenceGrammarRule()
    : EarlyChoiceGrammarRule(std::vector<ConditionalRule>{
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return (*tokenStream)->type == TokenType::kSymbol;
        }, new VariableGrammarProducer()),
        std::make_pair([](TokenIterator tokenStream)-> bool {
          return (*tokenStream)->type == TokenType::kLiteral;
        }, new ConstantGrammarProducer()),
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