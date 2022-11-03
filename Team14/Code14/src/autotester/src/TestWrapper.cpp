#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = nullptr;
AbstractWrapper *WrapperFactory::createWrapper() {
  if (wrapper == nullptr)
    wrapper = new TestWrapper;
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
  try {
    this->ui_->LoadSource();
  } catch (SyntaxError &e) {
    AbstractWrapper::GlobalStop = true;
  } catch (SemanticError &e) {
    AbstractWrapper::GlobalStop = true;
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  this->ui_->SetQueryString(query);
  try {
    auto *result = this->ui_->ExecuteQuery();
    auto result_set = result->GetResults();
    std::copy(result_set.begin(), result_set.end(), std::back_inserter(results));
  } catch (SemanticError &e) {
    results.emplace_back("SemanticError");
    return;
  } catch (SyntaxError &e) {
    results.emplace_back("SyntaxError");
    return;
  }
}
