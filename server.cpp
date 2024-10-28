#define UNICODE
#include "socket_configure.h"
#include <Windows.h>
#include <iostream>
#include <thread>
using namespace std;

SOCKET soc;

char message_board[socket_configure::message_buffer_size];

void connection_server(SOCKET * con_soc, int soc_index)
{
    
    con_soc[soc_index] = accept(soc, 0, 0);
    if(con_soc[soc_index] == INVALID_SOCKET)
    {
        cout << "Accept error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();        
        return;
    }
    char buffer[socket_configure::message_buffer_size];
    recv(con_soc[soc_index], buffer, socket_configure::message_buffer_size, 0);

    while(true)
    {
        recv(con_soc[soc_index], buffer, socket_configure::message_buffer_size, 0);
        cout << "client<" << soc_index << "> : " << buffer << endl;
        if(buffer[0] == 'e' && buffer[1] == 'n' && buffer[2] == 'd' && buffer[3] == '\0')
        {
            closesocket(con_soc[soc_index]);
            break;
        }
        int buffer_length = string(buffer).length();
        int message_board_length = string(message_board).length();
        message_board[message_board_length] = '\r';
        message_board[message_board_length + 1] = '\n';
        for(int i = message_board_length + 2, j = 0; j < buffer_length; ++i, ++j)
        {

            message_board[i] = buffer[j];
        }
        for(int i = 0; i < socket_configure::connection_limit; ++i)
        {
            send(con_soc[i], message_board, socket_configure::message_buffer_size, 0);
        }
    }
}



int main()
{
    for(int i = 0; i < socket_configure::message_buffer_size; ++i)
    {
        message_board[i] = '\0';
    }
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

    if(listen(soc, socket_configure::connection_limit) == SOCKET_ERROR)
    {
        cout << "Listening error, code : " << WSAGetLastError() << endl;
        closesocket(soc);
        WSACleanup();
    }
    else
    {
        cout << "Listening..." << endl;
    }
    
    thread server[socket_configure::connection_limit];
    SOCKET con_soc[socket_configure::connection_limit];
    for(int i = 0; i < socket_configure::connection_limit; ++i)
    {
        server[i] = thread(connection_server, con_soc, i);
    }

    for(int i = 0; i < socket_configure::connection_limit; ++i)
    {        
        server[i].join();
    }

    return 0;
}