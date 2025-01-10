#ifndef PROCESS_CHAR_ARRAY
#define PROCESS_CHAR_ARRAY
#include <string>
using namespace std;
bool is_end_message(char * buffer)
{
    return buffer[0] == 'e' && buffer[1] == 'n' && buffer[2] == 'd' && buffer[3] == '\0';
}

#endif