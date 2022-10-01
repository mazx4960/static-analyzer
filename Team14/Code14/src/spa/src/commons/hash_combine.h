#pragma once

#include <functional>

template<class T>
/*
 * Combines hash of two hashes
 */
class HashCombine {
 private:
  static const size_t kAddedVal = 0x9e3779b9;

  static const size_t kLeftShift = 6;

  static const size_t kRightShift = 2;

  HashCombine() = default;
 public:
  static void hash_combine(std::size_t &seed, const T &val) {
    std::hash<T> hash_func;
    seed ^= hash_func(val) + kAddedVal + (seed << kLeftShift) + (seed >> kRightShift);
  }
};
