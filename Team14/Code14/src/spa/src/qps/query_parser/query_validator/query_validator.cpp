#include "query_validator.h"

#include <utility>
#include <algorithm>

QueryValidator::QueryValidator(std::vector<Token *> tokens) {
  this->tokens_ = std::move(tokens);
}

void QueryValidator::validate() {
  while (!outOfTokens()) {
    Token* tmp = peekToken();
    if (this->declaration_keywords_.count(tmp->value)) {
      validateDeclarations();
    }else if (this->call_keywords_.count(tmp->value)) {
      ValidateQueryCalls();
    }else if (*tmp == EndOfFileToken()) {
      break;
    } else {
      throw ParseSyntaxError("Unexpected token: " + tmp->value);
    }
  }
}

std::vector<QueryDeclaration *> QueryValidator::getDeclarations() {
  return this->query_declarations_;
}
std::vector<QueryCall *> QueryValidator::getQueryCalls() {
  return this->query_calls_;
}

Token* QueryValidator::nextToken() {
  return tokens_[this->token_index_++];
}
Token* QueryValidator::peekToken() {
  return tokens_[this->token_index_];
}
bool QueryValidator::outOfTokens() {
  return this->tokens_.size() == this->token_index_;
}
void QueryValidator::validateDeclarations() {
  while (declaration_keywords_.count(peekToken()->value)) {
    query_declarations_.push_back(validateDeclaration());
  }
}
QueryDeclaration* QueryValidator::validateDeclaration() {
  Token* token = nextToken();
  QueryDeclaration* declaration = nullptr;
  if (*token == KeywordToken("stmt")) {
    declaration = new StatementDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("variable")) {
    declaration = new VariableDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("constant")) {
    declaration = new ConstantDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("procedure")) {
    declaration = new ProcedureDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("read")) {
    declaration = new ReadDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("print")) {
    declaration = new PrintDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("call")) {
    declaration = new CallDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("while")) {
    declaration = new WhileDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("if")) {
    declaration = new IfDeclaration(validateSynonym());
  }
  if (*token == KeywordToken("assign")) {
    declaration = new AssignDeclaration(validateSynonym());
  }
  if (declaration == nullptr) {
    throw ParseSyntaxError("Unknown declaration type: " + token->value);
  }
  if (!(*nextToken() == SemicolonToken())) {
    throw ParseSyntaxError("Missing `;` after declaration");
  }
  return declaration;
}

QueryDeclaration* QueryValidator::validateStmtRefDeclaration(bool allowWild) {
  if (peekToken()->type == TokenType::kLiteral) {
    return validateLiteralDeclaration();
  }
  if (peekToken()->type == TokenType::kSymbol) {
    return getDeclaration(nextToken()->value);
  }
  if (peekToken()->type == TokenType::kWildCard) {
    if (allowWild) {
      return new WildCardDeclaration();
    }
    throw ParseSemanticError("Wildcard '_' is not allowed here");
  }
  throw ParseSyntaxError("Unknown StmtRef: " + peekToken()->value);
}

QueryDeclaration* QueryValidator::validateEntRefDeclaration(bool allowWild) {
  if (peekToken()->type == TokenType::kQuote) {
    return validateQuotedDeclaration();
  }
  if (peekToken()->type == TokenType::kSymbol) {
    return getDeclaration(nextToken()->value);
  }
  if (peekToken()->type == TokenType::kWildCard) {
    if (allowWild) {
      return new WildCardDeclaration();
    }
    throw ParseSemanticError("Wildcard '_' is not allowed here");
  }
  throw ParseSyntaxError("Unknown EntRef: " + peekToken()->value);
}


IntegerDeclaration *QueryValidator::validateLiteralDeclaration() {
  return new IntegerDeclaration(std::stoi(nextToken()->value));
}

StringDeclaration *QueryValidator::validateStringDeclaration() {
  return new StringDeclaration(nextToken()->value);
}

StringDeclaration *QueryValidator::validateQuotedDeclaration() {
  if (nextToken()->type != TokenType::kQuote) {
    throw ParseSyntaxError("Missing '\"' before declaration");
  }
  StringDeclaration* declaration = validateStringDeclaration();
  if (nextToken()->type != TokenType::kQuote) {
    throw ParseSyntaxError("Missing '\"' after declaration");
  }
  return declaration;
}

QueryDeclaration* QueryValidator::getDeclaration(const std::string& synonym) {
  for (QueryDeclaration* declaration : query_declarations_) {
    if (declaration->getSynonym() == QuerySynonym(synonym)) {
      return declaration;
    }
  }
  throw ParseSemanticError("Missing declaration: " + synonym);
}

bool QueryValidator::isDeclared(const std::string &synonym) {
  return this->synonyms_.count(synonym) != 0U;
}

QuerySynonym QueryValidator::validateSynonym() {
  if (peekToken()->type != TokenType::kSymbol) {
    throw ParseSemanticError("Invalid synonym name: " + peekToken()->value);
  }
  if (isDeclared(peekToken()->value)) {
    throw ParseSemanticError("Synonym already declared: " + peekToken()->value);
  }
  this->synonyms_.insert(peekToken()->value);
  return QuerySynonym(nextToken()->value);
}

void QueryValidator::ValidateQueryCalls() {
  Token* token = nextToken();
  if (*token == KeywordToken("Select")) {
    QueryDeclaration* synonym_declaration = getDeclaration(nextToken()->value);
    std::vector<QueryClause> clause_vector;
    while (!(*peekToken() == EndOfFileToken())) {
      clause_vector.push_back(validateClause());
    }
    query_calls_.push_back(new SelectCall(synonym_declaration, clause_vector));
  } else {
    throw ParseSyntaxError("Unknown query call: " + token->value);
  }
}
QueryClause QueryValidator::validateClause() {
  Token* token = nextToken();
  if (*token == KeywordToken("such") && *nextToken() == KeywordToken("that")) {
    return validateSuchThat();
  }
  if (*token == KeywordToken("pattern")) {
    return validatePattern();
  }
  throw ParseSyntaxError("Unknown clause: " + token->value);
}
PatternClause QueryValidator::validatePattern() {
  if (peekToken()->type != TokenType::kSymbol) {
    throw ParseSyntaxError("Missing synonym");
  }
  QueryDeclaration* first = getDeclaration(nextToken()->value);
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  QueryDeclaration* second = validateEntRefDeclaration(true);
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  QueryDeclaration* third = validateExpression();
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return AssignPatternClause(first, second, third);
}
SuchThatClause QueryValidator::validateSuchThat() {
  Token* token = nextToken();
  if (*token == KeywordToken("Follows")) {
    return validateFollows();
  }
  if (*token == KeywordToken("Parent")) {
    return validateParent();
  }
  if (*token == KeywordToken("Uses")) {
    return validateUses();
  }
  if (*token == KeywordToken("Modifies")) {
    return validateModifies();
  }
  throw ParseSyntaxError("Unknown such-that relationship: " + token->value);
}
FollowsClause QueryValidator::validateFollows() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  QueryDeclaration* first = validateStmtRefDeclaration(true);
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  QueryDeclaration* second = validateStmtRefDeclaration(true);;
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return FollowsClause(first, second);
}
ParentClause QueryValidator::validateParent() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  QueryDeclaration* first = validateStmtRefDeclaration(true);
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  QueryDeclaration* second = validateStmtRefDeclaration(true);;
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return ParentClause(first, second);
}
UsesClause QueryValidator::validateUses() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  QueryDeclaration* first;
  if (*peekToken() == QuoteToken()) {
    first = validateStmtRefDeclaration(false);
  } else {
    first = validateEntRefDeclaration(false);
  }
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  QueryDeclaration* second  = validateEntRefDeclaration(false);
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return UsesClause(first, second);
}
ModifiesClause QueryValidator::validateModifies() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  QueryDeclaration* first;
  if (*peekToken() == QuoteToken()) {
    first = validateStmtRefDeclaration(false);
  } else {
    first = validateEntRefDeclaration(false);
  }
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  QueryDeclaration* second = validateEntRefDeclaration(false);;
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return ModifiesClause(first, second);
}
ExpressionDeclaration *QueryValidator::validateExpression() {
  std::string expression;
  if (peekToken()->type == TokenType::kWildCard) {
    expression.append(nextToken()->value);
  }
  if (peekToken()->type == TokenType::kQuote) {
    Token* tmp = nextToken();
    bool toggle = true;
    while (peekToken()->type != TokenType::kQuote) {
      tmp = nextToken();
      if (toggle) {
        if (tmp->type == TokenType::kSymbol) {
          expression.append(tmp->value);
        } else {
          throw ParseSyntaxError("Unexpected symbol in expression" + tmp->value);
        }
        toggle = false;
      } else {
        if (tmp->type == TokenType::kOperator) {
          expression.append(tmp->value);
        } else {
          throw ParseSyntaxError("Unexpected operator in expression" + tmp->value);
        }
        toggle = true;
      }
    }
  }
  if (peekToken()->type == TokenType::kWildCard) {
    expression.append(nextToken()->value);
  }
  return new ExpressionDeclaration(expression);
}
