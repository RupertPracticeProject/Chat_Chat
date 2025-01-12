#define UNICODE
#include "socketConfigure.hpp"
#include "processCharArray.hpp"
#include <Windows.h>
#include <iostream>
#include <thread>
using namespace std;

SOCKET soc;

char messageBoard[SocketConfigure::messageBufferSize];

void ConnectionServer(SOCKET * conSoc, int socIndex)
{
    
    conSoc[socIndex] = accept(soc, 0, 0);
    if(conSoc[socIndex] == INVALID_SOCKET)
    {
        cout << "Accept error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();        
        return;
    }
    char buffer[SocketConfigure::messageBufferSize];
    recv(conSoc[socIndex], buffer, SocketConfigure::messageBufferSize, 0);

    while(true)
    {
        recv(conSoc[socIndex], buffer, SocketConfigure::messageBufferSize, 0);
        cout << "client<" << socIndex << "> : " << buffer << endl;
        if(IsEndMessage(buffer))
        {
            closesocket(conSoc[socIndex]);
            break;
        }
        else if(ParseFileSendingCommand(buffer))
        {
            closesocket(conSoc[socIndex]);
            break;
        }
        else
        {
            int bufferLength = string(buffer).length();
            int messageBoardLength = string(messageBoard).length();
            messageBoard[messageBoardLength] = '\r';
            messageBoard[messageBoardLength + 1] = '\n';
            for(int i = messageBoardLength + 2, j = 0; j < bufferLength; ++i, ++j)
            {

                messageBoard[i] = buffer[j];
            }
            for(int i = 0; i < SocketConfigure::connectionLimit; ++i)
            {
                send(conSoc[i], messageBoard, SocketConfigure::messageBufferSize, 0);
            }
        }
    }
}



int main()
{
    for(int i = 0; i < SocketConfigure::messageBufferSize; ++i)
    {
        messageBoard[i] = '\0';
    }
    WSADATA data;
    ZeroMemory(&data, sizeof(data));
    auto version = MAKEWORD(2,2);

    if(WSAStartup(version, &data)!=0)
    {
        cout << "WSA Start up error, code : " << WSAGetLastError() << endl;
        WSACleanup();
    }
    else
    {
        cout << "WSA Start up success" << endl;
    }
    
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == INVALID_SOCKET)
    {
        cout << "Socket Start up error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();
    }
    else
    {
        cout << "Socket Start up success" << endl;
    }
    sockaddr_in bindAddr;
    bindAddr.sin_port = htons(SocketConfigure::port);
    bindAddr.sin_addr.s_addr = inet_addr(SocketConfigure::getAddr().data());
    bindAddr.sin_family = AF_INET;
    
    if(bind(soc, reinterpret_cast<SOCKADDR*>(&bindAddr), sizeof(bindAddr)) == SOCKET_ERROR)
    {
        cout << "Bind error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();
    }
    else
    {
        cout << "Bind success" << endl;
    }

    if(listen(soc, SocketConfigure::connectionLimit) == SOCKET_ERROR)
    {
        cout << "Listening error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();
    }
    else
    {
        cout << "Listening..." << endl;
    }
    
    thread server[SocketConfigure::connectionLimit];
    SOCKET conSoc[SocketConfigure::connectionLimit];
    for(int i = 0; i < SocketConfigure::connectionLimit; ++i)
    {
        server[i] = thread(ConnectionServer, conSoc, i);
    }

    for(int i = 0; i < SocketConfigure::connectionLimit; ++i)
    {        
        server[i].join();
    }

    return 0;
}