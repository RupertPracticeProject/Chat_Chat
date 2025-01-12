#ifndef SUPER_WIN
#define SUPER_WIN
#define UNICODE
#include <string>
using namespace std;
#include <iostream>
#include "socketConfigure.hpp"
#include <thread>
#include <Windows.h>
#include <map>
#include "processCharArray.hpp"


SOCKET soc;
bool StartingServerListen = false;

class SuperInput
{
    public:        
        HWND handle;
        SuperInput(wchar_t* text, HWND baseWin, int posX, int posY, int width, int height)
        {
            this -> handle = CreateWindow(
                L"Edit",
                text,
                ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE | ES_MULTILINE,
                posX,
                posY,
                width,
                height,
                baseWin,
                NULL,
                NULL,
                NULL
            );                         
        }
};

class SuperBoard
{
    public:        
        HWND handle;
        SuperBoard(wchar_t* text, HWND baseWin, int posX, int posY, int width, int height)
        {
            this -> handle = CreateWindow(
                L"Edit",
                text,
                ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
                posX,
                posY,
                width,
                height,
                baseWin,
                NULL,
                NULL,
                NULL
            );                         
        }
};

class SuperButton
{
    public:        
        HWND handle;
        SuperButton(wchar_t* text, HWND baseWin, int posX, int posY, int width, int height)
        {
            this -> handle = CreateWindow(
                L"Button",
                text,
                BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
                posX,
                posY,
                width,
                height,
                baseWin,
                NULL,
                NULL,
                NULL
            );                         
        }
};
class SuperWin;
map<HWND, SuperWin*> handleWinDict;
map<HWND, SuperButton*> handleButtonDict;
map<HWND, SuperInput*> handleInputDict;
map<HWND, SuperBoard*> handleBoardDict;
map<string, HWND> nameHandleDict;

void ReceiveFromServerAndWriteToBoard()
{
    while(!StartingServerListen);
    char buffer[SocketConfigure::messageBufferSize];
    while(true)
    {
        recv(soc, buffer, SocketConfigure::messageBufferSize, 0);
        wchar_t wcBoard[SocketConfigure::messageBufferSize];
        size_t s;
        mbstowcs_s(&s, wcBoard, SocketConfigure::messageBufferSize, buffer, SocketConfigure::messageBufferSize);
        SetWindowTextW(nameHandleDict[string("messageBoard")], wcBoard);
    }
}

class SuperWin
{
    public:
        static const int posX = CW_USEDEFAULT;
        static const int posY = CW_USEDEFAULT;
        static const int width = 500;
        static const int height = 500;
        HWND handle;
        

        static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
        {
            switch(message)
            {
                case WM_DESTROY:
                    PostQuitMessage(0);
                    send(soc, "end", SocketConfigure::messageBufferSize, 0);
                    return 0;
                case WM_COMMAND:
                    switch(HIWORD(wparam))
                    {
                        case BN_CLICKED:
                            if(nameHandleDict[string("send")] == (HWND)lparam)
                            {
                                
                                int length = GetWindowTextLengthW(nameHandleDict[string("message")]);
                                wchar_t wcMessage[SocketConfigure::messageBufferSize];
                                GetWindowTextW(nameHandleDict[string("message")], wcMessage, length + 1);
                                SetWindowTextW(nameHandleDict[string("message")], L"");
                                char message[SocketConfigure::messageBufferSize];
                                size_t s;
                                wcstombs_s(&s, message, SocketConfigure::messageBufferSize, wcMessage, SocketConfigure::messageBufferSize);
                                
                                


                                send(soc, message, SocketConfigure::messageBufferSize, 0);
                                if(IsEndMessage(message))
                                {
                                    PostQuitMessage(0);
                                    return 0;
                                }
                                if(ParseFileSendingCommand(message))
                                {
                                    PostQuitMessage(0);
                                    return 0;
                                }
                            }
                            else if(nameHandleDict[string("connect")] == (HWND)lparam)
                            {
                                wchar_t wchip[SocketConfigure::ipLength];
                                int ipLength = GetWindowTextLengthW(nameHandleDict[string("ip")]);
                                if(ipLength == 0)
                                {
                                    cout << "IP empty" << endl;
                                    return 0;
                                }   
                                GetWindowTextW(nameHandleDict[string("ip")], wchip, ipLength + 1);
                                
                                char ip[SocketConfigure::ipLength];
                                size_t s;
                                wcstombs_s(&s, ip, SocketConfigure::ipLength, wchip, SocketConfigure::ipLength);
                                WSADATA data;
                                ZeroMemory(&data, sizeof(data));
                                auto version = MAKEWORD(2,2);

                                if(WSAStartup(version, &data)!=0)
                                {
                                    cout << "WSA Start up error, code : " << WSAGetLastError() << endl;
                                    WSACleanup();
                                    return 0;
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
                                    return 0;
                                }
                                else
                                {
                                    cout << "Socket Start up success" << endl;
                                }
                                sockaddr_in bindAddr;
                                bindAddr.sin_port = htons(SocketConfigure::port);
                                bindAddr.sin_addr.s_addr = inet_addr(string(ip).data());
                                bindAddr.sin_family = AF_INET;
                                
                                if(connect(soc, reinterpret_cast<SOCKADDR*>(&bindAddr), sizeof(bindAddr)) == SOCKET_ERROR)
                                {
                                    cout << "Connect error, code : " << WSAGetLastError() << endl;
                                    closesocket(soc);
                                    WSACleanup();
                                    return 0;
                                }
                                else
                                {
                                    cout << "Connect success" << endl;
                                }

                                char buffer[SocketConfigure::messageBufferSize] = "Start chatting ...\r\n";
                                
                                send(soc, buffer, SocketConfigure::messageBufferSize, 0);
                                SetWindowTextW(handle, wchip);
                                DestroyWindow(nameHandleDict[string("connect")]);
                                DestroyWindow(nameHandleDict[string("ip")]);
                                DestroyWindow(nameHandleDict[string("ip_board")]);
                                handleWinDict[nameHandleDict["main_frame"]] -> AddBoard(
                                    string("messageBoard"),
                                    L""
                                );
                                handleWinDict[nameHandleDict["main_frame"]] -> AddInput(
                                    string("message"),
                                    L""
                                );
                                handleWinDict[nameHandleDict["main_frame"]] -> AddButton(
                                    string("send"),
                                    L"SEND"
                                );
                                StartingServerListen = true;
                            }
                            break;
                    }
                    break;

            }
            return DefWindowProc(handle, message, wparam, lparam);
        }
        
        SuperWin(wchar_t* text = L"Chat chat >W< ")
        {
            WNDCLASS wndClass;
            ZeroMemory(&wndClass, sizeof(wndClass));
            wndClass.lpszClassName = L"main_frame";
            wndClass.style = CS_HREDRAW | CS_VREDRAW;
            wndClass.lpfnWndProc = WndProc;
            wndClass.hbrBackground = (HBRUSH)3;
            RegisterClass(&wndClass);
            
            this -> handle = CreateWindow(
                L"main_frame",
                text,
                WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU,
                posX,
                posY,
                width,
                height,
                NULL,
                NULL,
                NULL,
                NULL
            );
            handleWinDict[this -> handle] = this;
            nameHandleDict["main_frame"] = this -> handle;
        }

        void Start()
        {
            thread tsk(ReceiveFromServerAndWriteToBoard);
            for(
                MSG msg;
                GetMessage(&msg, NULL, 0, 0) > 0;
                TranslateMessage(&msg),
                DispatchMessage(&msg)
            );
            tsk.detach();
        }
        ~SuperWin()
        {
            for(auto p:handleButtonDict)
            {
                free(p.second);
            }
            for(auto p:handleInputDict)
            {
                free(p.second);
            }
            for(auto p:handleBoardDict)
            {
                free(p.second);
            }
        }
        
        void AddBoard(
            string name,
            wchar_t* text = L"",
            int x = width*1/5, 
            int y = height/5, 
            int w = width*3/5, 
            int h = height*2/5
        )
        {
            SuperBoard * sb;
            sb = (SuperBoard*)malloc(sizeof(SuperBoard));
            *sb = SuperBoard(text, this -> handle, x, y, w, h);
            handleBoardDict[sb -> handle] = sb;
            nameHandleDict[name] = sb -> handle;
        }
        void AddInput(
            string name,
            wchar_t* text = L"",
            int x = width*1/5, 
            int y = height*3/5, 
            int w = width*3/5, 
            int h = height*9/50
        )
        {
            SuperInput * sb;
            sb = (SuperInput*)malloc(sizeof(SuperInput));
            *sb = SuperInput(text, this -> handle, x, y, w, h);
            handleInputDict[sb -> handle] = sb;
            nameHandleDict[name] = sb -> handle;
            SetFocus(sb -> handle);
        }
        void AddButton(
            string name,
            wchar_t* text = L"button",
            int x = width*2/5, 
            int y = height*4/5, 
            int w = width/5, 
            int h = height/15
        )
        {
            SuperButton * sb;
            sb = (SuperButton*)malloc(sizeof(SuperButton));
            *sb = SuperButton(text, this -> handle, x, y, w, h);
            handleButtonDict[sb -> handle] = sb;
            nameHandleDict[name] = sb -> handle;
        }
};

#endif