#define UNICODE
#include "socket_configure.h"
#include <Windows.h>
#include <iostream>
using namespace std;
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
    
    SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
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
    bind_addr.sin_port = htons(3333);
    bind_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind_addr.sin_family = AF_INET;
    
    if(bind(soc, reinterpret_cast<SOCKADDR*>(&bind_addr), sizeof(bind_addr)) == SOCKET_ERROR)
    {
        cout << "Bind error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();
    }
    else
    {
        cout << "Bind success" << endl;
    }

    

    return 0;
}