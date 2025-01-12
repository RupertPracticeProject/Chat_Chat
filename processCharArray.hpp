#ifndef PROCESS_CHAR_ARRAY
#define PROCESS_CHAR_ARRAY
#include <string>
using namespace std;
bool IsEndMessage(char * buffer)
{
    return buffer[0] == 'e' && buffer[1] == 'n' && buffer[2] == 'd' && buffer[3] == '\0';
}
bool ParseFileSendingCommand(char * buffer)
{
    return buffer[0] == 'f' && buffer[1] == 'i' && buffer[2] == 'l' && buffer[3] == 'e';
}

#endif