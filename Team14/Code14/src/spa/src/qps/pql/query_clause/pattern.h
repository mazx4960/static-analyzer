// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"

enum class PatternType {
  kPattern
};

class Pattern {
 private:
  PatternType type_ = PatternType::kPattern;
  Entity* first_;
  Entity* second_;

 public:
  Pattern(Entity* first, Entity* second);
  PatternType GetType();
  Entity* GetFirst();
  Entity* GetSecond();
  bool operator==(const Pattern &other) const;
};
