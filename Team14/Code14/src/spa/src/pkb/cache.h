// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <list>
#include <unordered_map>

/**
 * LRU cache implementation.
 * @tparam K key type
 * @tparam V value type
 * @tparam KHash hash function for key
 */
template<class K, class V, class KHash = std::hash<K>>
class Cache {
  struct Result {
    V value;
    bool found;
  };

 private:
  int hits_ = 0, misses_ = 0;
  int capacity_ = 0;
  std::list<std::pair<K, V>> access_list_;
  std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator, KHash> map_;

  void UpdateAccess(const K &key) {
    access_list_.splice(access_list_.begin(), access_list_, map_[key]);
  }

  void Evict() {
    if (map_.size() == capacity_) {
      auto key = access_list_.back().first;
      access_list_.pop_back();
      map_.erase(key);
    }
  }

 public:
  explicit Cache(int capacity) {
    this->capacity_ = capacity;
  };
  /**
   * Puts a key-value pair into the cache.
   * If the key already exists, the value will be updated.
   * @tparam K
   * @tparam V
   * @param key
   * @param value
   */
  void Add(K key, V value) {
    if (this->map_.find(key) != this->map_.end()) {
      this->UpdateAccess(key);
      this->map_[key]->second = value;
    } else {
      this->Evict();
      this->access_list_.push_front(std::make_pair(key, value));
      this->map_[key] = this->access_list_.begin();
    }
  };
  /**
   * Gets a value from the cache.
   * @tparam K
   * @tparam V
   * @param key
   * @return
   */
  Result Get(K key) {
    Result result;
    if (this->map_.find(key) != this->map_.end()) {
      this->hits_++;
      this->UpdateAccess(key);
      result.value = this->map_.at(key)->second;
      result.found = true;
    } else {
      this->misses_++;
      result.value = V();
      result.found = false;
    }
    return result;
  };
  /**
   * Clears the cache.
   */
  void Clear() {
    this->map_.clear();
    this->access_list_.clear();
  };
  [[nodiscard]] int Size() const {
    return this->map_.size();
  };
  [[nodiscard]] int Hits() const {
    return this->hits_;
  };
  [[nodiscard]] int Misses() const {
    return this->misses_;
  };
};
