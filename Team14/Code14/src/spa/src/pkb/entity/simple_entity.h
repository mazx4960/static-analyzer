// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <list>
#include <map>
#include <string>

class SIMPLEEntity {
 public:
  virtual std::list<std::string>*
  populate(std::string entity) = 0;
};
