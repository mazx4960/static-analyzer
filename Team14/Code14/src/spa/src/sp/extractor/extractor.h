// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "sp/ast/simpleast.h"

class Extractor {
 protected:
  ProgramNode* program_node_;

 public:
  explicit Extractor(ProgramNode* program_node) : program_node_(program_node){};
  ~Extractor() = default;
  virtual std::vector<Entity> extract(){};
};