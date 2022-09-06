#include <gtest/gtest.h>

#include <algorithm>
#include "qps/pql/query_call/query_call.h"
#include "qps/query_parser/query_builder.h"

TEST(BuilderTest, QueryBuildTest) {
  std::vector<Token *> input_tokens_short = {
      new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
      new KeywordToken("assign"), new SymbolToken("v2"), new SemicolonToken(),
      new KeywordToken("Select"), new SymbolToken("v1"),
      new EndOfFileToken()};
  QueryBuilder query_builder_short = QueryBuilder(input_tokens_short);
  Query query_short = query_builder_short.build();



  ASSERT_EQ(query_short.getQueryCall().getType(), CallType::kSelect);

  std::vector<Token *> input_tokens_long = {
      new KeywordToken("assign"), new SymbolToken("v1"), new SemicolonToken(),
      new KeywordToken("assign"), new SymbolToken("v2"), new SemicolonToken(),
      new KeywordToken("Select"), new SymbolToken("v1"),
      new SymbolToken("such"), new SymbolToken("that"),
      new KeywordToken("Parent"), new RoundOpenBracketToken(),
      new SymbolToken("v1"), new CommaToken(),
      new SymbolToken("v2"), new RoundCloseBracketToken(),
      new EndOfFileToken()};
  QueryBuilder query_builder_long = QueryBuilder(input_tokens_long);
  Query query_long = query_builder_long.build();



  ASSERT_EQ(query_long.getQueryCall().getType(), CallType::kSelect);
}