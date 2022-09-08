#include "query_validator.h"

#include <utility>

QueryValidator::QueryValidator(std::vector<Token *> tokens) {
  this->tokens_ = std::move(tokens);
}

void QueryValidator::validate() {
  while (!outOfTokens()) {
    Token* tmp = peekToken();
    if (this->declaration_keywords_.count(tmp->value)) {
      validateDeclarations();
    }
    if (this->call_keywords_.count(tmp->value)) {
      ValidateQueryCalls();
    }
    if (*tmp == EndOfFileToken()) {
      break;
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
  Entity* entity = validateEntity();
  QuerySynonym synonym = validateSynonym();
  if (*nextToken() == SemicolonToken()) {
    return new QueryDeclaration(entity, synonym);
  }
  throw ParseSyntaxError("Missing `;` after declaration");
}
QueryDeclaration* QueryValidator::findDeclaration(const QuerySynonym &synonym) {
  for (QueryDeclaration* declaration : query_declarations_) {
    if (declaration->getSynonym() == synonym) {
      return declaration;
    }
  }
  throw ParseSemanticError("Missing declaration: " + synonym.getSynonym());
}
Entity* QueryValidator::validateEntity() {
  Token* token = nextToken();
  if (*token == KeywordToken("variable")) {
    return new VariableEntity(peekToken()->value);
  }
  if (*token == KeywordToken("constant")) {
    return new ConstantEntity(peekToken()->value);
  }
  if (*token == KeywordToken("procedure")) {
    return new ProcedureEntity(peekToken()->value);
  }
  if (*token == KeywordToken("read")) {
    return new ReadEntity(-1);
  }
  if (*token == KeywordToken("print")) {
    return new PrintEntity(-1);
  }
  if (*token == KeywordToken("call")) {
    return new CallEntity(-1);
  }
  if (*token == KeywordToken("while")) {
    return new WhileEntity(-1);
  }
  if (*token == KeywordToken("if")) {
    return new IfEntity(-1);
  }
  if (*token == KeywordToken("assign")) {
    return new AssignEntity(-1);
  }
  throw ParseSyntaxError("Unknown entity: " + token->value);
}
QuerySynonym QueryValidator::validateSynonym() {
  if (peekToken()->type == TokenType::kSymbol) {
    return QuerySynonym(nextToken()->value);
  }
  throw ParseSyntaxError("Invalid synonym name: " + peekToken()->value);
}
void QueryValidator::ValidateQueryCalls() {
  Token* token = nextToken();
  if (*token == KeywordToken("Select")) {
    QueryDeclaration* synonym_declaration = findDeclaration(validateSynonym());
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
    return SuchThatClause(validateRelationship());
  }
  if (*token == KeywordToken("pattern")) {
    return PatternClause(validatePattern());
  }
  throw ParseSyntaxError("Unknown clause: " + token->value);
}
Pattern QueryValidator::validatePattern() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  Entity* first = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  Entity* second = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return Pattern(first, second);
}
Relationship QueryValidator::validateRelationship() {
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
FollowsRelationship QueryValidator::validateFollows() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  Entity* first = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  Entity* second = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return FollowsRelationship(first, second);
}
ParentRelationship QueryValidator::validateParent() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  Entity* first = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  Entity* second = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return ParentRelationship(first, second);
}
UsesRelationship QueryValidator::validateUses() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  Entity* first = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  Entity* second = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return UsesRelationship(first, second);
}
ModifiesRelationship QueryValidator::validateModifies() {
  if (!(*nextToken() == RoundOpenBracketToken())) {
    throw ParseSyntaxError("Missing '(' before parameters");
  }
  Entity* first = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == CommaToken())) {
    throw ParseSyntaxError("Missing ',' between parameters");
  }
  Entity* second = findDeclaration(validateSynonym())->getEntity();
  if (!(*nextToken() == RoundCloseBracketToken())) {
    throw ParseSyntaxError("Missing ')' after parameters");
  }
  return ModifiesRelationship(first, second);
}



