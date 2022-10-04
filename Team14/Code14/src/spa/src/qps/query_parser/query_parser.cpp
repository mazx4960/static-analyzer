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

QueryDeclaration *QueryParser::parseStmtRefDeclaration() {
  Token *stmtref = peekToken();
  QueryDeclaration *declaration = nullptr;
  switch (stmtref->type) {
    case TokenType::kLiteral:
      declaration = parseLiteralDeclaration();
      break;
    case TokenType::kSymbol:
      declaration = getDeclaration(nextToken());
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
      declaration = getDeclaration(nextToken());
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
      throw ParseSemanticError("Wildcard '_' is not allowed here");
    default:
      throw ParseSyntaxError("Unknown Ref: " + ref->value);
  }
  return declaration;
}

IntegerDeclaration *QueryParser::parseLiteralDeclaration() {
  Token *literal = nextToken();
  expect(literal, {TokenType::kLiteral});
  std::string literal_string = literal->value;
  if (literal_string.length() > 1 && literal_string[0] == '0') {
    throw ParseSyntaxError("INTEGER cannot have leading zero: " + literal_string);
  }
  return builder_.buildLiteral(literal_string);
}

IdentDeclaration *QueryParser::parseIdentDeclaration() {
  Token *symbol = nextToken();
  expect(symbol, {TokenType::kSymbol});
  return builder_.buildIdent(symbol->value);
}

IdentDeclaration *QueryParser::parseQuotedDeclaration() {
  expect(nextToken(), {TokenType::kQuote});
  IdentDeclaration *declaration = parseIdentDeclaration();
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
  QueryDeclaration *syn_assign = getDeclaration(synonym);
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *ent_ref = parseEntRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *expression_spec = parseExpression();
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  spdlog::debug("Pattern parsed: pattern {}({}, {})",
                syn_assign->toString() , ent_ref->toString(), expression_spec->toString());
  return builder_.buildAssignPattern(syn_assign, ent_ref, expression_spec);
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
    case RsType::kCalls:
      return parseCalls();
    case RsType::kNext:
      return parseNext();
    case RsType::kAffects:
      return parseAffects();
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
    clause = builder_.buildSuchThat(RsType::kFollowsT, first, second);
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
    clause = builder_.buildSuchThat(RsType::kParentT, first, second);
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
SuchThatClause *QueryParser::parseCalls() {
  Token *star = peekToken();
  bool calls_all = false;
  if (*star == OperatorToken("*")) {
    nextToken();
    calls_all = true;
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseEntRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseEntRefDeclaration();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});

  SuchThatClause *clause;
  if (calls_all) {
    clause = builder_.buildSuchThat(RsType::kCallsT, first, second);
  } else {
    clause = builder_.buildSuchThat(RsType::kCalls, first, second);
  }
  spdlog::debug("Calls parsed: Calls({}, {}), *: {}", first->toString(), second->toString(), calls_all);
  return clause;
}
SuchThatClause *QueryParser::parseNext() {
  Token *star = peekToken();
  bool next_all = false;
  if (*star == OperatorToken("*")) {
    nextToken();
    next_all = true;
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseStmtRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseStmtRefDeclaration();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});

  SuchThatClause *clause;
  if (next_all) {
    clause = builder_.buildSuchThat(RsType::kNextT, first, second);
  } else {
    clause = builder_.buildSuchThat(RsType::kNext, first, second);
  }
  spdlog::debug("Next parsed: Next({}, {}), *: {}", first->toString(), second->toString(), next_all);
  return clause;
}
SuchThatClause *QueryParser::parseAffects() {
  Token *star = peekToken();
  bool affects_all = false;
  if (*star == OperatorToken("*")) {
    nextToken();
    affects_all = true;
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseStmtRefDeclaration();
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseStmtRefDeclaration();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});

  SuchThatClause *clause;
  if (affects_all) {
    clause = builder_.buildSuchThat(RsType::kAffectsT, first, second);
  } else {
    clause = builder_.buildSuchThat(RsType::kAffects, first, second);
  }
  spdlog::debug("Parent parsed: Parent({}, {}), *: {}", first->toString(), second->toString(), affects_all);
  return clause;
}

QueryDeclaration *QueryParser::parseExpression() {
  bool wild_expression = false;
  Token *expr = peekToken();
  if (expr->type == TokenType::kWildCard) {
    nextToken();
    wild_expression = true;
    expr = peekToken();
  }
  if (expr->type == TokenType::kQuote) {
    std::string expression = parseFlattenedExpression();
    if (wild_expression) {
      expect(nextToken(), {TokenType::kWildCard});
      return builder_.buildWildcardExpression(expression);
    }
    return builder_.buildExpression(expression);
  }
  return builder_.buildWildcardExpression();
}

std::string QueryParser::parseFlattenedExpression() {
  expect(nextToken(), {TokenType::kQuote});
  std::string expression;
  std::vector<Token *> expr_tokens;

  while (peekToken()->type != TokenType::kQuote) {
    Token *tmp  = nextToken();
    expr_tokens.push_back(tmp);
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




