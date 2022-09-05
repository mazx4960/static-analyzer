// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_set>

#include "commons/relationship.h"
#include "pkb.h"
#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"

class PKB {
 public:
  PKB() = default;
  Result get(PKBQuery &);
  bool save(Relationship &);
};
