// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb_interface.h"
#include "commons/relationship.h"

Result PKBInterface::get(PkbQuery &query) {
  std::vector<std::string> results_placeholder(0);
  Synonym synonym_placeholder = Synonym("placeholder");
  return Result(synonym_placeholder, results_placeholder);
}

bool PKBInterface::save(Relationship &rs) {
  return true;
}