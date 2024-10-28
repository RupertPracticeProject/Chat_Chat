#define UNICODE
#include "super_win.h"
#include <string>
#include <iostream>
#include <string>


using namespace std;

int main()
{
    super_win sw;
    sw.add_button(
        string("connect"),
        L"CONNECT"
    );
    
    sw.add_board(
        string("ip_board"),
        L"IP",
        sw.width*3/20, 
        sw.height*3/5,
        sw.width/20, 
        sw.height/25
    );
    sw.add_input(
        string("ip"),
        L"",
        sw.width*1/5, 
        sw.height*3/5, 
        sw.width*3/5, 
        sw.height/25
    );
    sw.start();
    
    
    return 0;
}