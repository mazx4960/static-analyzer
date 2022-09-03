#pragma once

#include "qps/qpl/synonym/synonym.h"

class QueryDeclaration {
 public:
  QueryDeclaration();
  QueryDeclaration(std::string entity, Synonym synonym);
  std::string entity;
  Synonym synonym;
};

