#ifndef SERVER_H
#define SERVER_H

#include <bits/stdc++.h>

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>

using namespace std;

# pragma comment(lib, "Ws2_32.lib")

#undef byte

bool Initialize();
void InteractWithClient(SOCKET clientSocket, vector<SOCKET>& clients);
void StartServer(int port);

#endif // SERVER_H
