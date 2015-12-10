#include "thread.h"
#include <stdio.h>
#include <unistd.h>

class _thread : public base::thread
{
    public:
        int a;
        void* run ()
        {
            for (int i=0; i<100; i++)
                printf ("[%d] %d -> Thread\n ", this->a, i);
        }
};

int main ()
{
    _thread t1, t2;
    
    t1.a=1;
    t2.a=2;
    t1.start();
    t2.start();
    
    t1.join();
    
    t1.start();
    t2.join();
    sleep(1);
    t1.stop();
}
