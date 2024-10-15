#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <chrono>
#include <random>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 1337

void handleserver(SOCKET clientsocket)
{
    char severbuffer[1024] = { 0 };
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(1, 10000000);

    cout << "[ beatbeat client ] connected to server!\n";
    while (true)
    {
        int heartbeatserver = recv(clientsocket, severbuffer, sizeof(severbuffer), 0);

        if (heartbeatserver > 0)
        {
            cout << "[ beatbeat client ] heartbeat received!\n";

            int randomheartbeat = distr(eng);
            string heartbeatclient = to_string(randomheartbeat) + "\n";

            send(clientsocket, heartbeatclient.c_str(), heartbeatclient.size(), 0);
            cout << "[ beatbeat client ] heartbeat sent!\n";
        }
        else
        {
            cout << "[ beatbeat client ] server disconnected\n";
            break;
        }

        this_thread::sleep_for(chrono::seconds(1));
    }

    closesocket(clientsocket);
}

int main()
{
    WSADATA wsadata;
    SOCKET clientsocket;
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        cout << "[ beatbeat client ] winsocket failed\n";
        Sleep(2000);
        return 1;
    }

    if ((clientsocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        cout << "[ beatbeat client ] socket creation failed\n";
        WSACleanup();
        return 1;
    }

    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0)
    {
        cout << "[ beatbeat client ] invalid server\n";
        closesocket(clientsocket);
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    if (connect(clientsocket, (sockaddr*)&server, sizeof(server)) < 0)
    {
        cout << "[ beatbeat client ] connection failed\n";
        closesocket(clientsocket);
        WSACleanup();
        Sleep(2000);
        return 1;
    }

    handleserver(clientsocket);

    return 0;
}
