#pragma once

#include <functional>

/*
 * Combines hash of two hashes
 */
template<class T>
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

struct PairHash {
  template<class A, class B>
  std::size_t operator()(const std::pair<A, B> &p) const {
    std::size_t seed = 0;
    HashCombine<A>::hash_combine(seed, p.first);
    HashCombine<B>::hash_combine(seed, p.second);
    return seed;
  }
};

struct TripletHash {
  template<class A, class B, class C>
  std::size_t operator()(const std::tuple<A, B, C> &t) const {
    std::size_t seed = 0;
    HashCombine<A>::hash_combine(seed, std::get<0>(t));
    HashCombine<B>::hash_combine(seed, std::get<1>(t));
    HashCombine<C>::hash_combine(seed, std::get<2>(t));
    return seed;
  }
};
