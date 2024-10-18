#ifndef SUPER_WIN
#define SUPER_WIN
#define UNICODE
#include <Windows.h>
#include <map>
using namespace std;

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

map<HWND, super_button*> handle_button_dict;
map<HWND, super_input*> handle_input_dict;
map<HWND, super_board*> handle_board_dict;
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
                    return 0;
                case WM_COMMAND:
                    switch(HIWORD(wparam))
                    {
                        case BN_CLICKED:
                            
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
        }

        void start()
        {
            for(
                MSG msg;
                GetMessage(&msg, NULL, 0, 0) > 0;
                TranslateMessage(&msg),
                DispatchMessage(&msg)
            );
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
            wchar_t* text = L"",
            int x = width*1/5, 
            int y = height*2/5, 
            int w = width*3/5, 
            int h = height*9/50
        )
        {
            super_board * sb;
            sb = (super_board*)malloc(sizeof(super_board));
            *sb = super_board(text, this -> handle, x, y, w, h);
            handle_board_dict[sb -> handle] = sb;
        }
        void add_input(
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
        }
        void add_button(
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
        }
};

#endif