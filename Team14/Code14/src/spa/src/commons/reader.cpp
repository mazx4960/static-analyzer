// Copyright 2022 CS3203 Team14. All rights reserved.

#include "reader.h"

#include <sstream>
#include <string>

std::istream* StreamReader::GetStreamFromFile(const std::string& filename) {
  auto* file_stream = new std::ifstream(filename);
  if (!file_stream->is_open()) { throw std::runtime_error("Unable to open file: " + filename); }
  return file_stream;
}
std::istream* StreamReader::GetStreamFromString(const std::string& content) {
  auto* string_stream = new std::stringstream(content);
  return string_stream;
}
