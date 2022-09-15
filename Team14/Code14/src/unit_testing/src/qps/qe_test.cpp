#include <gtest/gtest.h>

#include <algorithm>
#include "qps/query_evaluator/query_evaluator.h"

class TestStorage {
 public:
  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      procedure_entities_ = {
      new ProcedureEntity("procedure1"),
      new ProcedureEntity("procedure2"),
      new ProcedureEntity("procedure3")
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      variable_entities_ = {
      new VariableEntity("x"),
      new VariableEntity("y"),
      new VariableEntity("z"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      constant_entities_ = {
      new ConstantEntity("1"),
      new ConstantEntity("2"),
      new ConstantEntity("3"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      stmt_assign_entities_ = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2"),
      new AssignStmtEntity("3"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      stmt_call_entities_ = {
      new CallStmtEntity("4"),
      new CallStmtEntity("5"),
      new CallStmtEntity("6"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      stmt_if_entities_ = {
      new IfStmtEntity("7"),
      new IfStmtEntity("8"),
      new IfStmtEntity("9"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      stmt_while_entities_ = {
      new WhileStmtEntity("10"),
      new WhileStmtEntity("11"),
      new WhileStmtEntity("12"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      stmt_print_entities_ = {
      new PrintStmtEntity("13"),
      new PrintStmtEntity("14"),
      new PrintStmtEntity("15"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      stmt_read_entities_ = {
      new ReadStmtEntity("16"),
      new ReadStmtEntity("17"),
      new ReadStmtEntity("18"),
  };

  inline static std::unordered_set<Entity *,
                                   EntityHashFunction,
                                   EntityPointerEquality>
      all_stmt_entities_ = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2"),
      new AssignStmtEntity("3"),
      new CallStmtEntity("4"),
      new CallStmtEntity("5"),
      new CallStmtEntity("6"),
      new IfStmtEntity("7"),
      new IfStmtEntity("8"),
      new IfStmtEntity("9"),
      new WhileStmtEntity("10"),
      new WhileStmtEntity("11"),
      new WhileStmtEntity("12"),
      new PrintStmtEntity("13"),
      new PrintStmtEntity("14"),
      new PrintStmtEntity("15"),
      new ReadStmtEntity("16"),
      new ReadStmtEntity("17"),
      new ReadStmtEntity("18"),
  };

};

class QueryEvaluatorMock : public QueryEvaluator {
 public:
  QueryEvaluatorMock(IPKBQuerier *pkb, Query &query) : QueryEvaluator(pkb, query) {};
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> mockCopyDeclarations() {
    return this->copyDeclarations();
  };
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> mockFetchContext() {
    return this->fetchContext();
  };
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> mockEvaluateSubQueries() {
    return this->evaluateSubQueries();
  };
};

class MockPKB : public IPKBQuerier {
  inline std::unordered_set<Entity *,
                            EntityHashFunction,
                            EntityPointerEquality> getEntities(EntityType entity_type)
  override {
    switch (entity_type) {
      case EntityType::kProcedure:return TestStorage::procedure_entities_;
      case EntityType::kVariable:return TestStorage::variable_entities_;
      case EntityType::kConstant:return TestStorage::constant_entities_;
      case EntityType::kStatement:return TestStorage::all_stmt_entities_;
      case EntityType::kAssignStmt:return TestStorage::stmt_assign_entities_;
      case EntityType::kCallStmt :return TestStorage::stmt_call_entities_;
      case EntityType::kIfStmt :return TestStorage::stmt_if_entities_;
      case EntityType::kWhileStmt:return TestStorage::stmt_while_entities_;
      case EntityType::kPrintStmt:return TestStorage::stmt_print_entities_;
      case EntityType::kReadStmt :return TestStorage::stmt_read_entities_;
      default: throw std::runtime_error("Invalid entity type!");
    };
  }
  inline std::unordered_set<Entity *,
                            EntityHashFunction,
                            EntityPointerEquality> getByRelationship(RsType rs_type,
                                                                     Entity *entity,
                                                                     bool inverse) override {
    return std::unordered_set<Entity *,
                              EntityHashFunction,
                              EntityPointerEquality>();
  };
  inline std::unordered_set<Entity *,
                            EntityHashFunction,
                            EntityPointerEquality> getByPattern(std::string &left_pattern,
                                                                std::string &right_pattern) override {
    return std::unordered_set<Entity *,
                              EntityHashFunction,
                              EntityPointerEquality>();
  };
};

TEST(QeCopyDeclarationTest, AllDeclarationsOnceEach) {
  IPKBQuerier *pkb = new MockPKB();

  auto *stmt_dec = new StatementDeclaration(new QuerySynonym("stmt_dec"));
  auto *read_dec = new ReadDeclaration(new QuerySynonym("read_dec"));
  auto *print_dec = new PrintDeclaration(new QuerySynonym("print_dec"));
  auto *call_dec = new CallDeclaration(new QuerySynonym("call_dec"));
  auto *while_dec = new WhileDeclaration(new QuerySynonym("while_dec"));
  auto *if_dec = new IfDeclaration(new QuerySynonym("if_dec"));
  auto *assign_dec = new AssignDeclaration(new QuerySynonym("assign_dec"));
  auto *var_dec = new VariableDeclaration(new QuerySynonym("var_dec"));
  auto *const_dec = new ConstantDeclaration(new QuerySynonym("const_dec"));
  auto *proc_dec = new ProcedureDeclaration(new QuerySynonym("proc_dec"));
  auto *expr_dec = new ExpressionDeclaration("x+y");
  auto *str_dec = new StringDeclaration("string_dec");
  auto *int_dec = new IntegerDeclaration("123");
  auto *wild_dec = new WildCardDeclaration();

  std::vector<QueryDeclaration *> declarations_vector = {
      stmt_dec, read_dec, print_dec, call_dec, while_dec,
      if_dec, assign_dec, var_dec, const_dec, proc_dec,
      expr_dec, str_dec, int_dec, wild_dec
  };
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> declarations_set = {
      stmt_dec, read_dec, print_dec, call_dec, while_dec,
      if_dec, assign_dec, var_dec, const_dec, proc_dec,
      expr_dec, str_dec, int_dec, wild_dec
  };

  SelectCall select_call = SelectCall(stmt_dec, {});
  Query query = Query(declarations_vector, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality>
      returned_declaration_set = query_evaluator->mockCopyDeclarations();

  ASSERT_EQ(declarations_set, returned_declaration_set);
}
TEST(QeCopyDeclarationTest, AllDeclarationsTwiceEach) {
  IPKBQuerier *pkb = new MockPKB();

  auto *stmt_dec_1 = new StatementDeclaration(new QuerySynonym("stmt_dec_1"));
  auto *read_dec_1 = new ReadDeclaration(new QuerySynonym("read_dec_1"));
  auto *print_dec_1 = new PrintDeclaration(new QuerySynonym("print_dec_1"));
  auto *call_dec_1 = new CallDeclaration(new QuerySynonym("call_dec_1"));
  auto *while_dec_1 = new WhileDeclaration(new QuerySynonym("while_dec_1"));
  auto *if_dec_1 = new IfDeclaration(new QuerySynonym("if_dec_1"));
  auto *assign_dec_1 = new AssignDeclaration(new QuerySynonym("assign_dec_1"));
  auto *var_dec_1 = new VariableDeclaration(new QuerySynonym("var_dec_1"));
  auto *const_dec_1 = new ConstantDeclaration(new QuerySynonym("const_dec_1"));
  auto *proc_dec_1 = new ProcedureDeclaration(new QuerySynonym("proc_dec_1"));
  auto *expr_dec_1 = new ExpressionDeclaration("x+y");
  auto *str_dec_1 = new StringDeclaration("string_dec_1");
  auto *int_dec_1 = new IntegerDeclaration("123");
  auto *wild_dec_1 = new WildCardDeclaration();

  auto *stmt_dec_2 = new StatementDeclaration(new QuerySynonym("stmt_dec_2"));
  auto *read_dec_2 = new ReadDeclaration(new QuerySynonym("read_dec_2"));
  auto *print_dec_2 = new PrintDeclaration(new QuerySynonym("print_dec_2"));
  auto *call_dec_2 = new CallDeclaration(new QuerySynonym("call_dec_2"));
  auto *while_dec_2 = new WhileDeclaration(new QuerySynonym("while_dec_2"));
  auto *if_dec_2 = new IfDeclaration(new QuerySynonym("if_dec_2"));
  auto *assign_dec_2 = new AssignDeclaration(new QuerySynonym("assign_dec_2"));
  auto *var_dec_2 = new VariableDeclaration(new QuerySynonym("var_dec_2"));
  auto *const_dec_2 = new ConstantDeclaration(new QuerySynonym("const_dec_2"));
  auto *proc_dec_2 = new ProcedureDeclaration(new QuerySynonym("proc_dec_2"));
  auto *expr_dec_2 = new ExpressionDeclaration("x+y");
  auto *str_dec_2 = new StringDeclaration("string_dec_2");
  auto *int_dec_2 = new IntegerDeclaration("123");
  auto *wild_dec_2 = new WildCardDeclaration();

  std::vector<QueryDeclaration *> declarations_vector = {
      stmt_dec_1, read_dec_1, print_dec_1, call_dec_1, while_dec_1,
      if_dec_1, assign_dec_1, var_dec_1, const_dec_1, proc_dec_1,
      expr_dec_1, str_dec_1, int_dec_1, wild_dec_1,

      stmt_dec_2, read_dec_2, print_dec_2, call_dec_2, while_dec_2,
      if_dec_2, assign_dec_2, var_dec_2, const_dec_2, proc_dec_2,
      expr_dec_2, str_dec_2, int_dec_2, wild_dec_2
  };
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> declarations_set = {
      stmt_dec_1, read_dec_1, print_dec_1, call_dec_1, while_dec_1,
      if_dec_1, assign_dec_1, var_dec_1, const_dec_1, proc_dec_1,
      expr_dec_1, str_dec_1, int_dec_1, wild_dec_1,

      stmt_dec_2, read_dec_2, print_dec_2, call_dec_2, while_dec_2,
      if_dec_2, assign_dec_2, var_dec_2, const_dec_2, proc_dec_2,
      expr_dec_2, str_dec_2, int_dec_2, wild_dec_2
  };

  SelectCall select_call = SelectCall(stmt_dec_1, {});
  Query query = Query(declarations_vector, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality>
      returned_declaration_set = query_evaluator->mockCopyDeclarations();

  ASSERT_EQ(declarations_set, returned_declaration_set);
}

TEST(QeCopyDeclarationTest, AllDeclarationsDuplicated) {
  IPKBQuerier *pkb = new MockPKB();

  auto *stmt_dec_1 = new StatementDeclaration(new QuerySynonym("stmt_dec"));
  auto *read_dec_1 = new ReadDeclaration(new QuerySynonym("read_dec"));
  auto *print_dec_1 = new PrintDeclaration(new QuerySynonym("print_dec"));
  auto *call_dec_1 = new CallDeclaration(new QuerySynonym("call_dec"));
  auto *while_dec_1 = new WhileDeclaration(new QuerySynonym("while_dec"));
  auto *if_dec_1 = new IfDeclaration(new QuerySynonym("if_dec"));
  auto *assign_dec_1 = new AssignDeclaration(new QuerySynonym("assign_dec"));
  auto *var_dec_1 = new VariableDeclaration(new QuerySynonym("var_dec"));
  auto *const_dec_1 = new ConstantDeclaration(new QuerySynonym("const_dec"));
  auto *proc_dec_1 = new ProcedureDeclaration(new QuerySynonym("proc_dec"));
  auto *expr_dec_1 = new ExpressionDeclaration("x+y");
  auto *str_dec_1 = new StringDeclaration("string_dec");
  auto *int_dec_1 = new IntegerDeclaration("123");
  auto *wild_dec_1 = new WildCardDeclaration();

  auto *stmt_dec_2 = new StatementDeclaration(new QuerySynonym("stmt_dec"));
  auto *read_dec_2 = new ReadDeclaration(new QuerySynonym("read_dec"));
  auto *print_dec_2 = new PrintDeclaration(new QuerySynonym("print_dec"));
  auto *call_dec_2 = new CallDeclaration(new QuerySynonym("call_dec"));
  auto *while_dec_2 = new WhileDeclaration(new QuerySynonym("while_dec"));
  auto *if_dec_2 = new IfDeclaration(new QuerySynonym("if_dec"));
  auto *assign_dec_2 = new AssignDeclaration(new QuerySynonym("assign_dec"));
  auto *var_dec_2 = new VariableDeclaration(new QuerySynonym("var_dec"));
  auto *const_dec_2 = new ConstantDeclaration(new QuerySynonym("const_dec"));
  auto *proc_dec_2 = new ProcedureDeclaration(new QuerySynonym("proc_dec"));
  auto *expr_dec_2 = new ExpressionDeclaration("x+y");
  auto *str_dec_2 = new StringDeclaration("string_dec");
  auto *int_dec_2 = new IntegerDeclaration("123");
  auto *wild_dec_2 = new WildCardDeclaration();

  std::vector<QueryDeclaration *> declarations_vector = {
      stmt_dec_1, read_dec_1, print_dec_1, call_dec_1, while_dec_1,
      if_dec_1, assign_dec_1, var_dec_1, const_dec_1, proc_dec_1,
      expr_dec_1, str_dec_1, int_dec_1, wild_dec_1,

      stmt_dec_2, read_dec_2, print_dec_2, call_dec_2, while_dec_2,
      if_dec_2, assign_dec_2, var_dec_2, const_dec_2, proc_dec_2,
      expr_dec_2, str_dec_2, int_dec_2, wild_dec_2
  };
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> declarations_set = {
      stmt_dec_1, read_dec_1, print_dec_1, call_dec_1, while_dec_1,
      if_dec_1, assign_dec_1, var_dec_1, const_dec_1, proc_dec_1,
      expr_dec_1, str_dec_1, int_dec_1, wild_dec_1,

      stmt_dec_2, read_dec_2, print_dec_2, call_dec_2, while_dec_2,
      if_dec_2, assign_dec_2, var_dec_2, const_dec_2, proc_dec_2,
      expr_dec_2, str_dec_2, int_dec_2, wild_dec_2
  };

  SelectCall select_call = SelectCall(stmt_dec_1, {});
  Query query = Query(declarations_vector, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality>
      returned_declaration_set = query_evaluator->mockCopyDeclarations();

  ASSERT_EQ(declarations_set.size(), 14);
  ASSERT_EQ(returned_declaration_set.size(), 14);
  ASSERT_EQ(declarations_set, returned_declaration_set);
}

TEST(QeFetchContextTest, StatementDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new StatementDeclaration(new QuerySynonym("stmt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::all_stmt_entities_);
}

TEST(QeFetchContextTest, ProcedureDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new ProcedureDeclaration(new QuerySynonym("proc"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::procedure_entities_);
}

TEST(QeFetchContextTest, VariableDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new VariableDeclaration(new QuerySynonym("stmt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::variable_entities_);
}

TEST(QeFetchContextTest, ConstantDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new ConstantDeclaration(new QuerySynonym("stmt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::constant_entities_);
}

TEST(QeFetchContextTest, StmtAssignDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new AssignDeclaration(new QuerySynonym("as"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_assign_entities_);
}

TEST(QeFetchContextTest, StmtCallDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new CallDeclaration(new QuerySynonym("cl"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_call_entities_);
}

TEST(QeFetchContextTest, StmtIfDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new IfDeclaration(new QuerySynonym("ifs"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_if_entities_);
}

TEST(QeFetchContextTest, StmtWhileDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new WhileDeclaration(new QuerySynonym("w"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_while_entities_);
}

TEST(QeFetchContextTest, StmtPrintDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new PrintDeclaration(new QuerySynonym("prt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_print_entities_);
}

TEST(QeFetchContextTest, StmtReadDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new ReadDeclaration(new QuerySynonym("rd"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_read_entities_);
}
