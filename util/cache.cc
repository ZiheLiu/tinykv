//
// Created by liuzihe on 2020/10/16.
//

#include <mutex>
#include <unordered_map>

#include "util/cache.h"
#include "util/hash.h"
#include "util/mutexlock.h"
#include "util/slice.h"
#include "constants.h"

namespace tinykv {
  namespace {

    struct Node {
      std::string value;
      int charge;
      Node *prev;
      Node *next;
      std::unordered_map<std::string, Node*>::iterator it;

      Node(): charge(0), prev(nullptr), next(nullptr) {}

      explicit Node(std::string value, int charge):
        value(std::move(value)), charge(charge), prev(nullptr), next(nullptr) {}

      ~Node() = default;
    };

    class LRUCache : public Cache {
    public:
      explicit LRUCache(): capacity_(0), usage_(0) {
        head_.next = head_.prev = &head_;
      }

      ~LRUCache() override {
        table_.clear();
        for (Node *node = head_.next; node != &head_; ) {
          Node *next_node = node->next;
          delete node;
          node = next_node;
        }
      }

      void set_capacity(size_t capacity) {
        capacity_ = capacity;
      }

      bool Get(const std::string &key, Slice* result, char *value_result) override;

      void Put(const std::string &key, const std::string &value, size_t charge) override;

    private:
      size_t capacity_;
      size_t usage_;
      Node head_;
      std::unordered_map<std::string, Node*> table_;
      std::mutex mutex_;

      void RemoveFromList(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
      }

      void AppendToList(Node* node) {
        head_.prev->next = node;
        node->prev = head_.prev;
        node->next = &head_;
        head_.prev = node;
      }
    };

    bool LRUCache::Get(const std::string &key, Slice* result, char *payload) {
      MutexLock l(&mutex_);

      auto it = table_.find(key);
      if (it == table_.end()) {
        return false;
      }
      Node* node = it->second;
      const std::string& value = node->value;
      memcpy(payload, value.data(), value.size());
      *result = Slice(payload, value.size());

      RemoveFromList(node);
      AppendToList(node);

      return true;
    }

    void LRUCache::Put(const std::string &key, const std::string &value, size_t charge) {
      MutexLock l(&mutex_);

      if (table_.find(key) != table_.end()) {
        return;
      }

      Node *node = new Node(value, charge);
      AppendToList(node);
      table_[key] = node;
      node->it = table_.find(key);

      usage_ += charge;
      while (usage_ >= capacity_ && head_.next != &head_) {
        Node *old = head_.next;
        RemoveFromList(old);
        table_.erase(old->it);
        usage_ -= old->charge;
        delete old;
      }
    }

    class ShardedLRUCache : public Cache {
    public:
      explicit ShardedLRUCache(size_t capacity) {
        const size_t per_shard = (capacity + (kNumCacheShards - 1)) / kNumCacheShards;
        for (int i = 0; i < kNumCacheShards; i++) {
          shard_[i].set_capacity(per_shard);
        }
      }

      ~ShardedLRUCache() override = default;

      bool Get(const std::string &key, Slice* result, char *payload) override {
        const uint32_t h = HashShard(key);
        return shard_[h].Get(key, result, payload);
      }

      void Put(const std::string &key, const std::string &value, size_t charge) override {
        const uint32_t h = HashShard(key);
        shard_[h].Put(key, value, charge);
      }

    private:
      LRUCache shard_[kNumCacheShards];

      static inline uint32_t HashShard(const std::string &key) {
        uint32_t h = Hash(key.data(), key.size(), 0xff77ff77);
        return h >> (32 - kNumCacheShardBits);
      }
    };

  } // End anonymous namespace.

  Cache *NewLRUCache(size_t capacity) {
    return new ShardedLRUCache(capacity);
  }

} // End tinykv namespace.