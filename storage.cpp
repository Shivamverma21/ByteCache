#include <bits/stdc++.h>
#include "storage.h"
#include <thread>

using namespace std;

string storage::create_key(const std::string &key, const std::string &value, int ttl) {
  std::lock_guard<std::mutex> lock(mutex);
  cache[key] = value;
  bool defaultTTL=true;
  if (ttl > 0) {
    defaultTTL=false;
    ttlMap[key] = std::chrono::steady_clock::now() + std::chrono::seconds(ttl);
  }
  if(defaultTTL) {
    return "Key got created with default TTL : " + std::to_string(ttl);
  }
  return "Key got created with TTL : " + std::to_string(ttl);
}

std::string storage::read_key(const std::string &key) {
  std::lock_guard<std::mutex> lock(mutex);
  expire_keys();

  if (cache.find(key) != cache.end()) {
    cout << "Key: " << key << " Expired: " << cache[key] << endl;
    return cache[key];
  }
  cout << "Key: " << key << " Not found" << endl;
  return "Key not found";
}

string storage::update_key(const std::string &key, const std::string &value) {
  std::lock_guard<std::mutex> lock(mutex);
  if (cache.find(key) != cache.end()) {
    cache[key] = value;
    return "Key updated";
  } else {
    return "Key does not exist";
  }
}

string storage::delete_key(const std::string &key) {
  std::lock_guard<std::mutex> lock(mutex);
  cache.erase(key);
  ttlMap.erase(key);
  return "Key deleted";
}

void storage::expire_keys() {
  auto now = std::chrono::steady_clock::now();
  for (auto it = ttlMap.begin(); it != ttlMap.end();) {
    if (now >= it->second) {
      cache.erase(it->first);
      it = ttlMap.erase(it);
    } else {
      ++it;
    }
  }
}

// Queue operations
void storage::enqueue(const std::string &value) {
  std::lock_guard<std::mutex> lock(mutex);
  q.push(value);
}

std::string storage::dequeue() {
  std::lock_guard<std::mutex> lock(mutex);
  if (q.empty()) return "Queue is empty";
  std::string front = q.front();
  q.pop();
  return front;
}
