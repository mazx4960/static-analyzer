#include "query_parser.h"

#include <utility>
#include <spdlog/spdlog.h>

#include "commons/parser/parser.h"

QueryParser::QueryParser(std::vector<Token *> tokens) {
  this->tokens_ = std::move(tokens);
  this->token_stream_ = tokens_.begin();
}

bool QueryParser::ShouldStop() {
  return **token_stream_ == EndOfFileToken();
}
Token *QueryParser::Expect(const Token &token) {
  if (**token_stream_ != token) {
    throw ParseSyntaxError("Expected " + token.ToString() + " but got " + (*token_stream_)->ToString());
  }
  return *token_stream_++;
}
Token *QueryParser::Expect(TokenType type) {
  if ((*token_stream_)->type != type) {
    throw ParseSyntaxError("Expected " + TokenTypeToString(type) + " but got " + (*token_stream_)->ToString());
  }
  return *token_stream_++;
}

Query *QueryParser::Parse() {
  this->query_builder_ = new QueryBuilder();
  ParseQuery();
  Expect(EndOfFileToken());
  auto *query = this->query_builder_->Build();
  return query;
}
void QueryParser::ParseQuery() {
  spdlog::debug("Parsing declarations");
  while (!ShouldStop() && QueryKeywords::isValidDeclarationKeyword((*token_stream_)->value)) {
    ParseDeclaration();
  }
  spdlog::debug("Parsing select");
  ParseSelect();
  spdlog::debug("Parsing clauses");
  auto clauses = std::vector<ClauseBlueprint *>();
  while (!ShouldStop() && QueryKeywords::isValidClauseKeyword((*token_stream_)->value)) {
    ParseClause(clauses);
  }
  query_builder_->AddClauses(clauses);
}

void QueryParser::ParseDeclaration() {
  EntityType type = QueryKeywords::declarationKeywordToType((*token_stream_++)->value);
  // Initial declaration
  query_builder_->AddDeclaration(new DeclarationBlueprint(ParseSynonym(), type));
  // While there are multiple declarations seperated by commas
  while ((*token_stream_)->type == TokenType::kComma) {
    Expect(TokenType::kComma);
    query_builder_->AddDeclaration(new DeclarationBlueprint(ParseSynonym(), type));
  }
  Expect(SemicolonToken());
}
void QueryParser::ParseSelect() {
  Expect(SymbolToken("Select"));

  if ((*token_stream_)->type == TokenType::kSymbol && (*token_stream_)->value == "BOOLEAN") {
    query_builder_->AddSelect(new SelectBlueprint(SelectType::kBoolean, {}));
    Expect(SymbolToken("BOOLEAN"));
    return;
  }

  auto elems = std::vector<ElemBlueprint *>();
  if (**token_stream_ == AngleOpenBracketToken()) {
    Expect(AngleOpenBracketToken());
    while (**token_stream_ != AngleCloseBracketToken()) {
      elems.push_back(ParseElem());
      if (**token_stream_ == CommaToken()) {
        Expect(CommaToken());
      }
    }
    Expect(AngleCloseBracketToken());
  } else {
    elems.push_back(ParseElem());
  }
  query_builder_->AddSelect(new SelectBlueprint(SelectType::kElem, elems));
}
void QueryParser::ParseClause(std::vector<ClauseBlueprint *> &clauses) {
  spdlog::debug("Checking clause for " + (*token_stream_)->ToString());
  if (**token_stream_ == KeywordToken("and")) {
    Expect(KeywordToken("and"));
    switch (clauses.back()->getClauseType()) {
      case ClauseType::kSuchThat: ParseSuchThat(clauses);
        break;
      case ClauseType::kPattern: ParsePattern(clauses);
        break;
      case ClauseType::kWith: ParseWith(clauses);
        break;
      default:throw ParseSyntaxError("and not supported");
    }
  } else if (**token_stream_ == KeywordToken("such") && **(token_stream_ + 1) == KeywordToken("that")) {
    Expect(KeywordToken("such"));
    Expect(KeywordToken("that"));
    ParseSuchThat(clauses);
  } else if (**token_stream_ == KeywordToken("pattern")) {
    Expect(KeywordToken("pattern"));
    ParsePattern(clauses);
  } else if (**token_stream_ == KeywordToken("with")) {
    Expect(KeywordToken("with"));
    ParseWith(clauses);
  } else {
    throw ParseSyntaxError("Unknown clause: " + (*token_stream_)->value);
  }
}

void QueryParser::ParseSuchThat(std::vector<ClauseBlueprint *> &clauses) {
  spdlog::debug("Parsing such that");
  Token *relationship = Expect(TokenType::kSymbol);

  RsType rs_type;
  std::string rs_keyword = relationship->value;
  if (**token_stream_ == OperatorToken("*")) {
    rs_keyword.append((*token_stream_++)->value);
  }
  // Convert string to rs type
  try {
    rs_type = QueryKeywords::relationshipKeywordToType(rs_keyword);
  } catch (std::out_of_range &oor) {
    throw ParseSyntaxError("Unknown such-that relationship: " + relationship->value);
  }
  Expect(RoundOpenBracketToken());
  auto *first = ParseBase();
  Expect(CommaToken());
  auto *second = ParseBase();
  Expect(RoundCloseBracketToken());
  auto *clause = new SuchThatBlueprint(rs_type, first, second);
  if (!clause->checkSyntax()) {
    throw ParseSyntaxError("Incorrect parameter syntax for: " + clause->toString());
  }
  spdlog::debug("Parsed clause: {}", clause->toString());
  clauses.push_back(clause);
}

void QueryParser::ParsePattern(std::vector<ClauseBlueprint *> &clauses) {
  spdlog::debug("Parsing pattern");
  auto *synonym = ParseSynonym();
  Expect(RoundOpenBracketToken());
  auto *ent_ref = ParseBase();
  Expect(CommaToken());
  auto *expr = ParseExpr();
  PatternBlueprint *clause;
  if ((*token_stream_)->type == TokenType::kComma) {
    Expect(CommaToken());
    auto *expr2 = ParseExpr();
    clause = new PatternBlueprint(synonym, ent_ref, expr, expr2);
  } else {
    clause = new PatternBlueprint(synonym, ent_ref, expr);
  }
  Expect(RoundCloseBracketToken());
  spdlog::debug("Parsed clause: {}", clause->toString());
  clauses.push_back(clause);
}

void QueryParser::ParseWith(std::vector<ClauseBlueprint *> &clauses) {
  spdlog::debug("Parsing with");
  auto *first = ParseBase();
  Expect(TokenType::kComparator);
  auto *second = ParseBase();
  auto *clause = new WithBlueprint(Comparator::kEquals, first, second);
  spdlog::debug("Parsed clause: {}", clause->toString());
  clauses.push_back(clause);
}

BaseBlueprint *QueryParser::ParseBase() {
  if ((*token_stream_)->type == TokenType::kQuote) {
    Expect(QuoteToken());
    auto *token = Expect(TokenType::kSymbol);
    Expect(QuoteToken());
    return new BaseBlueprint(ReferenceType::kIdent, token->value);
  }
  if ((*token_stream_)->type == TokenType::kLiteral) {
    auto *token = Expect(TokenType::kLiteral);
    return new BaseBlueprint(ReferenceType::kInteger, token->value);
  }
  if ((*token_stream_)->type == TokenType::kSymbol && (*(token_stream_ + 1))->type == TokenType::kDot) {
    return ParseElem();
  }
  if ((*token_stream_)->type == TokenType::kSymbol) {
    return ParseSynonym();
  }
  if ((*token_stream_)->type == TokenType::kWildCard) {
    Expect(WildCardToken());
    return new BaseBlueprint(ReferenceType::kWildcard, "");
  }
  throw ParseSyntaxError("Unknown base type: " + (*token_stream_)->ToString());
}

SynonymBlueprint *QueryParser::ParseSynonym() {
  auto *synonym_token = Expect(TokenType::kSymbol);
  return new SynonymBlueprint(synonym_token->value);
}

ElemBlueprint *QueryParser::ParseElem() {
  auto *synonym_bp = ParseSynonym();
  if ((*token_stream_)->type == TokenType::kDot) {
    Expect(TokenType::kDot);
    auto *attr_token = Expect(TokenType::kSymbol);
    std::string attr_name = attr_token->value;
    if (**token_stream_ == HashtagToken()) {
      auto *ht = Expect(HashtagToken());
      attr_name.append(ht->value);
    }
    AttributeType attribute_type;
    try {
      attribute_type = QueryKeywords::attributeKeywordToType(attr_name);
    } catch (std::out_of_range &oor) {
      throw ParseSyntaxError("Unknown attribute: " + attr_name);
    }
    return new ElemBlueprint(synonym_bp, attribute_type);
  }
  return new ElemBlueprint(synonym_bp, AttributeType::kNone);
}

ExprBlueprint *QueryParser::ParseExpr() {
  bool is_exact = true;
  std::string expr_string;
  if ((*token_stream_)->type == TokenType::kWildCard) {
    Expect(WildCardToken());
    is_exact = false;
  }
  if ((*token_stream_)->type == TokenType::kQuote) {
    Expect(QuoteToken());
    std::vector<Token *> expr_tokens;
    while ((*token_stream_)->type != TokenType::kQuote) {
      Token *tmp = *token_stream_++;
      expr_tokens.push_back(tmp);
    }
    Expect(QuoteToken());
    expr_string = Parser::ParseExpression(expr_tokens)->ToString();
    if (!is_exact) {
      Expect(WildCardToken());
    }
  }
  return new ExprBlueprint(expr_string, is_exact);
}
