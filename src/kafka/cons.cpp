#include <iostream>
#include "kuser.h"

using namespace network::kafka;

int main ()
{
    kconsumer* myprod = kconsumer::create ("localhost:9092", "test");
    
    for (int i=0; i<5; i++)
        myprod->recv();

    
    
    delete myprod;
    return 0;
}
