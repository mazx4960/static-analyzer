#pragma once

#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <unordered_set>

// include your other headers here
#include "AbstractWrapper.h"
#include "ui/ui.h"

class TestWrapper : public AbstractWrapper {
 private:
  UI *ui_;

 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper() = default;

  // method for parsing the SIMPLE source
  void parse(std::string filename) override;

  // method for evaluating a query
  void evaluate(std::string query, std::list<std::string> &results) override;
};
