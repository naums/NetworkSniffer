#ifndef NAUMS_PCAP_TRACE
#define NAUMS_PCAP_TRACE

#include <pcap.h>
#include <sys/time.h>

#include <netinet/ip.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace network {

    namespace pcap {
    
        /**
         * \brief parse a network-message captured by pcap_loop
         **/
        class parser 
        {
            public:
                /**
                 * \brief virtual method for parsing / interpreting the message; will be called by pcap_loop
                 * \param[in] h header-information from pcap (eg. length,...)
                 * \param[in] packet the payload of the message (including all headers)
                 * \sa device::start_sniffing()
                 **/
                virtual void recv_msg ( const struct pcap_pkthdr *h, const u_char *packet ) = 0;     
                
                static void _recv_msg_cb ( u_char* usr, const struct pcap_pkthdr *h, const u_char *packet );

        };
    
    
    }



    /**
     * message layout:
     *
     * trace:
     * 0         7      39       71       119       167       183        199     207      239         271         303                    until \0 ...       
     * ------------------------------------------------------------------------------------------------------------------------------------------
     * | version | srcIp | destIp | srcMac | destMac | srcPort | destPort | flags | length | timestamp | nanostamp | application_name ....'\0' |
     * ------------------------------------------------------------------------------------------------------------------------------------------
     *
     * trace_start:
     * 0    ... (\0)                       \0                            
     * -------------------------------------------------------------------
     * | hostname | { devicename (eg. eth0) | ipAdress | MAC-Adress \0 } | 
     * -------------------------------------------------------------------
    **/


    /**
     * \brief trace-dataformat for easier sending the trace-information to kafka
     **/
    struct trace
    {
        char version;                 ///< data-format version (for extention)
        unsigned int srcIp;           ///< source IP adress of the traced message
        unsigned int destIp;          ///< destination IP adress of the traced message
        unsigned char srcMac[6];      ///< MAC-Adress (source)      
        unsigned char destMac[6];     ///< MAC-Adress (destination)
        u_short srcPort;     ///< Sourceport
        u_short destPort;    ///< destination port
        /**
         * \brief flags representing binary information about the message
         *
         * represents binary information about the traced message
         * first bit: message sent by us or someone else?
         * second bit: UDP or TCP used?
         * other bits: unused
         **/
        char flags;          
        #define TRACE_SENT 0x01
        #define TRACE_TCP 0x02
        uint length;         ///< length of the message
        time_t timestamp;    ///< Unix timestamp, when the message was captured
        suseconds_t nanostamp; ///< microseconds and stuff
        // <memory for application name> (end with \0)        
    };
    
    struct trace_start
    {
        time_t timestamp;
    };
    
    void debug_trace (struct trace* tr, char* application);
}

#endif
