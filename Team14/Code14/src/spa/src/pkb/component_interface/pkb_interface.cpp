// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb_interface.h"
#include "commons/relationship.h"

Result PKBInterface::get(PKBQuery &query) {
  QuerySynonym synonym_placeholder = QuerySynonym("placeholder");
  return Result::empty(synonym_placeholder);
}

bool PKBInterface::save(Relationship &rs) {
  return true;
}