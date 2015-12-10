#include "pcap.h"

#include <cstring>

using namespace network::pcap;

int main ()
{
    char* buffer = start_info();
    
    printf ("%s\n", buffer);
    
    int i = strlen ( buffer );
    i++;
    int* ptr_b = (int*) &buffer[i];
    int num = *ptr_b;
    i+=4;
    int j;
    
    for (int z = 0; z<num; z++)
    {
        j = i + strlen ( &buffer[i] ) + 1;
        printf (" - %s -- %hhu.%hhu.%hhu.%hhu\n", &buffer[i], buffer[j], buffer[j+1], buffer[j+2], buffer[j+3]);
        i=j+4;
    }
}
