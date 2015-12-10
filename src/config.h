#ifndef NAUMS_PCAP_TRACER_CONFIG
#define NAUMS_PCAP_TRACER_CONFIG

#include "base/thread/thread.h"
#include "kafka/kuser.h"

/**
 * \brief data-structure containing some config information for the program
 **/
class config
{
    public:        
        const char* mysql_server;
        const char* mysql_username;
        const char* mysql_passwd;
        const char* mysql_dbname;
        const char* mysql_trace_table;
        const char* mysql_init_table;
        const char* mysql_app_table;
        
        int hostId;
        int offset;
                
        /// is the program running, or should it be exited?
        bool running;
        
        config ( const char* srv, const char* usr, const char* passwd, 
                 const char* dbname, const char* trace, const char* init, 
                 const char* app );
        
        
        /// destructs the object
        virtual ~config();
        /**
         * \brief sets the program running (this->running=true)
         **/
        void setRunning();
        /**
         * \brief sets the program not running (this->running=false)
         **/
        void exit();
};

/**
 * \brief Thread for pulling new configuration data from kafka-server
 **/
class cfgThread : public base::thread
{
    private:
        /// configuration-information for the program (to be edited here!)
        config* cfg;
        /// the kconsumer-object (see kuser.h) for pulling new information from the kafka-server
        network::kafka::kconsumer* cons;
        
    public:
        /**
         * \brief creates the thread-object and initiates the kconsumer-object
         * \param[in] c the first configuration-object
         **/
        cfgThread( config* c ); 
        /**
         * \brief deletes the kconsumer-object and stops the thread
         **/
        virtual ~cfgThread();
        
        /**
         * \brief sets a new configuration-object
         * \param[in] c the new configuration-object
         **/
        void setCfg ( config* c );
        
        virtual void* run ();
};

#endif
