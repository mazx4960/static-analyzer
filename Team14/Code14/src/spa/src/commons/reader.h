// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <fstream>

class StreamReader {
 public:
  static std::ifstream GetStreamFromFile(const std::string& filename);
};
