// Copyright 2022 CS3203 Team14. All rights reserved.

#include <utility>
#include <vector>
#include <unordered_set>
#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query/query.h"

class QueryValidator {
 private:
  std::vector<Token*> tokens_;

  std::vector<QueryDeclaration*> query_declarations_;
  std::vector<QueryCall*> query_calls_;



  int token_index_ = 0;

  std::unordered_set<std::string> call_keywords_ = {
      "Select"};

  std::unordered_set<std::string> declaration_keywords_ = {
      "pattern", "stmt", "read", "print", "call", "while",
      "if", "assign", "variable", "constant", "procedure"};
  Token* nextToken();
  Token* peekToken();
  bool outOfTokens();

 public:
  explicit QueryValidator(std::vector<Token*> tokens);
  void validate();
  std::vector<QueryDeclaration*> getDeclarations();
  std::vector<QueryCall*> getQueryCalls();
  void validateDeclarations();
  void ValidateQueryCalls();
  QueryDeclaration* validateDeclaration();
  QueryDeclaration* getDeclaration(const QuerySynonym &synonym);
  Entity * validateEntity();
  QuerySynonym validateSynonym();
  QueryClause validateClause();
  Pattern validatePattern();
  Relationship validateRelationship();
  FollowsRelationship validateFollows();
  ParentRelationship validateParent();
  UsesRelationship validateUses();
  ModifiesRelationship validateModifies();
  bool isDeclared(const std::string &synonym);
};

