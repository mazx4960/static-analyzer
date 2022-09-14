#include <gtest/gtest.h>

#include <algorithm>
#include "qps/query_evaluator/query_evaluator.h"

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

      case EntityType::kProcedure: {
        return std::unordered_set<Entity *>(
            {
                new ProcedureEntity("procedure1"),
                new ProcedureEntity("procedure2"),
                new ProcedureEntity("procedure3")
            }
        );
      };
      case EntityType::kVariable: {
        return std::unordered_set<Entity *>(
            {
                new VariableEntity("x"),
                new VariableEntity("y"),
                new VariableEntity("z"),
            }
        );
      };
      case EntityType::kConstant: {
        return std::unordered_set<Entity *>(
            {
                new ConstantEntity("1"),
                new ConstantEntity("2"),
                new ConstantEntity("3"),
            }
        );
      };
      case EntityType::kAssignStmt: {
        return std::unordered_set<Entity *>{
            new AssignStmtEntity(1),
            new AssignStmtEntity(2),
            new AssignStmtEntity(3),
        };
      };
      case EntityType::kCallStmt: {
        return std::unordered_set<Entity *>{
            new CallStmtEntity(4),
            new CallStmtEntity(5),
            new CallStmtEntity(6),
        };
      };
      case EntityType::kIfStmt: {
        return std::unordered_set<Entity *>{
            new IfStmtEntity(7),
            new IfStmtEntity(8),
            new IfStmtEntity(9),
        };
      };
      case EntityType::kWhileStmt: {
        return std::unordered_set<Entity *>{
            new WhileStmtEntity(10),
            new WhileStmtEntity(11),
            new WhileStmtEntity(12),
        };
      };
      case EntityType::kPrintStmt: {
        return std::unordered_set<Entity *>{
            new PrintStmtEntity(13),
            new PrintStmtEntity(14),
            new PrintStmtEntity(15),
        };
      };
      case EntityType::kReadStmt: {
        return std::unordered_set<Entity *>{
            new ReadStmtEntity(16),
            new ReadStmtEntity(17),
            new ReadStmtEntity(18),
        };
      };
      case EntityType::kStatement:throw std::runtime_error("This statement should not handle statements!");
      default: throw std::runtime_error("Invalid entity type!");
    };
  }
  inline std::unordered_set<Entity *> getByRelationship(RsType, Entity *, bool) override {
    return std::unordered_set<Entity *>();
  };
  inline std::unordered_set<Entity *> getByPattern(std::string &, std::string &) override {
    return std::unordered_set<Entity *>();
  };
};

TEST(QeCopyDeclarationTest, AllDeclarationsOnceEach) {
  IPKBQuerier *pkb = new MockPKB();

  auto *dec1 = new StatementDeclaration(new QuerySynonym("dec1"));
  auto *dec2 = new ReadDeclaration(new QuerySynonym("dec2"));
  auto *dec3 = new PrintDeclaration(new QuerySynonym("dec3"));
  auto *dec4 = new CallDeclaration(new QuerySynonym("dec4"));
  auto *dec5 = new WhileDeclaration(new QuerySynonym("dec5"));
  auto *dec6 = new IfDeclaration(new QuerySynonym("dec6"));
  auto *dec7 = new AssignDeclaration(new QuerySynonym("dec7"));
  auto *dec8 = new VariableDeclaration(new QuerySynonym("dec8"));
  auto *dec9 = new ConstantDeclaration(new QuerySynonym("dec9"));
  auto *dec10 = new ProcedureDeclaration(new QuerySynonym("dec10"));
  auto *dec11 = new ExpressionDeclaration("x+y");
  auto *dec12 = new StringDeclaration("12");
  auto *dec13 = new IntegerDeclaration(13);
  auto *dec14 = new WildCardDeclaration();

  std::vector<QueryDeclaration *> declarations_vector = {
      dec1, dec2, dec3, dec4, dec5, dec6, dec7,
      dec8, dec9, dec10, dec11, dec12, dec13, dec14
  };
  std::unordered_set<QueryDeclaration *> declarations_set = {
      dec1, dec2, dec3, dec4, dec5, dec6, dec7,
      dec8, dec9, dec10, dec11, dec12, dec13, dec14
  };

  SelectCall select_call = SelectCall(dec1, {});
  Query query = Query(declarations_vector, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  std::unordered_set<QueryDeclaration *> returned_declaration_set = query_evaluator->mockCopyDeclarations();

  ASSERT_EQ(declarations_set, returned_declaration_set);
}
TEST(QeCopyDeclarationTest, AllDeclarationsTwiceEach) {
  IPKBQuerier *pkb = new MockPKB();

  auto *dec1_1 = new StatementDeclaration(new QuerySynonym("dec1_1"));
  auto *dec1_2 = new StatementDeclaration(new QuerySynonym("dec1_2"));
  auto *dec2_1 = new ReadDeclaration(new QuerySynonym("dec2_1"));
  auto *dec2_2 = new ReadDeclaration(new QuerySynonym("dec2_2"));
  auto *dec3_1 = new PrintDeclaration(new QuerySynonym("dec3_1"));
  auto *dec3_2 = new PrintDeclaration(new QuerySynonym("dec3_2"));
  auto *dec4_1 = new CallDeclaration(new QuerySynonym("dec4_1"));
  auto *dec4_2 = new CallDeclaration(new QuerySynonym("dec4_2"));
  auto *dec5_1 = new WhileDeclaration(new QuerySynonym("dec5_1"));
  auto *dec5_2 = new WhileDeclaration(new QuerySynonym("dec5_2"));
  auto *dec6_1 = new IfDeclaration(new QuerySynonym("dec6_1"));
  auto *dec6_2 = new IfDeclaration(new QuerySynonym("dec6_2"));
  auto *dec7_1 = new AssignDeclaration(new QuerySynonym("dec7_1"));
  auto *dec7_2 = new AssignDeclaration(new QuerySynonym("dec7_2"));
  auto *dec8_1 = new VariableDeclaration(new QuerySynonym("dec8_1"));
  auto *dec8_2 = new VariableDeclaration(new QuerySynonym("dec8_2"));
  auto *dec9_1 = new ConstantDeclaration(new QuerySynonym("dec9_1"));
  auto *dec9_2 = new ConstantDeclaration(new QuerySynonym("dec9_2"));
  auto *dec10_1 = new ProcedureDeclaration(new QuerySynonym("dec10_1"));
  auto *dec10_2 = new ProcedureDeclaration(new QuerySynonym("dec10_2"));
  auto *dec11_1 = new ExpressionDeclaration("x+y+1");
  auto *dec11_2 = new ExpressionDeclaration("x+y+2");
  auto *dec12_1 = new StringDeclaration("12_1");
  auto *dec12_2 = new StringDeclaration("12_2");
  auto *dec13_1 = new IntegerDeclaration(131);
  auto *dec13_2 = new IntegerDeclaration(132);
  auto *dec14_1 = new WildCardDeclaration();
  auto *dec14_2 = new WildCardDeclaration();

  std::vector<QueryDeclaration *> declarations_vector = {
      dec1_1, dec1_2, dec2_1, dec2_2, dec3_1, dec3_2,
      dec4_1, dec4_2, dec5_1, dec5_2, dec6_1, dec6_2,
      dec7_1, dec7_2, dec8_1, dec8_2, dec9_1, dec9_2,
      dec10_1, dec10_2, dec11_1, dec11_2, dec12_1, dec12_2,
      dec13_1, dec13_2, dec14_1, dec14_2
  };
  std::unordered_set<QueryDeclaration *> declarations_set = {
      dec1_1, dec1_2, dec2_1, dec2_2, dec3_1, dec3_2,
      dec4_1, dec4_2, dec5_1, dec5_2, dec6_1, dec6_2,
      dec7_1, dec7_2, dec8_1, dec8_2, dec9_1, dec9_2,
      dec10_1, dec10_2, dec11_1, dec11_2, dec12_1, dec12_2,
      dec13_1, dec13_2, dec14_1, dec14_2
  };

  SelectCall select_call = SelectCall(dec1_1, {});
  Query query = Query(declarations_vector, select_call);

  auto *query_evaluator = new QueryEvaluatorMock(pkb, query);
  std::unordered_set<QueryDeclaration *> returned_declaration_set = query_evaluator->mockCopyDeclarations();

  ASSERT_EQ(declarations_set, returned_declaration_set);
}
