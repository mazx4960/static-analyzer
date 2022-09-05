#include <gtest/gtest.h>

#include "commons/lexer/token.h"
#include "sp/ast/simpleast.h"
#include "sp/ast/simplenodetype.h"
#include "sp/grammar/simplegrammar.h"

TEST(SimpleParserTest, TestBasic) {
  std::vector<Token*> input_tokens = {
      new KeywordToken("procedure"), new SymbolToken("main"),
      new CurlyOpenBracketToken(), new SymbolToken("x"),
      new OperatorToken("="), new LiteralToken("1"),
      new SemicolonToken(), new CurlyCloseBracket(),
      new EndOfFileToken()};
  auto iter = input_tokens.begin();
  SimpleAstNode* program_node = (new ProgramGrammarRule())->parseNode(iter);
  ASSERT_EQ(program_node->getNodeType(),SimpleNodeType::kProgram);
}
