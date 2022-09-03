// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/component_interface/interface.h"
#include "pkb/pkb.h"
#include "commons/evaluator/result.h"
#include "commons/evaluator/pkb_query.h"
#include "commons/relationship.h"
#include <string>
#include <vector>

class PKBInterface : public Interface {
 private:
  PKB component_;

 public:
  Result get(PkbQuery &);
  bool save(Relationship &);
};
