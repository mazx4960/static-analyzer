// Copyright 2022 CS3203 Team14. All rights reserved.

#include "reader.h"

#include <string>

std::ifstream StreamReader::GetStreamFromFile(const std::string& filename) {
  return std::ifstream(filename);
}
