#include "query_parser.h"

#include <algorithm>
#include <utility>
#include <functional>

#include "commons/parser/parser.h"
#include "qps/pql/query_keywords.h"
#include "spdlog/spdlog.h"

QueryParser::QueryParser(std::vector<Token *> tokens) {
  this->tokens_ = std::move(tokens);
}

Query *QueryParser::parse() {
  QueryCall *maybe_call;
  while (!outOfTokens()) {
    Token *tmp = peekToken();
    if (QueryKeywords::isValidDeclarationKeyword(tmp->value)) {
      parseDeclarations();
    } else if (QueryKeywords::isValidCallKeyword(tmp->value)) {
      maybe_call = parseQueryCall();
    } else if (*tmp == EndOfFileToken()) {
      break;
    } else {
      throw ParseSyntaxError("Unexpected token: " + tmp->value);
    }
  }
  maybe_query_ =  new Query(maybe_declarations_, maybe_call);
  return maybe_query_;
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

std::vector<SynonymDeclaration *> QueryParser::parseDeclarations() {
  while (QueryKeywords::isValidDeclarationKeyword(peekToken()->value)) {
    parseDeclaration();
  }
  return this->maybe_declarations_;
}

void QueryParser::parseDeclaration() {
  Token *prefix = nextToken();
  EntityType type;
  try {
    type = QueryKeywords::declarationKeywordToType(prefix->value);
  }
  catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown declaration type: " + prefix->value);
  }
  maybe_declarations_.push_back(new SynonymDeclaration(parseSynonym(), type));

  // While there are multiple declarations seperated by commas
  while (*peekToken() == CommaToken()) {
    nextToken();
    maybe_declarations_.push_back(new SynonymDeclaration(parseSynonym(), type));
  }

  // End declaration with a semicolon
  expect(nextToken(), {TokenType::kSemicolon});
}


QueryDeclaration *QueryParser::parseReference(SyntaxRuleType syntax_type) {
  checkReferenceSyntax(syntax_type);
  Token *reference = peekToken();
  QueryDeclaration *declaration;
  switch (reference->type) {
    case TokenType::kQuote:declaration = parseQuotedDeclaration();
      break;
    case TokenType::kLiteral:declaration = parseLiteralDeclaration();
      break;
    case TokenType::kSymbol:declaration = parseInlineSynonymDeclaration();
      break;
    case TokenType::kWildCard:declaration = parseWildcard();
      break;
    default:throw ParseSyntaxError("Unknown Reference: " + reference->value);
  }
  return declaration;
}

void QueryParser::checkReferenceSyntax(SyntaxRuleType syntax_type) {
  auto reference_rules = syntax_token_map.at(syntax_type);
  if (reference_rules.find(peekToken()->type) == reference_rules.end()) {
    throw ParseSyntaxError("Invalid reference syntax");
  }
}

SynonymDeclaration *QueryParser::parseInlineSynonymDeclaration() {
  return new SynonymDeclaration(parseSynonym());
}

IntegerDeclaration *QueryParser::parseLiteralDeclaration() {
  Token *literal = nextToken();
  expect(literal, {TokenType::kLiteral});
  std::string literal_string = literal->value;
  if (literal_string.length() > 1 && literal_string[0] == '0') {
    throw ParseSyntaxError("INTEGER cannot have leading zero: " + literal_string);
  }
  return new IntegerDeclaration(literal_string);
}

IdentDeclaration *QueryParser::parseQuotedDeclaration() {
  expect(nextToken(), {TokenType::kQuote});
  IdentDeclaration *declaration = parseIdentDeclaration();
  expect(nextToken(), {TokenType::kQuote});
  return declaration;
}

IdentDeclaration *QueryParser::parseIdentDeclaration() {
  Token *symbol = nextToken();
  expect(symbol, {TokenType::kSymbol});
  return new IdentDeclaration(symbol->value);
}

QuerySynonym *QueryParser::parseSynonym() {
  Token *synonym = nextToken();
  expect(synonym, {TokenType::kSymbol});
  return new QuerySynonym(synonym->value);
}

QueryCall *QueryParser::parseQueryCall() {
  Token *call = nextToken();
  if (!QueryKeywords::isValidCallKeyword(call->value)) {
    throw ParseSyntaxError("Unknown query call: " + call->value);
  }
  SynonymDeclaration *synonym_declaration = parseInlineSynonymDeclaration();
  while (*peekToken() != EndOfFileToken()) {
    maybe_clauses_.push_back(parseClause());
  }
  return new SelectCall(synonym_declaration, maybe_clauses_);
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

SuchThatClause *QueryParser::parseSuchThat() {
  Token *relationship = nextToken();
  std::vector<SyntaxRuleType> syntax_rules;
  RsType rs_type;
  try {
    rs_type = QueryKeywords::relationshipKeywordToType(relationship->value);
    if (*peekToken() == OperatorToken("*")) {
      nextToken();
      rs_type = allow_star_relationship_map.at(rs_type);
    }
    syntax_rules = such_that_syntax_rules.at(rs_type);
  }
  catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *first = parseReference(syntax_rules[0]);
  expect(nextToken(), {TokenType::kComma});
  QueryDeclaration *second = parseReference(syntax_rules[1]);;
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  spdlog::debug("SuchThat parsed: {}({}, {})", RsTypeToString(rs_type), first->toString(), second->toString());
  return new SuchThatClause(rs_type, first, second);
}

PatternClause *QueryParser::parsePattern() {
  expect(peekToken(), {TokenType::kSymbol});
  SynonymDeclaration *syn_assign = parseInlineSynonymDeclaration();
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryDeclaration *ent_ref = parseReference(pattern_syntax_rule);
  expect(nextToken(), {TokenType::kComma});
  StaticDeclaration *expression_spec = parseExpression();
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  return new AssignPatternClause(syn_assign, ent_ref, expression_spec);
}

StaticDeclaration *QueryParser::parseExpression() {
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
      return new WildCardExpressionDeclaration(expression);
    }
    return new ExpressionDeclaration(expression);

  }
  return new WildCardExpressionDeclaration("");

}

std::string QueryParser::parseFlattenedExpression() {
  expect(nextToken(), {TokenType::kQuote});
  std::vector<Token *> expr_tokens;
  while (peekToken()->type != TokenType::kQuote) {
    Token *tmp = nextToken();
    expr_tokens.push_back(tmp);
  }
  expect(nextToken(), {TokenType::kQuote});
  expr_tokens.push_back(new EndOfFileToken());
  return Parser::ParseExpression(expr_tokens)->ToString();
}

WildcardDeclaration *QueryParser::parseWildcard() {
  expect(nextToken(), {TokenType::kWildCard});
  return new WildcardDeclaration();
}

void QueryParser::expect(Token *token, const std::unordered_set<TokenType> &expected_types) {
  if (expected_types.count(token->type) == 0) {
    throw ParseSyntaxError("Invalid token type: " + token->value);
  }
}




