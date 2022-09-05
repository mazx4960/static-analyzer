// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "pkb/entity/result.h"
#include "pkb/entity/pkb_query.h"

class PKB {
 public:
  PKB() = default;
  Result get(PKBQuery &);
};

