#include "pubSub.h"
#include <bits/stdc++.h>

using namespace std;

string pubSub::create_channel(const std::string &channel, SOCKET client) {
    lock_guard<mutex> lock(pubsub_mutex); // Thread safety

    if (channels.find(channel) != channels.end()) {
        return "Channel already exists";
    }
    // Check if already a publisher
    auto &publishers = publications[channel];
    publishers.push_back(client);

    channels.insert(channel);
    return "Channel created successfully";
}

string pubSub::delete_channel(const std::string &channel) {
    std::cout << "Deleting channel: " << channel << std::endl;
    channels.erase(channel);
    publications.erase(channel);
    subscriptions.erase(channel);
    return "Channel deleted as there is no publisher for : " + channel;
}

string pubSub::subscribe(const string &channel, SOCKET client) {
    lock_guard<mutex> lock(pubsub_mutex); // Thread safety

    if (channels.find(channel) == channels.end()) {
        return "Channel does not exist.";
    }

    // Check if already subscribed
    auto &subscribers = subscriptions[channel];
    if (std::find(subscribers.begin(), subscribers.end(), client) != subscribers.end()) {
        return "Already subscribed.";
    }

    subscribers.push_back(client);
    return "Subscribed to channel: " + channel;
}

string pubSub::unsubscribe(const std::string &channel, SOCKET client) {
    lock_guard<mutex> lock(pubsub_mutex); // Thread safety

    if (channels.find(channel) == channels.end()) {
        return "Channel does not exist.";
    }

    auto &subscribers = subscriptions[channel];

    auto it = std::find(subscribers.begin(), subscribers.end(), client);
    if (it != subscribers.end()) {
        subscribers.erase(it);
        return "Unsubscribed from channel: " + channel;
    }

    return "Client was not subscribed to the channel.";
}

string pubSub::publish(const std::string &channel, const std::string &message, SOCKET client) {
    std::lock_guard<std::mutex> lock(pubsub_mutex); // Thread safety

    if (channels.find(channel) == channels.end()) {
        return "Channel does not exist. Please create channel first";
    }

    auto &publishers = publications[channel];
    if (std::find(publishers.begin(), publishers.end(), client) == publishers.end()) {
        return "Can't publish, not a publishers to " + channel;
    }

    if (subscriptions[channel].empty()) {
        return "No subscribers for this channel.";
    }

    std::string fullMessage = "Message from " + channel + ": " + message;

    // Send to all subscribers
    auto &subscribers = subscriptions[channel];
    for (auto it = subscribers.begin(); it != subscribers.end();) {
        SOCKET subscriber = *it;
        if (subscriber == client) {
            ++it;
            continue;
        }
        int result = send(subscriber, fullMessage.c_str(), fullMessage.size(), 0);

        if (result == SOCKET_ERROR) {
            std::cerr << "Failed to send to client " << client << ". Removing from subscribers." << std::endl;
            it = subscribers.erase(it);
        } else {
            ++it;
        }
    }

    return "Message published successfully.";
}

string pubSub::add_publishers(const string &channel, SOCKET client) {
    lock_guard<mutex> lock(pubsub_mutex);

    if (channels.find(channel) == channels.end()) {
        return "Channel does not exist.";
    }

    // Check if already a publisher
    auto &publishers = publications[channel];
    if (std::find(publishers.begin(), publishers.end(), client) != publishers.end()) {
        return "Already a publisher of channel: ." + channel;
    }

    publishers.push_back(client);
    return "Publishers to channel: " + channel;
}

string pubSub::remove_publishers(const std::string &channel, SOCKET client) {
    lock_guard<mutex> lock(pubsub_mutex); // Thread safety

    if (channels.find(channel) == channels.end()) {
        return "Channel does not exist.";
    }

    std::cout << "Removing publisher from channel: " << channel << std::endl;

    auto &publishers = publications[channel];

    auto it = std::find(publishers.begin(), publishers.end(), client);
    if (it != publishers.end()) {
        publishers.erase(it);
        std::cout << "Publisher removed from channel: " << channel << std::endl;
        std::cout << "Publisher empty hai ?: " << publishers.empty() << std::endl;
        if (publishers.empty()) {
            cout << "No publishers for this channel." << std::endl;
            delete_channel(channel);
        }
        std::cout << "uaha ?: " << endl;
        return "Remove publisher from channel: " + channel;
    }

    return "Client was not publishers to the channel.";
}
