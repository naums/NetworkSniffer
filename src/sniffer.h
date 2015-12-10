#ifndef NAUMS_PCAP_TRACER_SNIFFER
#define NAUMS_PCAP_TRACER_SNIFFER

#include "base/thread/thread.h"
#include "pcap/pcap.h"
#include "pcap/trace.h"
#include "config.h"
#include "mysql/store.h"

/**
 * \brief class for having a sniffer-thread (you gonna have to have a sniffer-object for __every__ device)
 **/
class sniffer : public base::thread
              , public network::pcap::parser
{
    private:
        /// the name of the device to be sniffed at (eg. eth0)
        const char* dev_name;
        /// the object (pcap::device) for that specific object
        network::pcap::device* dev;
        /// the configuration information for the program (and therefor the kafka-producer)
        config* cfg;
        /// wrapper object to use the mysql-connection
        dbstore* db;
        
        /// the ip-address of the device
        char ip[4];
                
    public:
    
        /**
         * \brief creates the sniffer-object
         * \param[in] cfg the configuration information
         * \param[in] device the name of the device to be sniffed at
         **/
        sniffer ( config* cfg, const char* device );
        /**
         * \brief deletes the pcap::device-Object created for the device to be sniffed from
         **/
        virtual ~sniffer ();
        
        /**
         * \brief retrieves the applicationname from port
         * \param[in] port the port to be analysed
         * \param[out] application the application name
         * \param[in] maxsize maximal number of characters left in the buffer
         **/
        void getApplicationName ( unsigned short port, char* application, int maxsize );
                
        virtual void* run ();
        
        virtual void recv_msg ( const struct pcap_pkthdr *h, const u_char *packet );        
};


#endif

