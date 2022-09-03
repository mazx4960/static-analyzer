#pragma once

#include <string>

class Synonym {
 public:
  Synonym(std::string synonym);
  Synonym();
  std::string synonym;
  bool operator==(const Synonym& other) const;
};
