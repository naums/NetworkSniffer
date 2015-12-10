#include <signal.h>
#include <pthread.h>
#include "thread.h"

namespace base
{
    thread::thread ()
    {
        this->construct();
    }
    
    thread::~thread ()
    {
        if (brun)
            pthread_cancel ( this->threadid );
    }
    
    void thread::construct ()
    {
        threadid=0;
        brun=false;
    }
    
    void thread::stop ()
    {
        if (brun)
            pthread_cancel ( this->threadid );
        brun=false;
    }
    
    void* thread::join ()
    {
        void* buffer;
        pthread_join ( this->threadid, &buffer );
        return buffer;
    }
    
    void thread::start ()
    {
         pthread_create(&threadid, NULL, &thread::_run, reinterpret_cast<void*>(this));
    }
    
    void* thread::_run(void* param)
    {
        return reinterpret_cast<thread*>(param)->run();
    }
}
