#ifndef NAUMS_THREAD
#define NAUMS_THREAD

#include <signal.h>

namespace base
{

    /**
     * \brief class that works as virtual class for running a thread
     **/
    class thread
    {
        private:
            /// thread id as given out by pthread
            pthread_t threadid;
            /// does the thread run?
            bool brun;
            
            /// does the inner constructor-work. 
            void construct();
            
            /// runs the thread-function
            static void* _run(void* param);
        
        public:
            /**
             * \brief constructs an empty thread.
             **/
            thread();
            /**
             * \brief deletes the thread
             **/
            virtual ~thread();
            
            /**
             * \brief starts the thread
             **/
            void start();
            /**
             * \brief stops / kills the thread
             **/
            void stop();
            /**
             * \brief waits for the thread to finish
             **/
            void* join();
            
            /**
             * \brief virtual method, that is the inner-function for the thread
             **/
            virtual void* run() = 0;
    };
}
#endif
