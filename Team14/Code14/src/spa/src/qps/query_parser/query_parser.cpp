#include "query_parser.h"

#include <utility>

#include "commons/parser/parser.h"

QueryParser::QueryParser(std::vector<Token *> tokens) {
  this->tokens_ = std::move(tokens);
}

Query *QueryParser::parse() {
  SynonymReferences query_declarations = parseDeclarations();
  QueryCall *query_call = parseQueryCall();
  Clauses query_clauses = parseClauses();
  if (!outOfTokens()) {
    throw ParseSyntaxError("Unexpected token");
  }
  QueryBuilder builder = QueryBuilder(new Query(query_declarations, query_call, query_clauses));
  auto *query = builder.build();
  return query;
}

Token *QueryParser::nextToken() {
  if (token_index_ >= tokens_.size() || *peekToken() == EndOfFileToken()) {
    throw ParseSyntaxError("Too few arguments");
  }
  return tokens_[this->token_index_++];
}

Token *QueryParser::peekToken() {
  if (token_index_ >= tokens_.size()) {
    return new EndOfFileToken();
  }
  return tokens_[this->token_index_];
}

bool QueryParser::outOfTokens() {
  return this->token_index_ == this->tokens_.size() || *peekToken() == EndOfFileToken();
}

SynonymReferences QueryParser::parseDeclarations() {
  declarations_.clear();
  while (!outOfTokens() && QueryKeywords::isValidDeclarationKeyword(peekToken()->value)) {
    parseDeclarationStatement();
  }
  return this->declarations_;
}

void QueryParser::parseDeclarationStatement() {
  Token *prefix = nextToken();
  EntityType type;
  try {
    type = QueryKeywords::declarationKeywordToType(prefix->value);
  } catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown declaration type: " + prefix->value);
  }

  SynonymReferences declarations;
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
    case EntityType::kProcedure:
      return new ProcedureDeclaration(parseSynonym());
    case EntityType::kStatement:
      return new StatementDeclaration(parseSynonym());
    case EntityType::kVariable:
      return new VariableDeclaration(parseSynonym());
    case EntityType::kConstant:
      return new ConstantDeclaration(parseSynonym());
    case EntityType::kAssignStmt:
      return new AssignDeclaration(parseSynonym());
    case EntityType::kCallStmt:
      return new CallDeclaration(parseSynonym());
    case EntityType::kIfStmt:
      return new IfDeclaration(parseSynonym());
    case EntityType::kWhileStmt:
      return new WhileDeclaration(parseSynonym());
    case EntityType::kPrintStmt:
      return new PrintDeclaration(parseSynonym());
    case EntityType::kReadStmt:
      return new ReadDeclaration(parseSynonym());
    default:
      throw ParseSyntaxError("Unknown declaration type:" + EntityTypeToString(type));
  }
}

QueryReference *QueryParser::parseClauseReference() {
  Token *reference = peekToken();
  switch (reference->type) {
    case TokenType::kQuote:
      return parseQuotedReference();
    case TokenType::kLiteral:
      return parseIntegerReference();
    case TokenType::kSymbol:
      return parseSynonymReference();
    case TokenType::kWildCard:
      return parseWildcardReference();
    default:
      throw ParseSyntaxError("Unknown Reference: " + reference->value);
  }
}

QueryReference *QueryParser::parseCompareReference() {
  Token *reference = peekToken();
  switch (reference->type) {
    case TokenType::kQuote:
      return parseQuotedReference();
    case TokenType::kLiteral:
      return parseIntegerReference();
    case TokenType::kSymbol:
      return parseAttrReference();
    default:
      throw ParseSyntaxError("Unknown Compare Reference: " + reference->value);
  }
}

ElemReference *QueryParser::parseElemReference() {
  auto *synonym_reference = parseSynonymReference();
  if (*peekToken() == DotToken()) {
    nextToken();
    return new AttrReference(synonym_reference, parseAttribute());
  }
  return synonym_reference;
}

AttrReference *QueryParser::parseAttrReference() {
  auto *synonym_reference = parseSynonymReference();
  if (*nextToken() != DotToken()) {
    throw ParseSyntaxError("Missing attribute ");
  }
  return new AttrReference(synonym_reference, parseAttribute());
}

AttributeType QueryParser::parseAttribute() {
  expect(peekToken(), {TokenType::kSymbol});
  std::string attr_name = nextToken()->value;
  if (*peekToken() == HashtagToken()) {
    attr_name.append(nextToken()->value);
  }
  try {
    return QueryKeywords::attributeKeywordToType(attr_name);
  } catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown attribute: " + attr_name);
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
  return new SelectCall(parseElemReferences());
}

std::vector<ElemReference *> QueryParser::parseElemReferences() {
  std::vector<ElemReference *> references;
  if (*peekToken() == AngleOpenBracketToken()) {
    expect(nextToken(), {TokenType::kAngleOpenBracket});
    references.push_back(parseElemReference());
    while (*peekToken() != AngleCloseBracketToken()) {
      expect(nextToken(), {TokenType::kComma});
      references.push_back(parseElemReference());
    }
    expect(nextToken(), {TokenType::kAngleCloseBracket});
  } else {
    references.push_back(parseElemReference());
  }
  return references;
}

Clauses QueryParser::parseClauses() {
  clauses_.clear();
  while (!outOfTokens() && QueryKeywords::isValidClauseKeyword(peekToken()->value)) {
    clauses_.push_back(parseClause());
  }
  return clauses_;
}

QueryClause *QueryParser::parseClause() {
  Token *clause = nextToken();
  if (*clause == KeywordToken("and")) {
    return parseAndClause();
  }
  if (*clause == KeywordToken("such") && *nextToken() == KeywordToken("that")) {
    return parseSuchThat();
  }
  if (*clause == KeywordToken("pattern")) {
    return parsePattern();
  }
  if (*clause == KeywordToken("with")) {
    return parseWith();
  }
  throw ParseSyntaxError("Unknown clause: " + clause->value);
}

QueryClause *QueryParser::parseAndClause() {
  switch (getPreviousClause()->getClauseType()) {
    case ClauseType::kSuchThat:
      return parseSuchThat();
    case ClauseType::kPattern:
      return parsePattern();
    case ClauseType::kWith:
      return parseWith();
    default:
      throw ParseSyntaxError("and not supported");
  }
}

QueryClause *QueryParser::getPreviousClause() {
  if (clauses_.empty()) {
    throw ParseSyntaxError("No previous clause");
  }
  return clauses_.back();
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
  } catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryReference *first = parseClauseReference();
  expect(nextToken(), {TokenType::kComma});
  QueryReference *second = parseClauseReference();
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  SuchThatClause *clause = parseSuchThat(rs_type, first, second);
  if (!clause->isSyntacticallyCorrect()) {
    throw ParseSyntaxError("Incorrect parameter syntax");
  }
  return clause;
}

SuchThatClause *QueryParser::parseSuchThat(RsType rs_type, QueryReference *first, QueryReference *second) {
  switch (rs_type) {
    case RsType::kFollows:
      return new FollowsClause(first, second);
    case RsType::kFollowsT:
      return new FollowsTClause(first, second);
    case RsType::kParent:
      return new ParentClause(first, second);
    case RsType::kParentT:
      return new ParentTClause(first, second);
    case RsType::kUses:
      return new UsesClause(first, second);
    case RsType::kModifies:
      return new ModifiesClause(first, second);
    case RsType::kCalls:
      return new CallsClause(first, second);
    case RsType::kCallsT:
      return new CallsTClause(first, second);
    case RsType::kNext:
      return new NextClause(first, second);
    case RsType::kNextT:
      return new NextTClause(first, second);
    case RsType::kAffects:
      return new AffectsClause(first, second);
    case RsType::kAffectsT:
      return new AffectsTClause(first, second);
    default:
      throw ParseSyntaxError("Unsupported such-that relationship: " + RsTypeToString(rs_type));
  }
}

PatternClause *QueryParser::parsePattern() {
  expect(peekToken(), {TokenType::kSymbol});
  SynonymReference *syn_assign = parseSynonymReference();
  expect(nextToken(), {TokenType::kRoundOpenBracket});
  QueryReference *ent_ref = parseClauseReference();
  expect(nextToken(), {TokenType::kComma});
  ExpressionSpec *expression_spec = parseExpression();
  expect(nextToken(), {TokenType::kRoundCloseBracket});
  return new PatternClause(syn_assign, ent_ref, expression_spec);
}

WithClause *QueryParser::parseWith() {
  QueryReference *first_ref = parseCompareReference();
  Comparator comparator = parseComparator();
  QueryReference *second_ref = parseCompareReference();
  auto *clause = new WithClause(comparator, first_ref, second_ref);
  if (!clause->isSyntacticallyCorrect()) {
    throw ParseSyntaxError("Incorrect parameter syntax");
  }
  return clause;
}

Comparator QueryParser::parseComparator() {
  expect(nextToken(), {TokenType::kComparator});
  return Comparator::kEquals;
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
  if (!is_wild) {
    throw ParseSyntaxError("Invalid expression type: " + expr->value);
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
