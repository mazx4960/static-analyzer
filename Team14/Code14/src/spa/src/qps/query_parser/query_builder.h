// Copyright 2022 CS3203 Team14. All rights reserved.

#include <vector>
#include <unordered_set>
#include "commons/lexer/token.h"
#include "qps/pql/query/query.h"

class QueryBuilder {
 private:
  std::vector<Token*> tokens_;
  std::vector<QueryDeclaration> query_declarations_;
  Token nextToken();
  Token peekToken();
  bool outOfTokens();
  int token_index_ = 0;
  std::vector<QueryClause> clause_vector_;
  const QueryCall empty_call_ = SelectCall(QueryDeclaration(VariableEntity(""), QuerySynonym("")), clause_vector_);
  std::unordered_set<std::string> relationship_keywords_ = {
      "Select", "such", "that", "Follows", "Parent", "Uses",
      "Modifies", "pattern"};
  std::unordered_set<std::string> declaration_keywords_ = {
      "pattern", "stmt", "read", "print", "call", "while",
      "if", "assign", "variable", "constant", "procedure"};
 public:
  explicit QueryBuilder(std::vector<Token*> tokens);
  Query build();
  QueryDeclaration buildDeclaration();
  Entity buildEntity();
  QuerySynonym buildSynonym();
  QueryCall buildQueryCall();
  QueryDeclaration findDeclaration(const QuerySynonym& synonym);
  QueryClause buildClause();
  Pattern buildPattern();
  Relationship buildRelationship();
  FollowsRelationship buildFollows();
  ParentRelationship buildParent();
  UsesRelationship buildUses();
  ModifiesRelationship buildModifies();
};

