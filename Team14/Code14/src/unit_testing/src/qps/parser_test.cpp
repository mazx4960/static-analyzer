#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_parser/query_parser.h"


TEST(QueryParserTest, SynonymParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("syn")
  };
  auto *expected = new QuerySynonym("syn");
  QueryParser parser = QueryParser(tokens);
  auto *synonym = parser.parseSynonym();
  ASSERT_EQ(*synonym, *expected);
}

TEST(QueryParserTest, SelectCallParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("Select"), new SymbolToken("v")
  };
  auto *expected = new SelectCall(new SynonymReference(new QuerySynonym("v")));
  QueryParser parser = QueryParser(tokens);
  auto *select = parser.parseQueryCall();
  ASSERT_EQ(*select->getReference(), *expected->getReference());
}

TEST(QueryParserTest, AssignDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("assign"), new SymbolToken("a"), new SemicolonToken()
  };
  auto *expected = new AssignDeclaration(new QuerySynonym("a"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, StmtDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("stmt"), new SymbolToken("s"), new SemicolonToken()
  };
  auto *expected = new StatementDeclaration(new QuerySynonym("s"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, VariableDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("variable"), new SymbolToken("v"), new SemicolonToken()
  };
  auto *expected = new VariableDeclaration(new QuerySynonym("v"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, ConstantDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("constant"), new SymbolToken("c"), new SemicolonToken()
  };
  auto *expected = new ConstantDeclaration(new QuerySynonym("c"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, ProcedureDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("procedure"), new SymbolToken("p"), new SemicolonToken()
  };
  auto *expected = new ProcedureDeclaration(new QuerySynonym("p"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, ReadDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("read"), new SymbolToken("r"), new SemicolonToken()
  };
  auto *expected = new ReadDeclaration(new QuerySynonym("r"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, PrintDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("print"), new SymbolToken("pr"), new SemicolonToken()
  };
  auto *expected = new PrintDeclaration(new QuerySynonym("pr"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, CallDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("call"), new SymbolToken("cl"), new SemicolonToken()
  };
  auto *expected = new CallDeclaration(new QuerySynonym("cl"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, WhileDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("while"), new SymbolToken("wh"), new SemicolonToken()
  };
  auto *expected = new WhileDeclaration(new QuerySynonym("wh"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, IfDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("if"), new SymbolToken("wh"), new SemicolonToken()
  };
  auto *expected = new IfDeclaration(new QuerySynonym("wh"));
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  ASSERT_EQ(*declarations.front(), *expected);
}

TEST(QueryParserTest, MultiAssignDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("assign"), new SymbolToken("a1"), new CommaToken(), new SymbolToken("a2"), new CommaToken(),
      new SymbolToken("a3"), new SemicolonToken()
  };
  Declarations expected = {new AssignDeclaration(new QuerySynonym("a1")), new AssignDeclaration(new QuerySynonym("a2")),
                           new AssignDeclaration(new QuerySynonym("a3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiStmtDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("stmt"), new SymbolToken("s1"), new CommaToken(), new SymbolToken("s2"), new CommaToken(),
      new SymbolToken("s3"), new SemicolonToken()
  };
  Declarations expected =
      {new StatementDeclaration(new QuerySynonym("s1")), new StatementDeclaration(new QuerySynonym("s2")),
       new StatementDeclaration(new QuerySynonym("s3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiVariableDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("variable"), new SymbolToken("v1"), new CommaToken(), new SymbolToken("v2"), new CommaToken(),
      new SymbolToken("v3"), new SemicolonToken()
  };
  Declarations expected =
      {new VariableDeclaration(new QuerySynonym("v1")), new VariableDeclaration(new QuerySynonym("v2")),
       new VariableDeclaration(new QuerySynonym("v3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiConstantDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("constant"), new SymbolToken("c1"), new CommaToken(), new SymbolToken("c2"), new CommaToken(),
      new SymbolToken("c3"), new SemicolonToken()
  };
  Declarations expected =
      {new ConstantDeclaration(new QuerySynonym("c1")), new ConstantDeclaration(new QuerySynonym("c2")),
       new ConstantDeclaration(new QuerySynonym("c3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiProcedureDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("procedure"), new SymbolToken("p1"), new CommaToken(), new SymbolToken("p2"), new CommaToken(),
      new SymbolToken("p3"), new SemicolonToken()
  };
  Declarations expected =
      {new ProcedureDeclaration(new QuerySynonym("p1")), new ProcedureDeclaration(new QuerySynonym("p2")),
       new ProcedureDeclaration(new QuerySynonym("p3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiReadDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("read"), new SymbolToken("rd1"), new CommaToken(), new SymbolToken("rd2"), new CommaToken(),
      new SymbolToken("rd3"), new SemicolonToken()
  };
  Declarations expected = {new ReadDeclaration(new QuerySynonym("rd1")), new ReadDeclaration(new QuerySynonym("rd2")),
                           new ReadDeclaration(new QuerySynonym("rd3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiPrintDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("print"), new SymbolToken("pr1"), new CommaToken(), new SymbolToken("pr2"), new CommaToken(),
      new SymbolToken("pr3"), new SemicolonToken()
  };
  Declarations expected = {new PrintDeclaration(new QuerySynonym("pr1")), new PrintDeclaration(new QuerySynonym("pr2")),
                           new PrintDeclaration(new QuerySynonym("pr3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiCallDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("call"), new SymbolToken("cl1"), new CommaToken(), new SymbolToken("cl2"), new CommaToken(),
      new SymbolToken("cl3"), new SemicolonToken()
  };
  Declarations expected = {new CallDeclaration(new QuerySynonym("cl1")), new CallDeclaration(new QuerySynonym("cl2")),
                           new CallDeclaration(new QuerySynonym("cl3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiWhileDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("while"), new SymbolToken("wh1"), new CommaToken(), new SymbolToken("wh2"), new CommaToken(),
      new SymbolToken("wh3"), new SemicolonToken()
  };
  Declarations expected = {new WhileDeclaration(new QuerySynonym("wh1")), new WhileDeclaration(new QuerySynonym("wh2")),
                           new WhileDeclaration(new QuerySynonym("wh3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiIfDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("if"), new SymbolToken("wh1"), new CommaToken(), new SymbolToken("wh2"), new CommaToken(),
      new SymbolToken("wh3"), new SemicolonToken()
  };
  Declarations expected = {new IfDeclaration(new QuerySynonym("wh1")), new IfDeclaration(new QuerySynonym("wh2")),
                           new IfDeclaration(new QuerySynonym("wh3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, MultiMixedDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("stmt"), new SymbolToken("s1"), new SemicolonToken(),
      new KeywordToken("variable"), new SymbolToken("v1"), new CommaToken(), new SymbolToken("v2"), new SemicolonToken(),
      new KeywordToken("call"), new SymbolToken("c1"), new CommaToken(), new SymbolToken("c2"), new SemicolonToken(),
      new KeywordToken("procedure"), new SymbolToken("p1"), new CommaToken(), new SymbolToken("p2"), new CommaToken(),
      new SymbolToken("p3"), new SemicolonToken()
  };
  Declarations expected = {new StatementDeclaration(new QuerySynonym("s1")),
                           new VariableDeclaration(new QuerySynonym("v1")),
                           new VariableDeclaration(new QuerySynonym("v2")),
                           new CallDeclaration(new QuerySynonym("c1")),
                           new CallDeclaration(new QuerySynonym("c2")),
                           new ProcedureDeclaration(new QuerySynonym("p1")),
                           new ProcedureDeclaration(new QuerySynonym("p2")),
                           new ProcedureDeclaration(new QuerySynonym("p3"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

// Declaration semantic error should not be caught by parser
TEST(QueryParserTest, SameSynonymMultiMixedDeclarationParseTest) {
  std::vector<Token *> tokens = {
      new KeywordToken("stmt"), new SymbolToken("s"), new SemicolonToken(),
      new KeywordToken("variable"), new SymbolToken("s"), new CommaToken(), new SymbolToken("s"), new SemicolonToken(),
      new KeywordToken("call"), new SymbolToken("s"), new CommaToken(), new SymbolToken("s"), new SemicolonToken(),
      new KeywordToken("procedure"), new SymbolToken("s"), new CommaToken(), new SymbolToken("s"), new CommaToken(),
      new SymbolToken("s"), new SemicolonToken()
  };
  Declarations expected = {new StatementDeclaration(new QuerySynonym("s")),
                           new VariableDeclaration(new QuerySynonym("s")),
                           new VariableDeclaration(new QuerySynonym("s")),
                           new CallDeclaration(new QuerySynonym("s")),
                           new CallDeclaration(new QuerySynonym("s")),
                           new ProcedureDeclaration(new QuerySynonym("s")),
                           new ProcedureDeclaration(new QuerySynonym("s")),
                           new ProcedureDeclaration(new QuerySynonym("s"))};
  QueryParser parser = QueryParser(tokens);
  auto declarations = parser.parseDeclarations();
  for (int i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(*declarations[i], *expected[i]);
  }
}

TEST(QueryParserTest, IntegerStmtReferenceParseTest) {
  std::vector<Token *> tokens = {
      new LiteralToken("1")
  };
  auto *expected = new IntegerReference("1");
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, IdentEntReferenceParseTest) {
  std::vector<Token *> tokens = {
      new QuoteToken(), new SymbolToken("ident"), new QuoteToken()
  };
  auto *expected = new IdentReference("ident");
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, SynonymReferenceParseTest) {
  std::vector<Token *> tokens = {
      new SymbolToken("s")
  };
  auto *expected = new SynonymReference(new QuerySynonym("s"));
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseReference();
  ASSERT_EQ(*reference, *expected);
}

TEST(QueryParserTest, WildcardReferenceParseTest) {
  std::vector<Token *> tokens = {
      new WildCardToken()
  };
  auto *expected = new WildcardReference();
  QueryParser parser = QueryParser(tokens);
  auto *reference = parser.parseReference();
  ASSERT_EQ(*reference, *expected);
}