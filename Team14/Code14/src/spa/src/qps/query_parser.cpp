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

Query QueryParser::parse() {
  while (!outOfTokens()) {
    Token *tmp = peekToken();
    if (QueryKeywords::isValidDeclarationKeyword(tmp->value)) {
      parseDeclarations();
    } else if (QueryKeywords::isValidCallKeyword(tmp->value)) {
      parseQueryCall(nextToken());
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
    parseDeclaration(nextToken());
  }
}

void QueryParser::parseDeclaration(Token *prefix) {
  EntityType type;
  try {
    type = QueryKeywords::declarationKeywordToType(prefix->value);
  }
  catch (std::out_of_range& oor) {
    throw ParseSyntaxError("Unknown declaration type: " + prefix->value);
  }
  builder_.buildDeclaration(type, parseSynonym(nextToken()));

  // While there are multiple declarations seperated by commas
  while (*peekToken() == CommaToken()) {
    nextToken();
    builder_.buildDeclaration(type, parseSynonym(nextToken()));
  }

  // End declaration with a semicolon
  if (*nextToken() != SemicolonToken()) {
    throw ParseSyntaxError("Missing `;` after declaration");
  }
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

QueryDeclaration *QueryParser::parseStmtRefDeclaration(Token *stmtref) {
  QueryDeclaration *declaration = nullptr;
  switch (stmtref->type) {
    case TokenType::kLiteral:
      declaration = parseLiteralDeclaration(nextToken());
      break;
    case TokenType::kSymbol:
      declaration = getStmtDeclaration(nextToken());
      break;
    case TokenType::kWildCard:
      declaration = parseWildcard(EntityType::kWildcardStmt, nextToken());
      break;
    default:
      throw ParseSyntaxError("Unknown StmtRef: " + stmtref->value);
  }
  return declaration;
}

QueryDeclaration *QueryParser::parseEntRefDeclaration(Token *entref) {
  QueryDeclaration *declaration = nullptr;
  switch (entref->type) {
    case TokenType::kQuote:
      declaration = parseQuotedDeclaration();
      break;
    case TokenType::kSymbol:
      declaration = getEntDeclaration(nextToken());
      break;
    case TokenType::kWildCard:
      declaration = parseWildcard(EntityType::kWildcardEnt, nextToken());
      break;
    default:
      throw ParseSyntaxError("Unknown EntRef: " + entref->value);
  }
  return declaration;
}

QueryDeclaration *QueryParser::parseAnyRefDeclaration(Token *ref) {
  QueryDeclaration *declaration = nullptr;
  switch (ref->type) {
    case TokenType::kQuote:
      declaration = parseQuotedDeclaration();
      break;
    case TokenType::kLiteral:
      declaration = parseLiteralDeclaration(nextToken());
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

IntegerDeclaration *QueryParser::parseLiteralDeclaration(Token *literal) {
  if (std::stoi(literal->value) <= 0) {
    throw ParseSyntaxError("Integer cannot be negative/zero: " + literal->value);
  }
  return builder_.buildLiteral(literal->value);
}

StringDeclaration *QueryParser::parseStringDeclaration(Token *symbol) {
  if (symbol->type != TokenType::kSymbol) {
    throw ParseSyntaxError("Invalid string: " + symbol->value);
  }
  return builder_.buildString(symbol->value);
}

StringDeclaration *QueryParser::parseQuotedDeclaration() {
  if (nextToken()->type != TokenType::kQuote) {
    throw ParseSyntaxError("Missing '\"' before declaration");
  }
  StringDeclaration *declaration = parseStringDeclaration(nextToken());
  if (nextToken()->type != TokenType::kQuote) {
    throw ParseSyntaxError("Missing '\"' after declaration");
  }
  return declaration;
}

QuerySynonym *QueryParser::parseSynonym(Token *synonym) {
  if (synonym->type != TokenType::kSymbol) {
    throw ParseSyntaxError("Invalid synonym name: " + synonym->value);
  }
  return builder_.buildSynonym(synonym->value);
}

QueryCall *QueryParser::parseQueryCall(Token *call) {
  if (!QueryKeywords::isValidCallKeyword(call->value)) {
    throw ParseSyntaxError("Unknown query call: " + call->value);
  }
  QueryDeclaration *synonym_declaration = getDeclaration(nextToken());
  std::vector<QueryClause *> clause_vector;
  while (*peekToken() != EndOfFileToken()) {
    clause_vector.push_back(parseClause(nextToken()));
  }
  return builder_.buildSelectCall(synonym_declaration, clause_vector);
}
QueryClause *QueryParser::parseClause(Token *clause) {
  if (*clause == KeywordToken("such") && *nextToken() == KeywordToken("that")) {
    return parseSuchThat(nextToken());
  }
  if (*clause == KeywordToken("pattern")) {
    return parsePattern(nextToken());
  }
  throw ParseSyntaxError("Unknown clause: " + clause->value);
}
PatternClause *QueryParser::parsePattern(Token *synonym) {
  if (synonym->type != TokenType::kSymbol) {
    throw ParseSyntaxError("Missing assign synonym");
  }
  QueryDeclaration *pattern_synonym = getDeclaration(synonym);
  parseBracket(nextToken(), true);
  QueryDeclaration *first_param = parseEntRefDeclaration(peekToken());
  parseComma(nextToken());
  QueryDeclaration *second_param = parseExpression();
  parseBracket(nextToken(), false);
  spdlog::debug("Pattern parsed: pattern []([], [])",
                pattern_synonym->toString() ,first_param->toString(), second_param->toString());
  return builder_.buildAssignPattern(pattern_synonym, first_param, second_param);
}
SuchThatClause *QueryParser::parseSuchThat(Token *relationship) {
  RsType rs_type;
  try {
    rs_type = QueryKeywords::relationshipKeywordToType(relationship->value);
    return such_that_mapping_[rs_type](peekToken());
  }
  catch (std::out_of_range& oor) {
    throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
  switch (rs_type) {
    case RsType::kFollows:
      return parseFollows(peekToken());
    case RsType::kParent:
      return parseParent(peekToken());
    case RsType::kUses:
      return parseUses(peekToken());
    case RsType::kModifies:
      return parseModifies(peekToken());
    default:
      throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
}

SuchThatClause *QueryParser::parseFollows(Token *star) {
  bool follows_all = false;
  if (*star == OperatorToken("*")) {
    nextToken();
    follows_all = true;
  }
  parseBracket(nextToken(), true);
  QueryDeclaration *first = parseStmtRefDeclaration(peekToken());
  parseComma(nextToken());
  QueryDeclaration *second = parseStmtRefDeclaration(peekToken());;
  parseBracket(nextToken(), false);

  SuchThatClause *clause;
  if (follows_all) {
    clause = builder_.buildSuchThat(RsType::kFollowsAll, first, second);
  } else {
    clause = builder_.buildSuchThat(RsType::kFollows, first, second);
  }
  spdlog::debug("Follows parsed: Follows([], []), *: []", first->toString(), second->toString(), follows_all);

  return clause;
}

SuchThatClause *QueryParser::parseParent(Token *star) {
  bool parent_all = false;
  if (*star == OperatorToken("*")) {
    nextToken();
    parent_all = true;
  }
  parseBracket(nextToken(), true);
  QueryDeclaration *first = parseStmtRefDeclaration(peekToken());
  parseComma(nextToken());
  QueryDeclaration *second = parseStmtRefDeclaration(peekToken());;
  parseBracket(nextToken(), false);

  SuchThatClause *clause;
  if (parent_all) {
    clause = builder_.buildSuchThat(RsType::kParentAll, first, second);
  } else {
    clause = builder_.buildSuchThat(RsType::kParent, first, second);
  }
  spdlog::debug("Parent parsed: Parent([], []), *: []", first->toString(), second->toString(), parent_all);
  return clause;
}
SuchThatClause *QueryParser::parseUses(Token *star) {
  if (*star == OperatorToken("*")) {
    throw ParseSyntaxError("* not allowed for Uses");
  }
  parseBracket(nextToken(), true);
  QueryDeclaration *first = parseAnyRefDeclaration(peekToken());
  parseComma(nextToken());
  QueryDeclaration *second = parseEntRefDeclaration(peekToken());
  parseBracket(nextToken(), false);
  spdlog::debug("Uses parsed: Uses([], [])", first->toString(), second->toString());
  return builder_.buildSuchThat(RsType::kUses, first, second);
}

SuchThatClause *QueryParser::parseModifies(Token *star) {
  if (*star == OperatorToken("*")) {
    throw ParseSyntaxError("* not allowed for Modifies");
  }
  parseBracket(nextToken(), true);
  QueryDeclaration *first = parseAnyRefDeclaration(peekToken());
  parseComma(nextToken());
  QueryDeclaration *second = parseEntRefDeclaration(peekToken());
  parseBracket(nextToken(), false);
  spdlog::debug("Modifies parsed: Modifies([], [])", first->toString(), second->toString());
  return builder_.buildSuchThat(RsType::kModifies, first, second);
}

void QueryParser::parseBracket(Token *bracket, bool open) {
  if (open && *bracket != RoundOpenBracketToken()) {
    throw ParseSyntaxError("Missing '(' before parameters");

  }
  if (!open && *bracket != RoundCloseBracketToken()) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
}

void QueryParser::parseComma(Token *comma) {
  if (*comma != CommaToken()) {
    throw ParseSyntaxError("Missing ',' between parameters");

  }
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
    std::string expression = parseFlattenedExpression(nextToken());
    if (wild_expression) {
      if (nextToken()->type != TokenType::kWildCard) {
        throw ParseSyntaxError("Invalid wildcard expression");
      }
      return builder_.buildWildcardExpression(expression);
    }
    return builder_.buildExpression(expression);
  }
  return builder_.buildWildcardStmt();
}

std::string QueryParser::parseFlattenedExpression(Token *quote) {
  parseQuote(quote);
  std::string expression;
  bool expect_operand = true;
  std::vector<Token *> expr_tokens;
  while (peekToken()->type != TokenType::kQuote) {
    Token *tmp  = nextToken();
    if (expect_operand) {
      if (tmp->type == TokenType::kSymbol || tmp->type == TokenType::kLiteral) {
        expr_tokens.push_back(tmp);
      } else {
        throw ParseSyntaxError("Unexpected symbol in expression: " + tmp->value);
      }
      expect_operand = false;
    } else {
      if (tmp->type == TokenType::kOperator) {
        expr_tokens.push_back(tmp);
      } else {
        throw ParseSyntaxError("Unexpected operator in expression: " + tmp->value);
      }
      expect_operand = true;
    }
  }
  if (expect_operand) {
    throw ParseSyntaxError("Missing symbol in expression");
  }
  parseQuote(nextToken());
  expr_tokens.push_back(new EndOfFileToken());
  return Parser::ParseExpression(expr_tokens)->ToString();
}

void QueryParser::parseQuote(Token *quote) {
  if (*quote != QuoteToken()) {
    throw ParseSyntaxError("Missing \"");
  }
}

QueryDeclaration *QueryParser::parseWildcard(EntityType type, Token *wildcard) {
  if (wildcard->type != TokenType::kWildCard) {
    throw ParseSyntaxError("Invalid wildcard expression");
  }
  switch (type) {
    case EntityType::kWildcardStmt:
      return builder_.buildWildcardStmt();
    case EntityType::kWildcardEnt:
      return builder_.buildWildcardEnt();
    default:
      throw ParseSyntaxError("Invalid wildcard type (internal error)");
  }
}


