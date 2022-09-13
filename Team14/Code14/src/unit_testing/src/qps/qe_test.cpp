#include <gtest/gtest.h>

#include <algorithm>
#include "qps/query_evaluator/query_evaluator.h"

class TestStorage {
 public:
  inline static std::unordered_set<Entity *> procedure_entities_ = {
      new ProcedureEntity("procedure1"),
      new ProcedureEntity("procedure2"),
      new ProcedureEntity("procedure3")
  };

  inline static std::unordered_set<Entity *> variable_entities_ = {
      new VariableEntity("x"),
      new VariableEntity("y"),
      new VariableEntity("z"),
  };

  inline static std::unordered_set<Entity *> constant_entities_ = {
      new ConstantEntity("1"),
      new ConstantEntity("2"),
      new ConstantEntity("3"),
  };

  inline static std::unordered_set<Entity *> stmt_assign_entities_ = {
      new AssignEntity(1),
      new AssignEntity(2),
      new AssignEntity(3),
  };

  inline static std::unordered_set<Entity *> stmt_call_entities_ = {
      new CallEntity(4),
      new CallEntity(5),
      new CallEntity(6),
  };

  inline static std::unordered_set<Entity *> stmt_if_entities_ = {
      new IfEntity(7),
      new IfEntity(8),
      new IfEntity(9),
  };

  inline static std::unordered_set<Entity *> stmt_while_entities_ = {
      new WhileEntity(10),
      new WhileEntity(11),
      new WhileEntity(12),
  };

  inline static std::unordered_set<Entity *> stmt_print_entities_ = {
      new PrintEntity(13),
      new PrintEntity(14),
      new PrintEntity(15),
  };

  inline static std::unordered_set<Entity *> stmt_read_entities_ = {
      new ReadEntity(16),
      new ReadEntity(17),
      new ReadEntity(18),
  };

  inline static std::unordered_set<Entity *> all_stmt_entities_ = {
      new AssignEntity(1),
      new AssignEntity(2),
      new AssignEntity(3),
      new CallEntity(4),
      new CallEntity(5),
      new CallEntity(6),
      new IfEntity(7),
      new IfEntity(8),
      new IfEntity(9),
      new WhileEntity(10),
      new WhileEntity(11),
      new WhileEntity(12),
      new PrintEntity(13),
      new PrintEntity(14),
      new PrintEntity(15),
      new ReadEntity(16),
      new ReadEntity(17),
      new ReadEntity(18),
  };

};

class QueryEvaluatorMock : public QueryEvaluator {
 public:
  QueryEvaluatorMock(IPKBQuerier *pkb, Query &query) : QueryEvaluator(pkb, query) {};
  std::unordered_set<QueryDeclaration *> mockCopyDeclarations() { return this->copyDeclarations(); };
  std::unordered_set<QueryDeclaration *> mockFetchContext() { return this->fetchContext(); };
  std::unordered_set<QueryDeclaration *> mockEvaluateSubQueries() { return this->evaluateSubQueries(); };
};

class MockPKB : public IPKBQuerier {
  inline std::unordered_set<Entity *> getEntities(EntityType entity_type) override {
    switch (entity_type) {
      case EntityType::kProcedure: return TestStorage::procedure_entities_;
      case EntityType::kVariable: return TestStorage::variable_entities_;
      case EntityType::kConstant: return TestStorage::constant_entities_;
      case EntityType::kStatement: return TestStorage::all_stmt_entities_;
      default: throw std::runtime_error("Invalid entity type!");
    };
  }
  inline std::unordered_set<Entity *> getEntities(StmtType stmt_type) override {
    switch (stmt_type) {
      case StmtType::kAssign: return TestStorage::stmt_assign_entities_;
      case StmtType::kCall: return TestStorage::stmt_call_entities_;
      case StmtType::kIf: return TestStorage::stmt_if_entities_;
      case StmtType::kWhile: return TestStorage::stmt_while_entities_;
      case StmtType::kPrint: return TestStorage::stmt_print_entities_;
      case StmtType::kRead: return TestStorage::stmt_read_entities_;
      default: throw std::runtime_error("Invalid statement type!");
    };
  };
  inline std::unordered_set<Entity *> getByRelationship(RsType, Entity *, bool) override {
    return std::unordered_set<Entity *>();
  };
  inline std::unordered_set<Entity *> getByPattern(std::string &, std::string &) override {
    return std::unordered_set<Entity *>();
  };
};

TEST(QeCopyDeclarationTest, AllDeclarationsOnceEach) {
  IPKBQuerier *pkb = new MockPKB();

  auto *stmt_dec = new StatementDeclaration(QuerySynonym("stmt_dec"));
  auto *read_dec = new ReadDeclaration(QuerySynonym("read_dec"));
  auto *print_dec = new PrintDeclaration(QuerySynonym("print_dec"));
  auto *call_dec = new CallDeclaration(QuerySynonym("call_dec"));
  auto *while_dec = new WhileDeclaration(QuerySynonym("while_dec"));
  auto *if_dec = new IfDeclaration(QuerySynonym("if_dec"));
  auto *assign_dec = new AssignDeclaration(QuerySynonym("assign_dec"));
  auto *var_dec = new VariableDeclaration(QuerySynonym("var_dec"));
  auto *const_dec = new ConstantDeclaration(QuerySynonym("const_dec"));
  auto *proc_dec = new ProcedureDeclaration(QuerySynonym("proc_dec"));
  auto *expr_dec = new ExpressionDeclaration("x+y");
  auto *str_dec = new StringDeclaration("string_dec");
  auto *int_dec = new IntegerDeclaration(123);
  auto *wild_dec = new WildCardDeclaration();

  std::vector<QueryDeclaration *> declarations_vector = {
      stmt_dec, read_dec, print_dec, call_dec, while_dec,
      if_dec, assign_dec, var_dec, const_dec, proc_dec,
      expr_dec, str_dec, int_dec, wild_dec
  };
  std::unordered_set<QueryDeclaration *> declarations_set = {
      stmt_dec, read_dec, print_dec, call_dec, while_dec,
      if_dec, assign_dec, var_dec, const_dec, proc_dec,
      expr_dec, str_dec, int_dec, wild_dec
  };

  SelectCall select_call = SelectCall(stmt_dec, {});
  Query query = Query(declarations_vector, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  std::unordered_set<QueryDeclaration *> returned_declaration_set = query_evaluator->mockCopyDeclarations();

  ASSERT_EQ(declarations_set, returned_declaration_set);
}
TEST(QeCopyDeclarationTest, AllDeclarationsTwiceEach) {
  IPKBQuerier *pkb = new MockPKB();

  auto *stmt_dec_1 = new StatementDeclaration(QuerySynonym("stmt_dec_1"));
  auto *read_dec_1 = new ReadDeclaration(QuerySynonym("read_dec_1"));
  auto *print_dec_1 = new PrintDeclaration(QuerySynonym("print_dec_1"));
  auto *call_dec_1 = new CallDeclaration(QuerySynonym("call_dec_1"));
  auto *while_dec_1 = new WhileDeclaration(QuerySynonym("while_dec_1"));
  auto *if_dec_1 = new IfDeclaration(QuerySynonym("if_dec_1"));
  auto *assign_dec_1 = new AssignDeclaration(QuerySynonym("assign_dec_1"));
  auto *var_dec_1 = new VariableDeclaration(QuerySynonym("var_dec_1"));
  auto *const_dec_1 = new ConstantDeclaration(QuerySynonym("const_dec_1"));
  auto *proc_dec_1 = new ProcedureDeclaration(QuerySynonym("proc_dec_1"));
  auto *expr_dec_1 = new ExpressionDeclaration("x+y");
  auto *str_dec_1 = new StringDeclaration("string_dec_1");
  auto *int_dec_1 = new IntegerDeclaration(123);
  auto *wild_dec_1 = new WildCardDeclaration();

  auto *stmt_dec_2 = new StatementDeclaration(QuerySynonym("stmt_dec_2"));
  auto *read_dec_2 = new ReadDeclaration(QuerySynonym("read_dec_2"));
  auto *print_dec_2 = new PrintDeclaration(QuerySynonym("print_dec_2"));
  auto *call_dec_2 = new CallDeclaration(QuerySynonym("call_dec_2"));
  auto *while_dec_2 = new WhileDeclaration(QuerySynonym("while_dec_2"));
  auto *if_dec_2 = new IfDeclaration(QuerySynonym("if_dec_2"));
  auto *assign_dec_2 = new AssignDeclaration(QuerySynonym("assign_dec_2"));
  auto *var_dec_2 = new VariableDeclaration(QuerySynonym("var_dec_2"));
  auto *const_dec_2 = new ConstantDeclaration(QuerySynonym("const_dec_2"));
  auto *proc_dec_2 = new ProcedureDeclaration(QuerySynonym("proc_dec_2"));
  auto *expr_dec_2 = new ExpressionDeclaration("x+y");
  auto *str_dec_2 = new StringDeclaration("string_dec_2");
  auto *int_dec_2 = new IntegerDeclaration(123);
  auto *wild_dec_2 = new WildCardDeclaration();

  std::vector<QueryDeclaration *> declarations_vector = {
      stmt_dec_1, read_dec_1, print_dec_1, call_dec_1, while_dec_1,
      if_dec_1, assign_dec_1, var_dec_1, const_dec_1, proc_dec_1,
      expr_dec_1, str_dec_1, int_dec_1, wild_dec_1,

      stmt_dec_2, read_dec_2, print_dec_2, call_dec_2, while_dec_2,
      if_dec_2, assign_dec_2, var_dec_2, const_dec_2, proc_dec_2,
      expr_dec_2, str_dec_2, int_dec_2, wild_dec_2
  };
  std::unordered_set<QueryDeclaration *> declarations_set = {
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
  std::unordered_set<QueryDeclaration *> returned_declaration_set = query_evaluator->mockCopyDeclarations();

  ASSERT_EQ(declarations_set, returned_declaration_set);
}

TEST(QeFetchContextTest, StatementDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new StatementDeclaration(QuerySynonym("stmt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::all_stmt_entities_);
}

TEST(QeFetchContextTest, ProcedureDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new ProcedureDeclaration(QuerySynonym("proc"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::procedure_entities_);
}

TEST(QeFetchContextTest, VariableDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new VariableDeclaration(QuerySynonym("stmt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::variable_entities_);
}

TEST(QeFetchContextTest, ConstantDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new ConstantDeclaration(QuerySynonym("stmt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::constant_entities_);
}

TEST(QeFetchContextTest, StmtAssignDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new AssignDeclaration(QuerySynonym("as"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_assign_entities_);
}

TEST(QeFetchContextTest, StmtCallDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new CallDeclaration(QuerySynonym("cl"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_call_entities_);
}

TEST(QeFetchContextTest, StmtIfDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new IfDeclaration(QuerySynonym("ifs"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_if_entities_);
}

TEST(QeFetchContextTest, StmtWhileDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new WhileDeclaration(QuerySynonym("w"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_while_entities_);
}

TEST(QeFetchContextTest, StmtPrintDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new PrintDeclaration(QuerySynonym("prt"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_print_entities_);
}

TEST(QeFetchContextTest, StmtReadDeclaration) {
  IPKBQuerier *pkb = new MockPKB();
  auto *declaration = new ReadDeclaration(QuerySynonym("rd"));
  SelectCall select_call = SelectCall(declaration, {});
  Query query = Query({declaration}, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  query_evaluator->mockFetchContext();

  ASSERT_EQ(select_call.getDeclaration()->getContext(), TestStorage::stmt_read_entities_);
}
