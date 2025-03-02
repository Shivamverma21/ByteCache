#include <bits/stdc++.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>

using namespace std;

# pragma comment(lib, "Ws2_32.lib")

bool Initialize(){
  WSADATA wsaData;
  return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

void SendMessageToServer(SOCKET sock) {
    cout<< "Enter your chat name : " <<endl;
    string name;
    getline(cin, name);
    cout<< "Hey " << name<< " !" << endl<< "Welcome to ByteCache, Please interact with server and rate you experience" << endl;
    cout<< "Protocal format : " << "COMMAND {ALL CAPS} Key Value TTL(Optional)"<< endl;
    string message;

    while (true) {
        getline(cin, message);
        int bytesent = send(sock, message.c_str(), message.size(), 0);
        if (bytesent == SOCKET_ERROR) {
            cout << "Error sending message" << endl;
            break;
        }
        if(message == "quit") {
            cout<< "Stopping the chat" << endl;
            break;
        }
    }
    closesocket(sock);
    WSACleanup();
}
void ReceiveMessageFromServer(SOCKET sock) {
    char buffer[4096];
    int bytes_received;
    while (true) {
        bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received == SOCKET_ERROR) {
            cout << "Error receiving message" << endl;
            break;
        }
        string receivedmessage(buffer,bytes_received);
        cout << receivedmessage << endl;
    }
    closesocket(sock);
    WSACleanup();
}
 int main(){
   if(!Initialize()){
     printf("WinSock initialization failed\n");
   }

  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }
    int port = 12345;
    string serveraddress = "127.0.0.1";
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, serveraddress.c_str(), &addr.sin_addr);

    if(connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Connect failed (%d).\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "successfully connected to server" << endl;

    thread senderthread(SendMessageToServer,sock);
    thread receiverthread(ReceiveMessageFromServer,sock);

    senderthread.join();
    receiverthread.join();

    closesocket(sock);
    WSACleanup();

   return 0;
  }