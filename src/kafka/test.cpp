#include <iostream>
#include "kuser.h"

using namespace network::kafka;

int main ()
{
    kproducer* myprod = kproducer::create ("localhost:9092", "test1");
    myprod->send ("Dies ist ein Probetext\0");
    int i=0;
    const char* pi = (char*) &i;
    for (; i<10000; i++)
    {
        myprod->send (pi, sizeof(int));
    }
    
    delete myprod;
    return 0;
}
