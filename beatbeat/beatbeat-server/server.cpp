#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <chrono>
#include <random>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 1337
#define HEARTBEAT_INTERVAL 3
#define RECEIVE_TIMEOUT 3 

void handleclient(SOCKET clientSocket)
{
    char buffer[1024] = { 0 };
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen);
    cout << "[ beatbeat ] client connected\n";

    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(1, 100);

    while (true)
    {
        int randomNum = distr(eng);
        string msg = to_string(randomNum) + "\n";
        send(clientSocket, msg.c_str(), msg.size(), 0);
        cout << "[ beatbeat ] heartbeat sent -> " << randomNum << endl;

        DWORD timeout = RECEIVE_TIMEOUT * 1000;
        setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

        int valRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (valRead > 0)
        {
            cout << "[ beatbeat ] heartbeat received -> " << buffer;
        }
        else
        {
            cout << "[ beatbeat ] failed to receive heartbeat\n";
            break; 
        }

        this_thread::sleep_for(chrono::seconds(HEARTBEAT_INTERVAL));
    }

    closesocket(clientSocket);
}

int main()
{
    WSADATA wsadata;
    SOCKET serversocket;
    SOCKET   clientsocket;
    sockaddr_in server;
    sockaddr_in client;
    int addrLen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        cout << "[ beatbeat ] winsocket failed\n";
        Sleep(2000);
        return 1;
    }

    if ((serversocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        cout << "[ beatbeat ] socket creation failed\n";
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    if (bind(serversocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        cout << "[ beatbeat ] bind failed\n";
        closesocket(serversocket);
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    if (listen(serversocket, 3) == SOCKET_ERROR)
    {
        cout << "[ beatbeat ] listen failed\n";
        closesocket(serversocket);
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    cout << "[ beatbeat ] server listening -> " << PORT << endl;

    while ((clientsocket = accept(serversocket, (struct sockaddr*)&client, &addrLen)) != INVALID_SOCKET)
    {
        thread(handleclient, clientsocket).detach();
    }

    closesocket(serversocket);
    WSACleanup();
    return 0;
}
