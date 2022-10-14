// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>

#include "qps/exceptions.h"
#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"

class QueryBuilder {
 private:
  std::vector<SynonymReference *> declarations_;
  std::unordered_set<std::string> synonyms_;
  Query *query_;

 public:
  explicit QueryBuilder(Query *query_blueprint = nullptr);
  Query *build();
  std::vector<SynonymReference *> buildDeclarations(const std::vector<SynonymReference *>& declaration_blueprint);
  SynonymReference *buildDeclaration(SynonymReference *declaration_blueprint);
  SynonymReference *getDeclaration(const QuerySynonym *synonym);

  std::vector<QueryClause *> buildClauses(const std::vector<QueryClause *> &clauses_blueprint);
  PatternClause *buildPattern(PatternClause *clause_blueprint);
  SuchThatClause *buildSuchThat(SuchThatClause *clause_blueprint);
  QueryCall *buildQueryCall(QueryCall *query_call_blueprint);
  std::vector<ElemReference *> buildQueryCallElemReferences(const std::vector<ElemReference *>& query_call_reference_blueprint);
  SynonymReference *getDeclaration(SynonymReference *synonym_reference);
};
