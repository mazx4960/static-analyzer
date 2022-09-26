// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <fstream>

class StreamReader {
 public:
  static std::istream *GetStreamFromFile(const std::string &filename);
  static std::istream *GetStreamFromString(const std::string &content);
};
