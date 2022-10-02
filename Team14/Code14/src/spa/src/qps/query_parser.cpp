#include "query_parser.h"

#include <algorithm>
#include <utility>
#include <functional>

#include "commons/parser/parser.h"
#include "qps/pql/query_keywords.h"
#include "spdlog/spdlog.h"

QueryParser::QueryParser(std::vector<Token *> tokens, QueryBuilder builder) {
  this->builder_ = std::move(builder);
  this->tokens_ = std::move(tokens);
}

Query *QueryParser::parse() {
  while (!outOfTokens()) {
    Token *tmp = peekToken();
    if (QueryKeywords::isValidDeclarationKeyword(tmp->value)) {
      parseDeclarations();
    } else if (QueryKeywords::isValidCallKeyword(tmp->value)) {
      parseQueryCall();
    } else if (*tmp == EndOfFileToken()) {
      break;
    } else {
      throw ParseSyntaxError("Unexpected token: " + tmp->value);
    }
  }
  return builder_.build();
}

Token *QueryParser::nextToken() {
  if (token_index_ >= tokens_.size() || *peekToken() == EndOfFileToken()) {
    throw ParseSyntaxError("Too few arguments");
  }
  return tokens_[this->token_index_++];
}
Token *QueryParser::peekToken() {
  return tokens_[this->token_index_];
}
bool QueryParser::outOfTokens() {
  return this->token_index_ == this->tokens_.size();
}

void QueryParser::parseDeclarations() {
  while (QueryKeywords::isValidDeclarationKeyword(peekToken()->value)) {
    parseDeclaration();
  }
}

void QueryParser::parseDeclaration() {
  Token *prefix = nextToken();
  EntityType type;
  try {
    type = QueryKeywords::declarationKeywordToType(prefix->value);
  }
  catch (std::out_of_range& oor) {
    throw ParseSyntaxError("Unknown declaration type: " + prefix->value);
  }
  builder_.buildDeclaration(type, parseSynonym());

  // While there are multiple declarations seperated by commas
  while (*peekToken() == CommaToken()) {
    nextToken();
    builder_.buildDeclaration(type, parseSynonym());
  }

  // End declaration with a semicolon
  expect(nextToken(), {TokenType::kSemicolon});
}

QueryDeclaration *QueryParser::getDeclaration(Token *synonym) {
  return builder_.getDeclaration(synonym->value);
}

QueryDeclaration *QueryParser::getStmtDeclaration(Token *synonym) {
  return builder_.getStmtDeclaration(synonym->value);
}

QueryDeclaration *QueryParser::getEntDeclaration(Token *synonym) {
  return builder_.getEntDeclaration(synonym->value);
}

QueryDeclaration *QueryParser::parseStmtRefDeclaration() {
  Token *stmtref = peekToken();
  QueryDeclaration *declaration = nullptr;
  switch (stmtref->type) {
    case TokenType::kLiteral:
      declaration = parseLiteralDeclaration();
      break;
    case TokenType::kSymbol:
      declaration = getStmtDeclaration(nextToken());
      break;
    case TokenType::kWildCard:
      declaration = parseWildcard(EntityType::kWildcardStmt);
      break;
    default:
      throw ParseSyntaxError("Unknown StmtRef: " + stmtref->value);
  }
  return declaration;
}

QueryDeclaration *QueryParser::parseEntRefDeclaration() {
  Token *entref = peekToken();
  QueryDeclaration *declaration = nullptr;
  switch (entref->type) {
    case TokenType::kQuote:
      declaration = parseQuotedDeclaration();
      break;
    case TokenType::kSymbol:
      declaration = getEntDeclaration(nextToken());
      break;
    case TokenType::kWildCard:
      declaration = parseWildcard(EntityType::kWildcardEnt);
      break;
    default:
      throw ParseSyntaxError("Unknown EntRef: " + entref->value);
  }
  return declaration;
}

QueryDeclaration *QueryParser::parseAnyRefDeclaration() {
  Token *ref = peekToken();
  QueryDeclaration *declaration = nullptr;
  switch (ref->type) {
    case TokenType::kQuote:
      declaration = parseQuotedDeclaration();
      break;
    case TokenType::kLiteral:
      declaration = parseLiteralDeclaration();
      break;
    case TokenType::kSymbol:
      declaration = getDeclaration(nextToken());
      break;
    case TokenType::kWildCard:
      throw ParseSyntaxError("Wildcard '_' is not allowed here");
    default:
      throw ParseSyntaxError("Unknown Ref: " + ref->value);
  }
  return declaration;
}

IntegerDeclaration *QueryParser::parseLiteralDeclaration() {
  Token *literal = nextToken();
  expect(literal, {TokenType::kLiteral});
  if (std::stoi(literal->value) <= 0) {
    throw ParseSyntaxError("Integer cannot be negative/zero: " + literal->value);
  }
  return builder_.buildLiteral(literal->value);
}

StringDeclaration *QueryParser::parseStringDeclaration() {
  Token *symbol = nextToken();
  expect(symbol, {TokenType::kSymbol});
  return builder_.buildString(symbol->value);
}

StringDeclaration *QueryParser::parseQuotedDeclaration() {
  expect(nextToken(), {TokenType::kQuote});
  StringDeclaration *declaration = parseStringDeclaration();
  expect(nextToken(), {TokenType::kQuote});
  return declaration;
}

QuerySynonym *QueryParser::parseSynonym() {
  Token *synonym = nextToken();
  expect(synonym, {TokenType::kSymbol});
  return builder_.buildSynonym(synonym->value);
}

QueryCall *QueryParser::parseQueryCall() {
  Token *call = nextToken();
  if (!QueryKeywords::isValidCallKeyword(call->value)) {
    throw ParseSyntaxError("Unknown query call: " + call->value);
  }
  QueryDeclaration *synonym_declaration = getDeclaration(nextToken());
  std::vector<QueryClause *> clause_vector;
  while (*peekToken() != EndOfFileToken()) {
    clause_vector.push_back(parseClause());
  }
  return builder_.buildSelectCall(synonym_declaration, clause_vector);
}
QueryClause *QueryParser::parseClause() {
  Token *clause = nextToken();
  if (*clause == KeywordToken("such") && *nextToken() == KeywordToken("that")) {
    return parseSuchThat();
  }
  if (*clause == KeywordToken("pattern")) {
    return parsePattern();
  }
  throw ParseSyntaxError("Unknown clause: " + clause->value);
}
PatternClause *QueryParser::parsePattern() {
  Token *synonym = nextToken();
  expect(synonym, {TokenType::kSymbol});
  QueryDeclaration *pattern_synonym = getDeclaration(synonym);
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first_param = parseEntRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second_param = parseExpression();
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  spdlog::debug("Pattern parsed: pattern {}({}, {})",
                pattern_synonym->toString() ,first_param->toString(), second_param->toString());
  return builder_.buildAssignPattern(pattern_synonym, first_param, second_param);
}
SuchThatClause *QueryParser::parseSuchThat() {
  Token *relationship = nextToken();
  RsType rs_type;
  try {
    rs_type = QueryKeywords::relationshipKeywordToType(relationship->value);
  }
  catch (std::out_of_range& oor) {
    throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
  switch (rs_type) {
    case RsType::kFollows:
      return parseFollows();
    case RsType::kParent:
      return parseParent();
    case RsType::kUses:
      return parseUses();
    case RsType::kModifies:
      return parseModifies();
    default:
      throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
}

SuchThatClause *QueryParser::parseFollows() {
  Token *star = peekToken();
  bool follows_all = false;
  if (*star == OperatorToken("*")) {
    nextToken();
    follows_all = true;
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseStmtRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseStmtRefDeclaration();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});

  SuchThatClause *clause;
  if (follows_all) {
    clause = builder_.buildSuchThat(RsType::kFollowsAll, first, second);
  } else {
    clause = builder_.buildSuchThat(RsType::kFollows, first, second);
  }
  spdlog::debug("Follows parsed: Follows({}, {}), *: {}", first->toString(), second->toString(), follows_all);

  return clause;
}

SuchThatClause *QueryParser::parseParent() {
  Token *star = peekToken();
  bool parent_all = false;
  if (*star == OperatorToken("*")) {
    nextToken();
    parent_all = true;
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseStmtRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseStmtRefDeclaration();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});

  SuchThatClause *clause;
  if (parent_all) {
    clause = builder_.buildSuchThat(RsType::kParentAll, first, second);
  } else {
    clause = builder_.buildSuchThat(RsType::kParent, first, second);
  }
  spdlog::debug("Parent parsed: Parent({}, {}), *: {}", first->toString(), second->toString(), parent_all);
  return clause;
}
SuchThatClause *QueryParser::parseUses() {
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseAnyRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseEntRefDeclaration();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  spdlog::debug("Uses parsed: Uses({}, {})", first->toString(), second->toString());
  return builder_.buildSuchThat(RsType::kUses, first, second);
}

SuchThatClause *QueryParser::parseModifies() {
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseAnyRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseEntRefDeclaration();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  spdlog::debug("Modifies parsed: Modifies({}, {})", first->toString(), second->toString());
  return builder_.buildSuchThat(RsType::kModifies, first, second);
}

QueryDeclaration *QueryParser::parseExpression() {
  bool wild_expression = false;
  Token *tmp = peekToken();
  if (tmp->type == TokenType::kWildCard) {
    nextToken();
    wild_expression = true;
    tmp = peekToken();
  }
  if (tmp->type == TokenType::kQuote) {
    std::string expression = parseFlattenedExpression();
    if (wild_expression) {
      expect(nextToken(), {TokenType::kWildCard});
      return builder_.buildWildcardExpression(expression);
    }
    return builder_.buildExpression(expression);
  }
  return builder_.buildWildcardStmt();
}

std::string QueryParser::parseFlattenedExpression() {
  expect(nextToken(), {TokenType::kQuote});
  std::string expression;
  bool expect_operand = true;
  std::vector<Token *> expr_tokens;
  while (peekToken()->type != TokenType::kQuote) {
    Token *tmp  = nextToken();
    if (expect_operand) {
      expect(tmp, {TokenType::kSymbol, TokenType::kLiteral});
      expr_tokens.push_back(tmp);
      expect_operand = false;
    } else {
      expect(tmp, {TokenType::kOperator});
      expr_tokens.push_back(tmp);
      expect_operand = true;
    }
  }
  if (expect_operand) {
    expect(peekToken(), {TokenType::kSymbol});
  }
  expect(nextToken(), {TokenType::kQuote});
  expr_tokens.push_back(new EndOfFileToken());
  return Parser::ParseExpression(expr_tokens)->ToString();
}

QueryDeclaration *QueryParser::parseWildcard(EntityType type) {
  Token *wildcard = nextToken();
  expect(wildcard, {TokenType::kWildCard});
  switch (type) {
    case EntityType::kWildcardStmt:
      return builder_.buildWildcardStmt();
    case EntityType::kWildcardEnt:
      return builder_.buildWildcardEnt();
    default:
      throw ParseSyntaxError("Wrong usage");
  }
}
std::vector<QueryDeclaration *> QueryParser::getDeclarations() {
  return this->builder_.getDeclarations();
}

QueryCall *QueryParser::getQueryCall() {
  return this->builder_.getQueryCall();
}

void QueryParser::expect(Token *token, const std::unordered_set<TokenType>& expected_types) {
  if (expected_types.count(token->type) == 0) {
    throw ParseSyntaxError("Invalid token type: " + token->value);
  }
}



