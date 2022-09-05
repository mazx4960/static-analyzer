// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_set>

#include "commons/component_interface/interface.h"
#include "pkb/public/pkb.h"
#include "pkb/entity/result.h"
#include "pkb/entity/pkb_query.h"
#include "commons/relationship.h"

class PKB : public Interface {
 public:
  Result get(PKBQuery &);
  bool save(Relationship &);
};
