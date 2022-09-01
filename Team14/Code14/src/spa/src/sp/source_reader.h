// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <fstream>

class SourceReader {
 public:
  static std::ifstream get_source_stream(const std::string& filename);
};