#include "server.h"
#include "storage.h"
#include "pubSub.h"


storage kv_store;
pubSub pub_sub;

bool Initialize() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

void InteractWithClient(SOCKET clientSocket, vector<SOCKET>& clients) {
    cout << "Client connected" << endl;

    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            printf("Receive failed (%d).\n", WSAGetLastError());
            break;
        } else {
            buffer[bytesReceived] = '\0';  // Ensure null termination
        }
        string message(buffer, bytesReceived);
        cout << "Message from " << message << endl;

        // Tokenize the message
        vector<string> tokens;
        stringstream ss(message);
        string token;
        while (ss >> token) {
            tokens.push_back(token);
        }

        string response;
        if (tokens.empty()) {
            response = "Invalid command, you have entered an empty message\n";
        } else {
            int disabledTTL=-1;
            string command = tokens[0];
            if (command == "SET" && tokens.size() >= 3) {
                string value = tokens[2];
                for (size_t i = 3; i < tokens.size() - 1; i++) {
                    value += " " + tokens[i];
                }
                int ttl = (tokens.size() > 3) ? std::stoi(tokens.back()) : disabledTTL;
                response=kv_store.create_key(tokens[1], value, ttl);
                response += "\n";
            } else if (command == "GET" && tokens.size() == 2) {
                response = kv_store.read_key(tokens[1]) + "\n";
            } else if (command == "UPDATE" && tokens.size() == 3) {
                string value = tokens[2];
                for (size_t i = 3; i < tokens.size() - 1; i++) {
                    value += " " + tokens[i];
                }
                response = kv_store.update_key(tokens[1], value);
                response += "\n";
            } else if (command == "DEL" && tokens.size() == 2) {
                response = kv_store.delete_key(tokens[1]);
                response += "\n";
            } else if (command == "CREATE_CHANNEL" && tokens.size() == 2) {
                response = pub_sub.create_channel(tokens[1],clientSocket);
                response += "\n";
            } else if (command == "SUBSCRIBE" && tokens.size() == 2) {
                response = pub_sub.subscribe(tokens[1], clientSocket);
                response += "\n";
            } else if (command == "UNSUBSCRIBE" && tokens.size() == 2) {
                response = pub_sub.unsubscribe(tokens[1], clientSocket);
                response += "\n";
            } else if (command == "PUBLISH" && tokens.size() >= 3) {
                string value = tokens[2];
                for (size_t i = 3; i < tokens.size(); i++) {
                    value += " " + tokens[i];
                }
                response = pub_sub.publish(tokens[1], value,clientSocket);
                response += "\n";
            } else if (command == "ADD_PUBLISHER" && tokens.size() == 2) {
                response = pub_sub.add_publishers(tokens[1],clientSocket);
                response += "\n";
            } else {
                response = "Invalid command\n";
            }
        }

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    auto it = find(clients.begin(), clients.end(), clientSocket);
    if (it != clients.end()) {
        clients.erase(it);
    }
    closesocket(clientSocket);
}

void StartServer(int port) {
    if (!Initialize()) {
        printf("WinSock initialization failed\n");
        return;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (InetPton(AF_INET, _T("0.0.0.0"), &serverAddress.sin_addr.s_addr) != 1) {
        printf("InetPton failed\n");
        closesocket(listenSocket);
        return;
    }

    if (bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    cout << "Server has started listening on port: " << port << endl;
    vector<SOCKET> clients;

    while (true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET) {
            printf("Accept failed\n");
            continue;
        }

        clients.push_back(clientSocket);
        thread t1(InteractWithClient, clientSocket, ref(clients));
        t1.detach();
    }

    closesocket(listenSocket);
    WSACleanup();
}
