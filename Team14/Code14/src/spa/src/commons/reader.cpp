// Copyright 2022 CS3203 Team14. All rights reserved.

#include "reader.h"

#include <string>

std::ifstream StreamReader::GetStreamFromFile(const std::string& filename) {
  std::ifstream file_stream;
  file_stream.open(filename);
  if (!file_stream.is_open()) { throw std::runtime_error("Unable to open file: " + filename); }
  return file_stream;
}
