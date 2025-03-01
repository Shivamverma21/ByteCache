#include <bits/stdc++.h>
#include "Storage.h"

using namespace std;

  void Storage::create_key(string key, string value) {
    cache[key] = value;
  }

  void Storage::update_key(string key, string value) {
    if (cache.count(key)) {
      cache[key] = value;
    }
    else {
      cout << "Key not found" << endl;
    }
  }

  void Storage::read_key(string key) {
    if (cache.count(key)) {
      cout << cache[key] << endl;
    }
    else {
      cout << "Key not found" << endl;
    }
  }

  void Storage::delete_key(string key) {
    if (cache.count(key)) {
      cache.erase(key);
    }
  }
