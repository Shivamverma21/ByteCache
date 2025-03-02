#pragma once
#include <bits/stdc++.h>

using namespace std;

class storage {
private:
    unordered_map<string, string> cache;
    unordered_map<string, chrono::steady_clock::time_point> ttlMap;
    queue<string> q;
    mutex mtx;

public:
    string create_key(const std::string &key, const std::string &value, int ttl = -1);
    string read_key(const std::string &key);
    string update_key(const std::string &key, const std::string &value);
    string delete_key(const std::string &key);
    void expire_keys();

    // Queue operations
    void enqueue(const std::string &value);
    string dequeue();
};
