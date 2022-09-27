#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = nullptr;
AbstractWrapper *WrapperFactory::createWrapper() {
  if (wrapper == nullptr) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // Initialize all components.
  this->ui_ = new UI();
  auto *sp = new SP();
  auto *qps = new QPS();
  auto *pkb = new PKB();

  // Set up the component dependencies.
  this->ui_->SetSP(sp);
  this->ui_->SetQPS(qps);
  sp->SetPKB(pkb);
  qps->SetPKB(pkb);
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  this->ui_->SetSourceFile(filename);
  this->ui_->LoadSource();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &final_results_list) {
  this->ui_->SetQueryString(query);
  auto *result = this->ui_->ExecuteQuery();

  if (result->is_empty()) {
    return;
  }

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  std::unordered_set<std::string> results_set = result->get_results_set();
  std::copy(results_set.begin(), results_set.end(), std::back_inserter(final_results_list));
}
