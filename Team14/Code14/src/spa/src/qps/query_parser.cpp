#include "query_parser.h"

#include <algorithm>
#include <utility>

#include "commons/parser/parser.h"
#include "qps/pql/query_keywords.h"
#include "spdlog/spdlog.h"

QueryParser::QueryParser(std::vector<Token *> tokens) { this->tokens_ = std::move(tokens); }

void QueryParser::parse() {
  while (!outOfTokens()) {
    Token *tmp = peekToken();
    if (QueryKeywords::isValidDeclarationKeyword(tmp->value)) {
      parseDeclarations();
    } else if (QueryKeywords::isValidCallKeyword(tmp->value)) {
      parseQueryCalls();
    } else if (*tmp == EndOfFileToken()) {
      break;
    } else {
      throw ParseSyntaxError("Unexpected token: " + tmp->value);
    }
  }
}

std::vector<QueryDeclaration *> QueryParser::getDeclarations() { return this->query_declarations_; }
std::vector<QueryCall *> QueryParser::getQueryCalls() { return this->query_calls_; }

Token *QueryParser::nextToken() { return tokens_[this->token_index_++]; }
Token *QueryParser::peekToken() { return tokens_[this->token_index_]; }
bool QueryParser::outOfTokens() { return this->tokens_.size() == this->token_index_; }
void QueryParser::parseDeclarations() {
  while (QueryKeywords::isValidDeclarationKeyword(peekToken()->value)) { parseDeclaration(); }
}
void QueryParser::parseDeclaration() {
  Token *token = nextToken();
  QueryDeclaration *declaration = nullptr;
  if (*token == KeywordToken("stmt")) { declaration = new StatementDeclaration(parseSynonym()); }
  if (*token == KeywordToken("variable")) { declaration = new VariableDeclaration(parseSynonym()); }
  if (*token == KeywordToken("constant")) { declaration = new ConstantDeclaration(parseSynonym()); }
  if (*token == KeywordToken("procedure")) { declaration = new ProcedureDeclaration(parseSynonym()); }
  if (*token == KeywordToken("read")) { declaration = new ReadDeclaration(parseSynonym()); }
  if (*token == KeywordToken("print")) { declaration = new PrintDeclaration(parseSynonym()); }
  if (*token == KeywordToken("call")) { declaration = new CallDeclaration(parseSynonym()); }
  if (*token == KeywordToken("while")) { declaration = new WhileDeclaration(parseSynonym()); }
  if (*token == KeywordToken("if")) { declaration = new IfDeclaration(parseSynonym()); }
  if (*token == KeywordToken("assign")) { declaration = new AssignDeclaration(parseSynonym()); }
  if (declaration == nullptr) { throw ParseSyntaxError("Unknown declaration type: " + token->value); }
  while (*peekToken() == CommaToken()) {
    nextToken();
    this->query_declarations_.push_back(new QueryDeclaration(declaration->getType(), parseSynonym()));
  }
  if (!(*nextToken() == SemicolonToken())) { throw ParseSyntaxError("Missing `;` after declaration"); }
  this->query_declarations_.push_back(declaration);
}

QueryDeclaration *QueryParser::parseStmtRefDeclaration(bool allowWild) {
  if (peekToken()->type == TokenType::kLiteral) { return parseLiteralDeclaration(); }
  if (peekToken()->type == TokenType::kSymbol) {
    QueryDeclaration *declaration = getDeclaration(nextToken()->value);
    if (declaration->getType() == EntityType::kProcedure) { return declaration; }
    for (auto type : GetAllStmtTypes()) {
      if (type == declaration->getType()) { return declaration; }
    }
    if (declaration->getType() == EntityType::kStatement) { return declaration; }
    throw ParseSemanticError("Parameter given is not a statement/procedure: "
                             + EntityTypeToString(declaration->getType()));
  }
  if (peekToken()->type == TokenType::kWildCard) {
    if (allowWild) {
      nextToken();
      return new StmtWildCardDeclaration();
    }
    throw ParseSemanticError("Wildcard '_' is not allowed here");
  }
  throw ParseSyntaxError("Unknown StmtRef: " + peekToken()->value);
}

QueryDeclaration *QueryParser::parseEntRefDeclaration(bool allowWild) {
  if (peekToken()->type == TokenType::kQuote) { return parseQuotedDeclaration(); }
  if (peekToken()->type == TokenType::kSymbol) {
    QueryDeclaration *declaration = getDeclaration(nextToken()->value);
    if (declaration->getType() == EntityType::kVariable) { return declaration; }
    throw ParseSemanticError("Parameter given is not a variable: " + EntityTypeToString(declaration->getType()));
  }
  if (peekToken()->type == TokenType::kWildCard) {
    if (allowWild) {
      nextToken();
      return new EntWildCardDeclaration();
    }
    throw ParseSemanticError("Wildcard '_' is not allowed here");
  }
  throw ParseSyntaxError("Unknown EntRef: " + peekToken()->value);
}

IntegerDeclaration *QueryParser::parseLiteralDeclaration() { return new IntegerDeclaration(nextToken()->value); }

StringDeclaration *QueryParser::parseStringDeclaration() { return new StringDeclaration(nextToken()->value); }

StringDeclaration *QueryParser::parseQuotedDeclaration() {
  if (nextToken()->type != TokenType::kQuote) { throw ParseSyntaxError("Missing '\"' before declaration"); }
  StringDeclaration *declaration = parseStringDeclaration();
  if (nextToken()->type != TokenType::kQuote) { throw ParseSyntaxError("Missing '\"' after declaration"); }
  return declaration;
}

QueryDeclaration *QueryParser::getDeclaration(const std::string &synonym) {
  auto *to_check = new QuerySynonym(synonym);
  for (QueryDeclaration *declaration : query_declarations_) {
    QuerySynonym *declaration_synonym = declaration->getSynonym();
    if (*declaration_synonym == *to_check) { return declaration; }
  }
  throw ParseSemanticError("Missing declaration: " + synonym);
}

bool QueryParser::isDeclared(const std::string &synonym) { return this->synonyms_.count(synonym) != 0U; }

QuerySynonym *QueryParser::parseSynonym() {
  if (peekToken()->type != TokenType::kSymbol) {
    throw ParseSemanticError("Invalid synonym name: " + peekToken()->value);
  }
  if (isDeclared(peekToken()->value)) { throw ParseSemanticError("Synonym already declared: " + peekToken()->value); }
  this->synonyms_.insert(peekToken()->value);
  return new QuerySynonym(nextToken()->value);
}

void QueryParser::parseQueryCalls() {
  Token *token = nextToken();
  if (*token == KeywordToken("Select")) {
    QueryDeclaration *synonym_declaration = getDeclaration(nextToken()->value);
    std::vector<QueryClause *> clause_vector;
    while (!(*peekToken() == EndOfFileToken())) { clause_vector.push_back(parseClause()); }
    query_calls_.push_back(new SelectCall(synonym_declaration, clause_vector));
  } else {
    throw ParseSyntaxError("Unknown query call: " + token->value);
  }
}
QueryClause *QueryParser::parseClause() {
  Token *token = nextToken();
  if (*token == KeywordToken("such") && *nextToken() == KeywordToken("that")) { return parseSuchThat(); }
  if (*token == KeywordToken("pattern")) { return parsePattern(); }
  throw ParseSyntaxError("Unknown clause: " + token->value);
}
PatternClause *QueryParser::parsePattern() {
  if (peekToken()->type != TokenType::kSymbol) { throw ParseSyntaxError("Missing assign synonym"); }
  QueryDeclaration *first = getDeclaration(nextToken()->value);
  if (!(*nextToken() == RoundOpenBracketToken())) { throw ParseSyntaxError("Missing '(' before parameters"); }
  QueryDeclaration *second = parseEntRefDeclaration(true);
  if (!(*nextToken() == CommaToken())) { throw ParseSyntaxError("Missing ',' between parameters"); }
  QueryDeclaration *third = parseExpression();
  if (!(*nextToken() == RoundCloseBracketToken())) { throw ParseSyntaxError("Missing ')' after parameters"); }
  spdlog::debug("Pattern parsed: " + first->toString() + "(" + second->toString() + ", " + third->toString()
                + ") expression type: " + EntityTypeToString(third->getType()));
  return new AssignPatternClause(first, second, third);
}
SuchThatClause *QueryParser::parseSuchThat() {
  Token *token = nextToken();
  if (*token == KeywordToken("Follows")) { return parseFollows(); }
  if (*token == KeywordToken("Parent")) { return parseParent(); }
  if (*token == KeywordToken("Uses")) { return parseUses(); }
  if (*token == KeywordToken("Modifies")) { return parseModifies(); }
  throw ParseSyntaxError("Unknown such-that relationship: " + token->value);
}
SuchThatClause *QueryParser::parseFollows() {
  bool follows_all = false;
  if (*peekToken() == OperatorToken("*")) {
    nextToken();
    follows_all = true;
  }
  if (!(*nextToken() == RoundOpenBracketToken())) { throw ParseSyntaxError("Missing '(' before parameters"); }
  QueryDeclaration *first = parseStmtRefDeclaration(true);
  if (!(*nextToken() == CommaToken())) { throw ParseSyntaxError("Missing ',' between parameters"); }
  QueryDeclaration *second = parseStmtRefDeclaration(true);
  ;
  if (!(*nextToken() == RoundCloseBracketToken())) { throw ParseSyntaxError("Missing ')' after parameters"); }

  if (follows_all) {
    spdlog::debug("Follows* parsed: " + first->toString() + ", " + second->toString());
    return new FollowsAllClause(first, second);
  }
  spdlog::debug("Follows parsed: " + first->toString() + ", " + second->toString());
  return new FollowsClause(first, second);
}
SuchThatClause *QueryParser::parseParent() {
  bool parent_all = false;
  if (*peekToken() == OperatorToken("*")) {
    nextToken();
    parent_all = true;
  }
  if (!(*nextToken() == RoundOpenBracketToken())) { throw ParseSyntaxError("Missing '(' before parameters"); }
  QueryDeclaration *first = parseStmtRefDeclaration(true);
  if (!(*nextToken() == CommaToken())) { throw ParseSyntaxError("Missing ',' between parameters"); }
  QueryDeclaration *second = parseStmtRefDeclaration(true);
  ;
  if (!(*nextToken() == RoundCloseBracketToken())) { throw ParseSyntaxError("Missing ')' after parameters"); }

  if (parent_all) {
    spdlog::debug("Parent* parsed: " + first->toString() + ", " + second->toString());
    return new ParentAllClause(first, second);
  }
  spdlog::debug("Parent parsed: " + first->toString() + ", " + second->toString());
  return new ParentClause(first, second);
}
SuchThatClause *QueryParser::parseUses() {
  if (!(*nextToken() == RoundOpenBracketToken())) { throw ParseSyntaxError("Missing '(' before parameters"); }
  QueryDeclaration *first;
  if (*peekToken() == QuoteToken()) {
    first = parseEntRefDeclaration(false);
  } else {
    first = parseStmtRefDeclaration(false);
  }
  if (!(*nextToken() == CommaToken())) { throw ParseSyntaxError("Missing ',' between parameters"); }
  QueryDeclaration *second = parseEntRefDeclaration(true);
  if (!(*nextToken() == RoundCloseBracketToken())) { throw ParseSyntaxError("Missing ')' after parameters"); }
  spdlog::debug("Uses parsed: " + first->toString() + ", " + second->toString());
  return new UsesClause(first, second);
}
SuchThatClause *QueryParser::parseModifies() {
  if (!(*nextToken() == RoundOpenBracketToken())) { throw ParseSyntaxError("Missing '(' before parameters"); }
  QueryDeclaration *first;
  if (*peekToken() == QuoteToken()) {
    first = parseEntRefDeclaration(false);
  } else {
    first = parseStmtRefDeclaration(false);
  }
  if (!(*nextToken() == CommaToken())) { throw ParseSyntaxError("Missing ',' between parameters"); }
  QueryDeclaration *second = parseEntRefDeclaration(true);
  ;
  if (!(*nextToken() == RoundCloseBracketToken())) { throw ParseSyntaxError("Missing ')' after parameters"); }
  spdlog::debug("Modifies parsed: " + first->toString() + ", " + second->toString());
  return new ModifiesClause(first, second);
}
QueryDeclaration *QueryParser::parseExpression() {
  bool wild_expression = false;
  std::string expression;
  Token *tmp = peekToken();
  if (tmp->type == TokenType::kWildCard) {
    nextToken();
    wild_expression = true;
    tmp = peekToken();
  }
  if (tmp->type == TokenType::kQuote) {
    nextToken();
    bool toggle = true;
    std::vector<Token *> expr_tokens;
    while (peekToken()->type != TokenType::kQuote) {
      tmp = nextToken();
      if (toggle) {
        if (tmp->type == TokenType::kSymbol || tmp->type == TokenType::kLiteral) {
          expr_tokens.push_back(tmp);
        } else {
          throw ParseSyntaxError("Unexpected symbol in expression: " + tmp->value);
        }
        toggle = false;
      } else {
        if (tmp->type == TokenType::kOperator) {
          expr_tokens.push_back(tmp);
        } else {
          throw ParseSyntaxError("Unexpected operator in expression: " + tmp->value);
        }
        toggle = true;
      }
    }
    if (toggle) { throw ParseSyntaxError("Missing symbol in expression"); }
    nextToken();
    expr_tokens.push_back(new EndOfFileToken());
    expression = Parser::ParseExpression(expr_tokens)->ToString();
    if (wild_expression) {
      if (nextToken()->type != TokenType::kWildCard) { throw ParseSyntaxError("Invalid wildcard expression"); }
      return new WildCardExpressionDeclaration(expression);
    }
    return new ExpressionDeclaration(expression);
  }
  return new StmtWildCardDeclaration();
}
