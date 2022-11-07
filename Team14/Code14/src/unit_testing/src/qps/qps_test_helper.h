#pragma once

#include <vector>

template<typename T>
class QpsTestHelper {
 public:
  static bool CompareVectors(const std::vector<T *> &results, const std::vector<T *> &expected) {
    bool result = true;

    for (auto &r : results) {
      bool found = false;
      for (auto &e : expected) {
        if (*r == *e) {
          found = true;
          break;
        }
      }
      result = result && found;
    }

    for (auto &e : expected) {
      bool found = false;
      for (auto &r : results) {
        if (*r == *e) {
          found = true;
          break;
        }
      }
      result = result && found;
    }

    return result;
  };
};