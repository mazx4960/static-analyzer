#include "gtest/gtest.h"

#include "qps/result.h"
#include "qps/pql/query_synonym.h"

struct ResultTestStatics {
 public:
  inline static const std::vector<ElemReference *> kElemRefs = {
      new SynonymReference(new QuerySynonym("var_x", EntityType::kVariable)),
      new SynonymReference(new QuerySynonym("assign_y", EntityType::kAssignStmt)),
      new SynonymReference(new QuerySynonym("proc_z", EntityType::kProcedure))
  };

};

TEST(ResultCreationTest, AllSynonymFullTable) {
  auto *result = new Result(ResultTestStatics::kElemRefs, {"a 1 main", "b 2 do_that", "a 3 do_this"});

  std::unordered_set<std::string> exp_set = {"a 1 main", "b 2 do_that", "a 3 do_this"};

  ASSERT_EQ(result->GetResults(), exp_set);
}

TEST(BoolResultCreationTest, HasResults) {
  auto *bool_result = Result::True();

  std::unordered_set<std::string> exp_set = {"TRUE"};

  ASSERT_EQ(bool_result->GetResults(), exp_set);
}

TEST(BoolResultCreationTest, NoResults) {
  auto *bool_result = Result::False();

  std::unordered_set<std::string> exp_set = {"FALSE"};

  ASSERT_EQ(bool_result->GetResults(), exp_set);
}
