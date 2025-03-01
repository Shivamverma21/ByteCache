#include <bits/stdc++.h>

using namespace std;

#ifndef SOURCE_H
#define SOURCE_H

class Storage {
private:
    map<string, string> cache;
    queue<string> q;

public:
    void create_key(string key, string value);

    void update_key(string key, string value);

    void read_key(string key);

    void delete_key(string key);

};

#endif //SOURCE_H
