#pragma once
#include <bits/stdc++.h>
#include <WinSock2.h>

using namespace std;

# pragma comment(lib, "Ws2_32.lib")

class pubSub {
private:
    unordered_set<string> channels;
    unordered_map<string, vector<SOCKET>> subscriptions;
    unordered_map<string, vector<SOCKET>> publications;
    mutex pubsub_mutex;

public:
    // pub/sub
    string create_channel(const string &channel, SOCKET client);
    string subscribe(const string &channel, SOCKET client);
    string unsubscribe(const string &channel, SOCKET client);
    string publish(const string &channel, const std::string &message, SOCKET client);
    string add_publishers(const string &channel, SOCKET client);
    string remove_publishers(const string &channel, SOCKET client);
    string delete_channel(const string &channel);
};
