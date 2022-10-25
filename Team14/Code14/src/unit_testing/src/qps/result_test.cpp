#include "gtest/gtest.h"
#include "qps/result.h"
#include "qps/pql/query_synonym.h"
#include "qps/query_evaluator/subquery_result.h"

using ResultRow = std::unordered_map<QuerySynonym *,
                                     Entity *,
                                     QuerySynonymHashFunction,
                                     QuerySynonymPointerEquality>;

struct ResultTestStatics {
 public:

  inline static std::vector<QuerySynonym *> syn_vec_1_ = {
      new QuerySynonym("var_x"),
      new QuerySynonym("assign_y"),
      new QuerySynonym("proc_z")
  };

  inline static std::vector<ElemReference *> elemref_vec_var_ = {
      new VariableDeclaration(syn_vec_1_[0]),
  };
  inline static std::vector<ElemReference *> elemref_vec_full_ = {
      new VariableDeclaration(syn_vec_1_[0]),
      new AssignDeclaration(syn_vec_1_[1]),
      new ProcedureDeclaration(syn_vec_1_[2])
  };

  inline static std::vector<ElemReference *> elemref_vec_full_shuffled_ = {
      new AssignDeclaration(syn_vec_1_[1]),
      new ProcedureDeclaration(syn_vec_1_[2]),
      new VariableDeclaration(syn_vec_1_[0])
  };

  inline static ResultRow row1_ = {
      {new QuerySynonym("var_x"), new VariableEntity("a")},
      {new QuerySynonym("assign_y"), new AssignStmtEntity("1")},
      {new QuerySynonym("proc_z"), new ProcedureEntity("main")}
  };

  inline static ResultRow row2_ = {
      {new QuerySynonym("var_x"), new VariableEntity("b")},
      {new QuerySynonym("assign_y"), new AssignStmtEntity("2")},
      {new QuerySynonym("proc_z"), new ProcedureEntity("do_that")}
  };

  inline static ResultRow row3_ = {
      {new QuerySynonym("var_x"), new VariableEntity("a")},
      {new QuerySynonym("assign_y"), new AssignStmtEntity("3")},
      {new QuerySynonym("proc_z"), new ProcedureEntity("do_this")}
  };

  inline static std::vector<ResultRow> full_result_rows_ = {row1_, row2_, row3_};
  inline static std::vector<ResultRow> empty_result_rows_ = {};

  inline static SubqueryResult all_rows_ = SubqueryResult(syn_vec_1_, full_result_rows_);

  inline static SubqueryResult no_rows_ = SubqueryResult(syn_vec_1_, empty_result_rows_);

  // Table without any synonyms declared
  inline static SubqueryResult no_syns_all_rows_ = SubqueryResult({}, {row1_, row2_, row3_});

  // Table with completely different synonyms
  inline static ResultRow row_diff_syn_ = {
      {new QuerySynonym("var_a"), new VariableEntity("a")},
      {new QuerySynonym("assign_b"), new AssignStmtEntity("3")},
      {new QuerySynonym("proc_c"), new ProcedureEntity("do_this")}
  };

  inline static SubqueryResult no_syns_diff_row_ = SubqueryResult({}, {row_diff_syn_});

  // Table with partially different synonyms
  inline static ResultRow row_partial_diff_syn_ = {
      {new QuerySynonym("var_a"), new VariableEntity("a")},
      {new QuerySynonym("assign_y"), new AssignStmtEntity("3")},
      {new QuerySynonym("proc_z"), new ProcedureEntity("do_this")}
  };

  inline static SubqueryResult no_syns_partial_diff_row_ = SubqueryResult({}, {row_partial_diff_syn_});

};

TEST(ElemResultCreationTest, AllSynonymFullTable) {
  Result *elem_result = new ElemResult(ResultTestStatics::elemref_vec_full_, ResultTestStatics::all_rows_);
  ASSERT_EQ(elem_result->get_synonyms(), "{ Variable:var_x, AssignStmt:assign_y, Procedure:proc_z }");
  ASSERT_FALSE(elem_result->is_empty());
  ASSERT_EQ(elem_result->size(), 3);

  std::unordered_set<std::string> exp_set = {
      "a 1 main",
      "b 2 do_that",
      "a 3 do_this"
  };

  ASSERT_EQ(elem_result->get_results_set(), exp_set);
}

TEST(ElemResultCreationTest, AllSynonymShuffledFullTable) {
  Result *elem_result = new ElemResult(ResultTestStatics::elemref_vec_full_shuffled_, ResultTestStatics::all_rows_);
  ASSERT_EQ(elem_result->get_synonyms(), "{ AssignStmt:assign_y, Procedure:proc_z, Variable:var_x }");
  ASSERT_FALSE(elem_result->is_empty());
  ASSERT_EQ(elem_result->size(), 3);

  std::unordered_set<std::string> exp_set = {
      "1 main a",
      "2 do_that b",
      "3 do_this a"
  };

  ASSERT_EQ(elem_result->get_results_set(), exp_set);
}

TEST(ElemResultCreationTest, AllSynonymEmptyTable) {
  Result *elem_result = new ElemResult({ResultTestStatics::elemref_vec_full_}, ResultTestStatics::no_rows_);
  ASSERT_EQ(elem_result->get_synonyms(), "{ Variable:var_x, AssignStmt:assign_y, Procedure:proc_z }");
  ASSERT_TRUE(elem_result->is_empty());
  ASSERT_EQ(elem_result->size(), 0);

  std::unordered_set<std::string> exp_set = {};

  ASSERT_EQ(elem_result->get_results_set(), exp_set);
}

TEST(ElemResultCreationTest, SingleSynFullTable) {
  Result *elem_result = new ElemResult(ResultTestStatics::elemref_vec_var_, ResultTestStatics::all_rows_);
  ASSERT_EQ(elem_result->get_synonyms(), "{ Variable:var_x }");
  ASSERT_FALSE(elem_result->is_empty());
  ASSERT_EQ(elem_result->size(), 2);

  std::unordered_set<std::string> exp_set = {"a", "b"};

  ASSERT_EQ(elem_result->get_results_set(), exp_set);
}

TEST(ElemResultCreationTest, SingleSynEmptyTable) {
  Result *elem_result = new ElemResult({ResultTestStatics::elemref_vec_var_}, ResultTestStatics::no_rows_);
  ASSERT_EQ(elem_result->get_synonyms(), "{ Variable:var_x }");
  ASSERT_TRUE(elem_result->is_empty());
  ASSERT_EQ(elem_result->size(), 0);

  std::unordered_set<std::string> exp_set = {};

  ASSERT_EQ(elem_result->get_results_set(), exp_set);
}

/*
 * Test that Result throws an error when synonym is not in table.
 */
TEST(ElemResultCreationTest, SynNotInTable) {
  ASSERT_THROW(new ElemResult(ResultTestStatics::elemref_vec_var_, ResultTestStatics::no_syns_diff_row_), ResultCreationError);
  ASSERT_THROW(new ElemResult(ResultTestStatics::elemref_vec_full_, ResultTestStatics::no_syns_diff_row_), ResultCreationError);

  ASSERT_THROW(new ElemResult(ResultTestStatics::elemref_vec_var_, ResultTestStatics::no_syns_all_rows_), ResultCreationError);
  ASSERT_THROW(new ElemResult(ResultTestStatics::elemref_vec_full_, ResultTestStatics::no_syns_all_rows_), ResultCreationError);

  ASSERT_THROW(new ElemResult(ResultTestStatics::elemref_vec_var_, ResultTestStatics::no_syns_partial_diff_row_), ResultCreationError);
  ASSERT_THROW(new ElemResult(ResultTestStatics::elemref_vec_full_, ResultTestStatics::no_syns_partial_diff_row_), ResultCreationError);
}

TEST(BoolResultCreationTest, HasResults) {
  Result *bool_result = new BooleanResult(true);
  ASSERT_EQ(bool_result->get_synonyms(), "{ BOOLEAN:BOOLEAN }");
  ASSERT_FALSE(bool_result->is_empty());

  std::unordered_set<std::string> exp_set = {"TRUE"};

  ASSERT_EQ(bool_result->get_results_set(), exp_set);
}

TEST(BoolResultCreationTest, NoResults) {
  Result *bool_result = new BooleanResult(false);
  ASSERT_EQ(bool_result->get_synonyms(), "{ BOOLEAN:BOOLEAN }");
  ASSERT_FALSE(bool_result->is_empty());

  std::unordered_set<std::string> exp_set = {"FALSE"};

  ASSERT_EQ(bool_result->get_results_set(), exp_set);
}

TEST(EmptyResultCreationTest, EmptyTable) {
  Result *empty_table = Result::empty();
  ASSERT_EQ(empty_table->get_synonyms(), "{ EMPTY:EMPTY }");
  ASSERT_TRUE(empty_table->is_empty());

  std::unordered_set<std::string> exp_set = {};

  ASSERT_EQ(empty_table->get_results_set(), exp_set);
}
