#define UNICODE
#include "superWin.hpp"
#include <string>
#include <iostream>
#include <string>


using namespace std;

int main()
{
    SuperWin sw;
    sw.AddButton(
        string("connect"),
        L"CONNECT"
    );
    
    sw.AddBoard(
        string("ip_board"),
        L"IP",
        sw.width*3/20, 
        sw.height*3/5,
        sw.width/20, 
        sw.height/25
    );
    sw.AddInput(
        string("ip"),
        L"",
        sw.width*1/5, 
        sw.height*3/5, 
        sw.width*3/5, 
        sw.height/25
    );
    sw.Start();
    
    
    return 0;
}