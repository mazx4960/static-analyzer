// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_set>

#include "commons/component_interface/interface.h"
#include "pkb/pkb.h"
#include "commons/evaluator/result.h"
#include "commons/evaluator/pkb_query.h"
#include "commons/relationship.h"

class PKBInterface : public Interface {
 private:
  PKB component_;

 public:
  Result get(PKBQuery &);
  bool save(Relationship &);
};
