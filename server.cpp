#define UNICODE
#include <winsock2.h>
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
        cout << "WSA start up success";
    }
    
    
    return 0;
}