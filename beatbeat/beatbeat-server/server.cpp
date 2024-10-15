#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <chrono>
#include <random>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 1337
#define SEND_INTERVAL 3
#define RECEIVE_INTERVAL 3 

void handleclient(SOCKET clientsocket)
{
    char clientbuffer[1024] = { 0 };
    sockaddr_in client;
    int addrlen = sizeof(client);
    getpeername(clientsocket, (struct sockaddr*)&client, &addrlen);
    cout << "[ beatbeat server ] client connected\n";

    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(1, 10000000);

    while (true)
    {
        int randomNum = distr(eng);
        string heartbeatserver = to_string(randomNum) + "\n";
        send(clientsocket, heartbeatserver.c_str(), heartbeatserver.size(), 0);
        cout << "[ beatbeat server ] heartbeat sent -> " << randomNum << endl;

        DWORD timeout = RECEIVE_INTERVAL * 1000;
        setsockopt(clientsocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

        int heartbeatclient = recv(clientsocket, clientbuffer, sizeof(clientbuffer), 0);
        if (heartbeatclient > 0)
        {
            cout << "[ beatbeat server ] heartbeat received -> " << clientbuffer;
        }
        else
        {
            cout << "[ beatbeat server ] failed to receive heartbeat\n";
            cout << "[ beatbeat server ] client disconnected\n";
            break; 
        }

        this_thread::sleep_for(chrono::seconds(SEND_INTERVAL));
    }

    closesocket(clientsocket);
}

int main()
{
    WSADATA wsadata;
    SOCKET serversocket;
    SOCKET clientsocket;
    sockaddr_in server;
    sockaddr_in client;
    int addrlen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        cout << "[ beatbeat server ] winsocket failed\n";
        Sleep(2000);
        return 1;
    }

    if ((serversocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        cout << "[ beatbeat server ] socket creation failed\n";
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    if (bind(serversocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        cout << "[ beatbeat server ] bind failed\n";
        closesocket(serversocket);
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    if (listen(serversocket, 3) == SOCKET_ERROR)
    {
        cout << "[ beatbeat server ] listen failed\n";
        closesocket(serversocket);
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    cout << "[ beatbeat server ] server listening -> " << PORT << endl;

    while ((clientsocket = accept(serversocket, (struct sockaddr*)&client, &addrlen)) != INVALID_SOCKET)
    {
        thread(handleclient, clientsocket).detach();
    }

    closesocket(serversocket);
    WSACleanup();
    return 0;
}
