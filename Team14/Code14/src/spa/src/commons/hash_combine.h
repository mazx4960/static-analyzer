#pragma once

#include <functional>

template<class T>
/*
 * Combines hash of two hashes
 */
class HashCombine {
 private:
  static const size_t ADDED_VAL = 0x9e3779b9;

  static const size_t LEFT_SHIFT = 6;

  static const size_t RIGHT_SHIFT = 2;

  HashCombine() = default;
 public:
  static void hash_combine(std::size_t &seed, const T &val) {
    std::hash<T> hash_func;
    seed ^= hash_func(val) + ADDED_VAL + (seed << LEFT_SHIFT) + (seed >> RIGHT_SHIFT);
  }
};
