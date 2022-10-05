// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <list>
#include <unordered_map>

/**
 * LRU cache implementation.
 * @tparam K key type
 * @tparam V value type
 */
template<class K, class V, class KHash = std::hash<K>>
class Cache {
  struct Result {
    V value;
    bool found;
  };

 private:
  int hits_, misses_;
  int capacity_;
  std::unordered_map<K, V, KHash> map_;
  std::list<K> access_list_;

  void update_access(const K &key) {
    access_list_.remove(key);
    access_list_.push_front(key);
  }

  void evict() {
    if (map_.size() > capacity_) {
      auto key = access_list_.back();
      map_.erase(key);
      access_list_.pop_back();
    }
  }

 public:
  explicit Cache(int capacity) { this->capacity_ = capacity; };
  /**
   * Puts a key-value pair into the cache.
   * If the key already exists, the value will be updated.
   * @tparam K
   * @tparam V
   * @param key
   * @param value
   */
  void put(K key, V value) {
    if (this->map_.find(key) != this->map_.end()) { this->map_.erase(key); }
    this->map_.insert({key, value});
    this->evict();
    this->update_access(key);
  };
  /**
   * Gets a value from the cache.
   * @tparam K
   * @tparam V
   * @param key
   * @return
   */
  Result get(K key) {
    Result result;
    if (this->map_.find(key) != this->map_.end()) {
      this->hits_++;
      this->update_access(key);
      result.value = this->map_.at(key);
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
  void clear() {
    this->map_.clear();
    this->access_list_.clear();
  };
  [[nodiscard]] int size() const { return this->map_.size(); };
  [[nodiscard]] int hits() const { return this->hits_; };
  [[nodiscard]] int misses() const { return this->misses_; };
};
