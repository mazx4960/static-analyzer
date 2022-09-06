#include <gtest/gtest.h>

#include <algorithm>
#include "qps/pql/query_call/query_call.h"
#include "qps/query_parser/query_builder.h"
#include "qps/query_evaluator/query_evaluator.h"

TEST(BuilderTest, QueryBuildTest) {
  std::vector<Token *> input_tokens = {
      new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
      new KeywordToken("assign"), new SymbolToken("v2"), new SemicolonToken(),
      new KeywordToken("Select"), new SymbolToken("v1"),
      new SymbolToken("such"), new SymbolToken("that"),
      new KeywordToken("Parent"), new RoundOpenBracketToken(),
      new SymbolToken("v1"), new CommaToken(),
      new SymbolToken("v2"), new RoundCloseBracketToken(),
      new EndOfFileToken()};
  QueryBuilder query_builder = QueryBuilder(input_tokens);
  Query query = query_builder.build();


  ASSERT_EQ(query.getQueryCall().getType(), CallType::kSelect);
}