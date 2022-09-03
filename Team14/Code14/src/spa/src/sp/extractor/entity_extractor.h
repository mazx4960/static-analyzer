// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "extractor.h"

class EntityExtractor : public Extractor {
 public:
  explicit EntityExtractor(ProgramNode* program_node) : Extractor(program_node){};
  ~EntityExtractor() = default;
  std::vector<Entity> extract() override;
};
