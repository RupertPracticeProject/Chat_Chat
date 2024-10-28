#ifndef SUPER_WIN
#define SUPER_WIN
#define UNICODE
#include <string>
using namespace std;
#include <iostream>
#include "socket_configure.h"
#include <thread>
#include <Windows.h>
#include <map>


SOCKET soc;
bool starting_server_listen = false;

class super_input
{
    public:        
        HWND handle;
        super_input(wchar_t* text, HWND base_win, int pos_x, int pos_y, int width, int height)
        {
            this -> handle = CreateWindow(
                L"Edit",
                text,
                ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE | ES_MULTILINE,
                pos_x,
                pos_y,
                width,
                height,
                base_win,
                NULL,
                NULL,
                NULL
            );                         
        }
};

class super_board
{
    public:        
        HWND handle;
        super_board(wchar_t* text, HWND base_win, int pos_x, int pos_y, int width, int height)
        {
            this -> handle = CreateWindow(
                L"Edit",
                text,
                ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
                pos_x,
                pos_y,
                width,
                height,
                base_win,
                NULL,
                NULL,
                NULL
            );                         
        }
};

class super_button
{
    public:        
        HWND handle;
        super_button(wchar_t* text, HWND base_win, int pos_x, int pos_y, int width, int height)
        {
            this -> handle = CreateWindow(
                L"Button",
                text,
                BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
                pos_x,
                pos_y,
                width,
                height,
                base_win,
                NULL,
                NULL,
                NULL
            );                         
        }
};
class super_win;
map<HWND, super_win*> handle_win_dict;
map<HWND, super_button*> handle_button_dict;
map<HWND, super_input*> handle_input_dict;
map<HWND, super_board*> handle_board_dict;
map<string, HWND> name_handle_dict;

void receive_from_server_and_write_to_board()
{
    while(!starting_server_listen);
    char buffer[socket_configure::message_buffer_size];
    while(true)
    {
        recv(soc, buffer, socket_configure::message_buffer_size, 0);
        wchar_t wc_board[socket_configure::message_buffer_size];
        size_t s;
        mbstowcs_s(&s, wc_board, socket_configure::message_buffer_size, buffer, socket_configure::message_buffer_size);
        SetWindowTextW(name_handle_dict[string("message_board")], wc_board);
    }
}

class super_win
{
    public:
        static const int pos_x = CW_USEDEFAULT;
        static const int pos_y = CW_USEDEFAULT;
        static const int width = 500;
        static const int height = 500;
        HWND handle;
        

        static LRESULT CALLBACK wnd_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
        {
            switch(message)
            {
                case WM_DESTROY:
                    PostQuitMessage(0);
                    send(soc, "end", socket_configure::message_buffer_size, 0);
                    return 0;
                case WM_COMMAND:
                    switch(HIWORD(wparam))
                    {
                        case BN_CLICKED:
                            if(name_handle_dict[string("send")] == (HWND)lparam)
                            {
                                
                                int length = GetWindowTextLengthW(name_handle_dict[string("message")]);
                                wchar_t wc_message[socket_configure::message_buffer_size];
                                GetWindowTextW(name_handle_dict[string("message")], wc_message, length + 1);
                                SetWindowTextW(name_handle_dict[string("message")], L"");
                                char message[socket_configure::message_buffer_size];
                                size_t s;
                                wcstombs_s(&s, message, socket_configure::message_buffer_size, wc_message, socket_configure::message_buffer_size);
                                send(soc, message, socket_configure::message_buffer_size, 0);
                            }
                            else if(name_handle_dict[string("connect")] == (HWND)lparam)
                            {
                                wchar_t wchip[socket_configure::ip_length];
                                int ip_length = GetWindowTextLengthW(name_handle_dict[string("ip")]);
                                if(ip_length == 0)
                                {
                                    cout << "IP empty" << endl;
                                    return 0;
                                }   
                                GetWindowTextW(name_handle_dict[string("ip")], wchip, ip_length + 1);
                                
                                char ip[socket_configure::ip_length];
                                size_t s;
                                wcstombs_s(&s, ip, socket_configure::ip_length, wchip, socket_configure::ip_length);
                                WSADATA data;
                                ZeroMemory(&data, sizeof(data));
                                auto version = MAKEWORD(2,2);

                                if(WSAStartup(version, &data)!=0)
                                {
                                    cout << "WSA start up error, code : " << WSAGetLastError() << endl;
                                    WSACleanup();
                                    return 0;
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
                                    return 0;
                                }
                                else
                                {
                                    cout << "Socket start up success" << endl;
                                }
                                sockaddr_in bind_addr;
                                bind_addr.sin_port = htons(socket_configure::port);
                                bind_addr.sin_addr.s_addr = inet_addr(string(ip).data());
                                bind_addr.sin_family = AF_INET;
                                
                                if(connect(soc, reinterpret_cast<SOCKADDR*>(&bind_addr), sizeof(bind_addr)) == SOCKET_ERROR)
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

                                char buffer[socket_configure::message_buffer_size] = "Start chatting ...\r\n";
                                
                                send(soc, buffer, socket_configure::message_buffer_size, 0);
                                SetWindowTextW(handle, wchip);
                                DestroyWindow(name_handle_dict[string("connect")]);
                                DestroyWindow(name_handle_dict[string("ip")]);
                                DestroyWindow(name_handle_dict[string("ip_board")]);
                                handle_win_dict[name_handle_dict["main_frame"]] -> add_board(
                                    string("message_board"),
                                    L""
                                );
                                handle_win_dict[name_handle_dict["main_frame"]] -> add_input(
                                    string("message"),
                                    L""
                                );
                                handle_win_dict[name_handle_dict["main_frame"]] -> add_button(
                                    string("send"),
                                    L"SEND"
                                );
                                starting_server_listen = true;
                            }
                            break;
                    }
                    break;

            }
            return DefWindowProc(handle, message, wparam, lparam);
        }
        
        super_win(wchar_t* text = L"Chat chat >W< ")
        {
            WNDCLASS wnd_class;
            ZeroMemory(&wnd_class, sizeof(wnd_class));
            wnd_class.lpszClassName = L"main_frame";
            wnd_class.style = CS_HREDRAW | CS_VREDRAW;
            wnd_class.lpfnWndProc = wnd_proc;
            wnd_class.hbrBackground = (HBRUSH)3;
            RegisterClass(&wnd_class);
            
            this -> handle = CreateWindow(
                L"main_frame",
                text,
                WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU,
                pos_x,
                pos_y,
                width,
                height,
                NULL,
                NULL,
                NULL,
                NULL
            );
            handle_win_dict[this -> handle] = this;
            name_handle_dict["main_frame"] = this -> handle;
        }

        void start()
        {
            thread tsk(receive_from_server_and_write_to_board);
            for(
                MSG msg;
                GetMessage(&msg, NULL, 0, 0) > 0;
                TranslateMessage(&msg),
                DispatchMessage(&msg)
            );
            tsk.detach();
        }
        ~super_win()
        {
            for(auto p:handle_button_dict)
            {
                free(p.second);
            }
            for(auto p:handle_input_dict)
            {
                free(p.second);
            }
            for(auto p:handle_board_dict)
            {
                free(p.second);
            }
        }
        
        void add_board(
            string name,
            wchar_t* text = L"",
            int x = width*1/5, 
            int y = height/5, 
            int w = width*3/5, 
            int h = height*2/5
        )
        {
            super_board * sb;
            sb = (super_board*)malloc(sizeof(super_board));
            *sb = super_board(text, this -> handle, x, y, w, h);
            handle_board_dict[sb -> handle] = sb;
            name_handle_dict[name] = sb -> handle;
        }
        void add_input(
            string name,
            wchar_t* text = L"",
            int x = width*1/5, 
            int y = height*3/5, 
            int w = width*3/5, 
            int h = height*9/50
        )
        {
            super_input * sb;
            sb = (super_input*)malloc(sizeof(super_input));
            *sb = super_input(text, this -> handle, x, y, w, h);
            handle_input_dict[sb -> handle] = sb;
            name_handle_dict[name] = sb -> handle;
            SetFocus(sb -> handle);
        }
        void add_button(
            string name,
            wchar_t* text = L"button",
            int x = width*2/5, 
            int y = height*4/5, 
            int w = width/5, 
            int h = height/15
        )
        {
            super_button * sb;
            sb = (super_button*)malloc(sizeof(super_button));
            *sb = super_button(text, this -> handle, x, y, w, h);
            handle_button_dict[sb -> handle] = sb;
            name_handle_dict[name] = sb -> handle;
        }
};

#endif