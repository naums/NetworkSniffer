#ifndef NAUMS_NETWORK_PCAP
#define NAUMS_NETWORK_PCAP

#include "trace.h"
#include <pcap.h>

/*
 * TODO: data types!
 * short int -> 16 bit
 * int       -> 32 bit
 * long int  -> 64 bit
 */
 

namespace network { namespace pcap {

    // code from http://www.tcpdump.org/pcap.html (06.05.2015)
    #define IP_TCP 6
    #define IP_UDP 17
    
    typedef u_int tcp_seq;
    struct tcp {
	    u_short th_sport;	/* source port */
	    u_short th_dport;	/* destination port */
	    tcp_seq th_seq;		/* sequence number */
	    tcp_seq th_ack;		/* acknowledgement number */
	    u_char th_offx2;	/* data offset, rsvd */
    #define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	    u_char th_flags;
    #define TH_FIN 0x01
    #define TH_SYN 0x02
    #define TH_RST 0x04
    #define TH_PUSH 0x08
    #define TH_ACK 0x10
    #define TH_URG 0x20
    #define TH_ECE 0x40
    #define TH_CWR 0x80
    #define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	    u_short th_win;		/* window */
	    u_short th_sum;		/* checksum */
	    u_short th_urp;		/* urgent pointer */
    };
    
    struct udp
    {
        u_short udp_sport;
        u_short udp_dport;
        u_short udp_length;
        u_short udp_checksum;
    };

    /**
     * \brief describes the error-codes of the method "getIPAddress()"
     **/
    enum ip_state
    {
        IP_SUCCESS=0, ///< ip-address has successfully been returned
        IP_NOTFOUND,  ///< the device was not found in the list of devices
        IP_NONIP,     ///< the device does not have an IP-address (maybe not up?)
        IP_OTHERWISE  ///< an other error occurred
    };

    /**
     * \brief prints the found devices to stdout
     * \param[in] descr shall a description be printed?
     * \author Stefan Naumann
     * \date 30.04.2015
     **/
    void list_devices ( bool descr=false );
    
    /**
     * \brief returns a malloc'd buffer containing hostname, devicesnames and ip-adresses
     *
     * start_info() will collect data about the devices within the host and the hostname.
     * datalayout: [HOSTNAME] '\0' {[DEVICENAME] '\0' [IP.ADD.RE.SS]} {[DEVICENAME] '\0' [IP.ADD.RE.SS]} ... 
     * \return the malloc'd buffer
     **/
    char* start_info ();

    /**
     * \class device represents a sniffable device for pcap-sniffing
     **/
    class device
    {
        private:
            /// handle to the device to be sniffed by pcap
            pcap_t* handle;
            /// string of the device (eg. any, eth0, ...)
            const char* dev;
            
            /**
             * \brief internal callback function; called when a packet is sniffed by pacp
             * \param[in] usr userdata (see pcap_loop); here: this
             * \param[in] h header-information about the package sniffed
             * \param[in] packet the actual payload (with packet-header)
             **/
            static void _recv_msg_cb ( u_char* usr, const struct pcap_pkthdr *h, const u_char *packet );
            
        public:
            /**
             * \brief creates the device-object
             * \param[in] dev sets the device to sniff at
             **/
            device ( const char* dev );
            /**
             * \brief destructs the device-object and stops the capturing
             **/
            virtual ~device ();
            
            /**
             * \brief opens the device and checks, if it support ETHERNET
             **/
            bool open();
            /**
             * \brief closes the device and stops capturing
             **/
            bool close();
            
            /**
             * \brief returns true if the devices is already opened, false if not
             **/
            bool isOpened();
            
            /**
             * \brief gets the IP-Adress
             * \param[out] ip the ip-address of the currently active device
             * \return IP_SUCCESS if successful, otherwise see enum ip_state
             **/
            enum ip_state getIPAddress( int* ip );
            
            /**
             * \brief compiles and activates a filter for capturing of ip-Packets
             * \param[in] filter the filterstring to be applied to the captured devices
             * \return true if successful, false otherwise
             **/
            bool setFilter ( const char* filter );
            
            
            /**
             * \brief starts the sniffing process
             * \param[in] count the number of packets you want to sniff
             * \note blocking! if you want asynchronous sniffing, start this method in a thread!
             **/
            bool start_sniffing( int count=-1 );
            
            /**
             * \brief start the sniffing process and calls the method recv_msg from an external parser object
             * \param[in] count number of packets to capture
             * \param[in] ps the external parser object
             **/
            bool start_sniffing ( int count, parser* ps );
            
            
            /**
             * \brief virtual method for computing the packet-data
             * \param[in] h header-information about the package sniffed
             * \param[in] packet the actual payload (with packet-header)
             **/
            virtual void recv_msg ( const struct pcap_pkthdr *h, const u_char *packet );
    };
}}

#endif
