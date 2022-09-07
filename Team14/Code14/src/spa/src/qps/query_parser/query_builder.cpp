// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"
#include "commons/parser/parser_exceptions.h"

#include <utility>
QueryBuilder::QueryBuilder(std::vector<Token *> tokens) {
  this->tokens_ = std::move(tokens);
}

Query QueryBuilder::build() {
  QueryCall query_call = empty_call_;

  while (!outOfTokens()) {
    if (declaration_keywords_.count(peekToken().value)) {
      QueryDeclaration query_declaration = buildDeclaration();
      query_declarations_.push_back(query_declaration);
    } else if (peekToken() == KeywordToken("Select")){
      query_call = buildQueryCall();
    } else if (peekToken() == EndOfFileToken()){
      return Query(query_declarations_, query_call);
    }
  }
  throw ParseSyntaxError("Unknown keyword: " + peekToken().value);
}

Token QueryBuilder::nextToken() {
  return *tokens_[this->token_index_++];
}

Token QueryBuilder::peekToken() {
  return *tokens_[this->token_index_];
}

bool QueryBuilder::outOfTokens() {
  return this->tokens_.size() == this->token_index_;
}


QueryDeclaration QueryBuilder::buildDeclaration() {
  Entity entity = buildEntity();
  QuerySynonym synonym = buildSynonym();
  QueryDeclaration query_declaration = QueryDeclaration(entity, synonym);
  if (nextToken() == SemicolonToken()) {
    return query_declaration;
  }
  throw ParseSyntaxError("Missing ; after declaration");
}

Entity QueryBuilder::buildEntity() {
  Token token = nextToken();
  if (token == KeywordToken("variable")){
    return VariableEntity(peekToken().value);
  }
  if (token == KeywordToken("constant")){
    return ConstantEntity(peekToken().value);
  }
  if (token == KeywordToken("procedure")){
    return ProcedureEntity(peekToken().value);
  }
  if (token == KeywordToken("read")){
    return ReadEntity(-1);
  }
  if (token == KeywordToken("print")){
    return PrintEntity(-1);
  }
  if (token == KeywordToken("call")){
    return CallEntity(-1);
  }
  if (token == KeywordToken("while")){
    return WhileEntity(-1);
  }
  if (token == KeywordToken("if")){
    return IfEntity(-1);
  }
  if (token == KeywordToken("assign")) {
    return AssignEntity(-1);
  }
  throw ParseSyntaxError("Invalid entity: "+ token.value);
}


QuerySynonym QueryBuilder::buildSynonym() {
  return QuerySynonym(nextToken().value);
}

QueryDeclaration QueryBuilder::findDeclaration(const QuerySynonym& synonym) {
  for (QueryDeclaration declaration: query_declarations_) {
    if (declaration.getSynonym() == synonym) {
      return declaration;
    }
  }
  throw ParseSyntaxError("No such declaration: " + synonym.getSynonym());
}

QueryClause QueryBuilder::buildClause() {
  Token token = nextToken();
  if (token == KeywordToken("such") && nextToken() == KeywordToken("that")) {
    return SuchThatClause(buildRelationship());
  }
  if (token == KeywordToken("pattern")) {
    return PatternClause(buildPattern());
  }
  throw ParseSyntaxError("Invalid clause: " + token.value);
}

Pattern QueryBuilder::buildPattern() {
  if (nextToken() == RoundOpenBracketToken()) {
    Entity first = findDeclaration(buildSynonym()).getEntity();
    if (nextToken() == CommaToken()) {
      Entity second = findDeclaration(buildSynonym()).getEntity();
      if (nextToken() == RoundCloseBracketToken()) {
        return Pattern(first, second);
      }
    }
  }
  throw ParseSyntaxError("Invalid pattern");
}


FollowsRelationship QueryBuilder::buildFollows() {
  if (nextToken() == RoundOpenBracketToken()) {
    Entity first = findDeclaration(buildSynonym()).getEntity();
    if (nextToken() == CommaToken()) {
      Entity second = findDeclaration(buildSynonym()).getEntity();
      if (nextToken() == RoundCloseBracketToken()) {
        return FollowsRelationship(&first, &second);
      }
    }
  }
  throw ParseSyntaxError("Invalid Follows");
}

ParentRelationship QueryBuilder::buildParent() {
  if (nextToken() == RoundOpenBracketToken()) {
    Entity first = findDeclaration(buildSynonym()).getEntity();
    if (nextToken() == CommaToken()) {
      Entity second = findDeclaration(buildSynonym()).getEntity();
      if (nextToken() == RoundCloseBracketToken()) {
        return ParentRelationship(&first, &second);
      }
    }
  }
  throw ParseSyntaxError("Invalid Parent");
}

UsesRelationship QueryBuilder::buildUses() {
  if (nextToken() == RoundOpenBracketToken()) {
    Entity first = findDeclaration(buildSynonym()).getEntity();
    if (nextToken() == CommaToken()) {
      Entity second = findDeclaration(buildSynonym()).getEntity();
      if (nextToken() == RoundCloseBracketToken()) {
        return UsesRelationship(&first, &second);
      }
    }
  }
  throw ParseSyntaxError("Invalid Uses");
}

ModifiesRelationship QueryBuilder::buildModifies() {
  if (nextToken() == RoundOpenBracketToken()) {
    Entity first = findDeclaration(buildSynonym()).getEntity();
    if (nextToken() == CommaToken()) {
      Entity second = findDeclaration(buildSynonym()).getEntity();
      if (nextToken() == RoundCloseBracketToken()) {
        return ModifiesRelationship(&first, &second);
      }
    }
  }
  throw ParseSyntaxError("Invalid Modifies");
}


Relationship QueryBuilder::buildRelationship() {
  Token token = nextToken();
  if (token == KeywordToken("Follows")) {
    return buildFollows();
  }
  if (token == KeywordToken("Parent")) {
    return buildParent();
  }
  if (token == KeywordToken("Uses")) {
    return buildUses();
  }
  if (token == KeywordToken("Modifies")) {
    return buildModifies();
  }
  throw ParseSyntaxError("Unknown such that relationship: " + token.value);
}


QueryCall QueryBuilder::buildQueryCall() {
  if (nextToken() == KeywordToken("Select")) {
    QueryDeclaration synonym_declaration = findDeclaration(buildSynonym());
    std::vector<QueryClause> clause_vector;

    if (!(peekToken() == EndOfFileToken())) {
      clause_vector.push_back(buildClause());
    }
    return SelectCall(synonym_declaration, clause_vector);
  }
  throw ParseSyntaxError("Missing ; after declaration");
}