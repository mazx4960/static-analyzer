#pragma once

#include <vector>

template<typename T>
class QpsTestHelper {
 public:
  static bool CompareVectors(const std::vector<T *> &results, const std::vector<T *> &expected) {
    if (results.size() != expected.size()) {
      return false;
    }

    // iterate through all of results
    return std::all_of(results.begin(), results.end(), [&expected](T *result) {
      // iterate through all of expected
      return std::any_of(expected.begin(), expected.end(), [&result](T *expected_result) {
        return *result == *expected_result;
      });
    });
  };
};