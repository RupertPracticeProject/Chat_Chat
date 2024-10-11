#define UNICODE
#include "socket_configure.h"
#include <Windows.h>
#include <iostream>
#include <thread>
using namespace std;

SOCKET soc;

int main()
{
    WSADATA data;
    ZeroMemory(&data, sizeof(data));
    auto version = MAKEWORD(2,2);

    if(WSAStartup(version, &data)!=0)
    {
        cout << "WSA start up error, code : " << WSAGetLastError() << endl;
        WSACleanup();
    }
    else
    {
        cout << "WSA start up success" << endl;
    }
    
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == INVALID_SOCKET)
    {
        cout << "Socket start up error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();
    }
    else
    {
        cout << "Socket start up success" << endl;
    }
    sockaddr_in bind_addr;
    bind_addr.sin_port = htons(socket_configure::port);
    bind_addr.sin_addr.s_addr = inet_addr(socket_configure::get_addr().data());
    bind_addr.sin_family = AF_INET;
    
    if(connect(soc, reinterpret_cast<SOCKADDR*>(&bind_addr), sizeof(bind_addr)) == SOCKET_ERROR)
    {
        cout << "Connect error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();
    }
    else
    {
        cout << "Connect success" << endl;
    }

    char buffer[socket_configure::message_buffer_size] = "Successfully send";

    send(soc, buffer, socket_configure::message_buffer_size, 0);

    return 0;
}