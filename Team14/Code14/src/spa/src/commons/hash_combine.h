#pragma once

#include <functional>

template<class T>
/*
 * Combines hash of two hashes
 */
class HashCombine {
 private:
  HashCombine() = default;
 public:
  static void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hash_func;
    seed ^= hash_func(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
};
