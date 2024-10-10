#ifndef SOCKET_CONFIGURE
#define SOCKET_CONFIGURE
#include <winsock2.h>
#include <string>
using namespace std;
class socket_configure
{
    public:
        static const int port = 5555;
        static string get_addr()
        {
            return "127.0.0.1";
        }
        static const int connection_limit = 5;
};
#endif