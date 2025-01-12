#ifndef SOCKET_CONFIGURE
#define SOCKET_CONFIGURE
#include <winsock2.h>
#include <string>
using namespace std;
class SocketConfigure
{
    public:
        static const int messageBufferSize = 1000;
        static const int port = 5555;
        static const int ipLength = 30;
        static string getAddr()
        {
            return "127.0.0.1";
        }
        static const int connectionLimit = 5;
};
#endif