// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>

#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"
#include "query_semantic_rules.h"



class QueryBuilder {
 private:
  std::vector<SynonymReference *> declarations_;
  std::unordered_set<std::string> synonyms_;
  Query *query_;

 public:
  explicit QueryBuilder(Query *query_blueprint = nullptr);
  Query *build();
  std::vector<SynonymReference *> buildDeclarations(const std::vector<SynonymReference *>& maybe_declarations);
  SynonymReference *buildDeclaration(SynonymReference *declaration_blueprint);
  SynonymReference *getDeclaration(const QuerySynonym *synonym);

  std::vector<QueryClause *> buildClauses(const std::vector<QueryClause *> &clauses_blueprint);
  PatternClause *buildAssignPattern(PatternClause *clause_blueprint);
  SuchThatClause *buildSuchThat(SuchThatClause *maybe_clause);
  static void checkParamSemantics(std::vector<EntityTypeSet> semantic_rules, QueryReference *query_declaration, int rule_no);
  QueryCall *buildQueryCall(QueryCall *query_call_blueprint);
};
