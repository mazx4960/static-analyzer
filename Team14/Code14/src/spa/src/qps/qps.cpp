// Copyright 2022 CS3203 Team14. All rights reserved.
#include "qps.h"

#include "query_builder.h"
#include "spdlog/spdlog.h"

void QPS::SetPKB(IPKBQuerier *pkb) {
  this->pkb_ = pkb;
}
Result *QPS::EvaluateQuery(std::istream *query_stream) {
  QueryLexer lexer(query_stream);
  auto tokens = lexer.lex();
  spdlog::info("Generated query tokens");
  std::string token_string;
  for (auto *token : tokens) {
    token_string += token->ToString() + " ";
  }
  spdlog::debug("Tokens[{}]: ", tokens.size(), token_string);

  QueryParser parser(tokens);
  spdlog::info("Parsing tokens...");
  try {
    parser.parse();
  } catch (ParseSemanticError &e) {
    return Result::semanticError();
  } catch (ParseSyntaxError &e) {
    return Result::syntacticError();
  }
  QueryBuilder builder = QueryBuilder();
  std::vector<QueryDeclaration *> query_declarations = parser.getDeclarations();
  std::vector<QueryCall *> query_calls = parser.getQueryCalls();
  builder.withDeclarations(query_declarations);
  builder.withQueryCalls(query_calls);
  std::string declaration_string;
  for (auto *declaration : parser.getDeclarations()) {
    declaration_string += EntityTypeToString(declaration->getType()) + ":" + declaration->toString() + " ";
  }
  spdlog::debug("Declarations[{}]: {}", query_declarations.size(), declaration_string);
  Query query = builder.build();
  spdlog::info("Tokens parsed");
  Result *result = (new QueryEvaluator(this->pkb_, query))->evaluate();
  return result;
}
