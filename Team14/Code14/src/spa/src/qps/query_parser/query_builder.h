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
  std::vector<SynonymDeclaration *> declarations_;
  std::unordered_set<std::string> synonyms_;
  Query *maybe_query_;

 public:
  explicit QueryBuilder(Query *maybe_query = nullptr);
  Query *build();
  std::vector<SynonymDeclaration *> buildDeclarations(const std::vector<SynonymDeclaration *>& maybe_declarations);
  SynonymDeclaration *buildDeclaration(SynonymDeclaration *declaration);
  SynonymDeclaration *getDeclaration(const QuerySynonym *synonym);

  std::vector<QueryClause *> buildClauses(const std::vector<QueryClause *> &maybe_clauses);
  PatternClause *buildAssignPattern(PatternClause *maybe_clause);
  SuchThatClause *buildSuchThat(SuchThatClause *maybe_clause);
  static void checkParamSemantics(std::vector<EntityTypeSet> semantic_rules, QueryDeclaration *query_declaration, int rule_no);
};
