#include "query_parser.h"

#include <algorithm>
#include <utility>
#include <functional>

#include "commons/parser/parser.h"
#include "qps/pql/query_keywords.h"
#include "spdlog/spdlog.h"

QueryParser::QueryParser(std::vector<Token *> tokens) {
  this->tokens_ = tokens;
}

Query *QueryParser::parse() {
  Declarations query_declarations = parseDeclarations();
  QueryCall *query_call = parseQueryCall();
  Clauses query_clauses = parseClauses();
  if (!outOfTokens()) {
    throw ParseSyntaxError("Unexpected token");
  }
  return new Query(query_declarations, query_call, query_clauses);
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
  return this->token_index_ == this->tokens_.size() || *peekToken() == EndOfFileToken();
}

Declarations QueryParser::parseDeclarations() {
  while (QueryKeywords::isValidDeclarationKeyword(peekToken()->value)) {
    parseDeclarationStatement();
  }
  return this->declarations_;
}

void QueryParser::parseDeclarationStatement() {
  Token *prefix = nextToken();
  EntityType type;
  try {
    type = QueryKeywords::declarationKeywordToType(prefix->value);
  }
  catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown declaration type: " + prefix->value);
  }

  Declarations declarations;
  // Initial declaration
  declarations_.push_back(parseDeclaration(type));

  // While there are multiple declarations seperated by commas
  while (*peekToken() != SemicolonToken()) {
    expect(nextToken(), {TokenType::kComma});
    declarations_.push_back(parseDeclaration(type));
  }

  // End declaration statement with a semicolon
  expect(nextToken(), {TokenType::kSemicolon});
}

SynonymReference *QueryParser::parseDeclaration(EntityType type) {
  switch (type) {
    case EntityType::kProcedure: return new ProcedureDeclaration(parseSynonym());
    case EntityType::kStatement: return new StatementDeclaration(parseSynonym());
    case EntityType::kVariable: return new VariableDeclaration(parseSynonym());
    case EntityType::kConstant: return new ConstantDeclaration(parseSynonym());
    case EntityType::kAssignStmt: return new AssignDeclaration(parseSynonym());
    case EntityType::kCallStmt: return new CallDeclaration(parseSynonym());
    case EntityType::kIfStmt: return new IfDeclaration(parseSynonym());
    case EntityType::kWhileStmt: return new WhileDeclaration(parseSynonym());
    case EntityType::kPrintStmt: return new PrintDeclaration(parseSynonym());
    case EntityType::kReadStmt: return new ReadDeclaration(parseSynonym());
    default:throw ParseSyntaxError("Unknown declaration type:" + EntityTypeToString(type));
  }
}

QueryReference *QueryParser::parseReference() {
  Token *reference = peekToken();
  switch (reference->type) {
    case TokenType::kQuote: return parseQuotedReference();
    case TokenType::kLiteral: return parseIntegerReference();
    case TokenType::kSymbol: return parseSynonymReference();
    case TokenType::kWildCard: return parseWildcardReference();
    default:throw ParseSyntaxError("Unknown Reference: " + reference->value);
  }
}

SynonymReference *QueryParser::parseSynonymReference() {
  return new SynonymReference(parseSynonym());
}

IntegerReference *QueryParser::parseIntegerReference() {
  expect(peekToken(), {TokenType::kLiteral});
  std::string literal_string = nextToken()->value;

  return new IntegerReference(literal_string);
}

IdentReference *QueryParser::parseQuotedReference() {
  expect(nextToken(), {TokenType::kQuote});
  IdentReference *declaration = parseIdentReference();
  expect(nextToken(), {TokenType::kQuote});
  return declaration;
}

IdentReference *QueryParser::parseIdentReference() {
  expect(peekToken(), {TokenType::kSymbol});
  return new IdentReference(nextToken()->value);
}
WildcardReference *QueryParser::parseWildcardReference() {
  expect(nextToken(), {TokenType::kWildCard});
  return new WildcardReference();
}

QuerySynonym *QueryParser::parseSynonym() {
  expect(peekToken(), {TokenType::kSymbol});
  return new QuerySynonym(nextToken()->value);
}

QueryCall *QueryParser::parseQueryCall() {
  Token *call = nextToken();
  if (!QueryKeywords::isValidCallKeyword(call->value)) {
    throw ParseSyntaxError("Unknown query call: " + call->value);
  }
  SynonymReference *synonym_reference = parseSynonymReference();

  return new SelectCall(synonym_reference);
}
Clauses QueryParser::parseClauses() {
  Clauses clauses;
  while (*peekToken() != EndOfFileToken()) {
    clauses.push_back(parseClause());
  }
  this->clauses_ = clauses;
  return clauses_;
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
  expect(relationship, {TokenType::kSymbol});
  RsType rs_type;
  std::string rs_keyword = relationship->value;

  // Check for *
  if (*peekToken() == OperatorToken("*")) {
    rs_keyword.append(nextToken()->value);
  }
  // Convert string to rs type
  try {
    rs_type = QueryKeywords::relationshipKeywordToType(rs_keyword);
  }
  catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryReference *first = parseReference();
  expect(nextToken(), {TokenType::kComma});
  QueryReference *second = parseReference();;
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  SuchThatClause *clause = parseSuchThat(rs_type, first, second);
  if (!clause->isSyntacticallyCorrect()) {
    throw ParseSyntaxError("Incorrect parameter syntax");
  }
  spdlog::debug("SuchThat parsed: {}({}, {})", RsTypeToString(rs_type), first->toString(), second->toString());
  return clause;
}

SuchThatClause *QueryParser::parseSuchThat(RsType rs_type, QueryReference *first, QueryReference *second) {
  switch (rs_type) {
    case RsType::kFollows:return new FollowsClause(first, second);
    case RsType::kFollowsT:return new FollowsTClause(first, second);
    case RsType::kParent:return new ParentClause(first, second);
    case RsType::kParentT:return new ParentTClause(first, second);
    case RsType::kUses:return new UsesClause(first, second);
    case RsType::kModifies:return new ModifiesClause(first, second);
    case RsType::kCalls:return new CallsClause(first, second);
    case RsType::kCallsT:return new CallsTClause(first, second);
    case RsType::kNext:return new NextClause(first, second);
    case RsType::kNextT:return new NextTClause(first, second);
    case RsType::kAffects:return new AffectsClause(first, second);
    case RsType::kAffectsT:return new AffectsTClause(first, second);
    default:throw ParseSyntaxError("Unsupported such-that relationship: " + RsTypeToString(rs_type));
  }
}

PatternClause *QueryParser::parsePattern() {
  expect(peekToken(), {TokenType::kSymbol});
  SynonymReference *syn_assign = parseSynonymReference();
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryReference *ent_ref = parseReference();
  expect(nextToken(), {TokenType::kComma});
  ExpressionSpec *expression_spec = parseExpression();
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  return new PatternClause(syn_assign, ent_ref, expression_spec);
}

ExpressionSpec *QueryParser::parseExpression() {
  bool is_wild = false;
  Token *expr = peekToken();
  if (expr->type == TokenType::kWildCard) {
    nextToken();
    is_wild = true;
    expr = peekToken();
  }
  if (expr->type == TokenType::kQuote) {
    std::string expression = parseFlattenedExpression();
    if (is_wild) {
      expect(nextToken(), {TokenType::kWildCard});
      return new WildExpression(expression);
    }
    return new ExactExpression(expression);
  }
  return new WildExpression();
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

void QueryParser::expect(Token *token, const std::unordered_set<TokenType> &expected_types) {
  if (expected_types.count(token->type) == 0) {
    throw ParseSyntaxError("Invalid token type: " + token->value);
  }
}

