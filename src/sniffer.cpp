#include "sniffer.h"
#include "base/thread/thread.h"
#include "pcap/pcap.h"
#include "kafka/kuser.h"
#include "config.h"

#include "base/debug/debug.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <netinet/ip.h>
#include <netinet/ether.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <arpa/inet.h>

// filter everything that is not TCP or UDP AND that does go to or from MySQL Server
#define PCAP_TEST_FILTER "((ip proto \\tcp and tcp[tcpflags] & (tcp-push) != 0) or ip proto \\udp) and not (src or dst host 10.10.10.141)\0"

using namespace network::kafka;

sniffer::sniffer ( config* c, const char* device )
{
    this->dev_name = device;
    dev = new network::pcap::device ( dev_name );
    
    dev->open();
    
    db = dbstore::connect ( c );
    if (!db)
        base::error("Connection to MySQL could not be established: %s", db->getMySQL()->error());
    
    cfg=c;
    //prod = kproducer::create ( cfg->kafka_server, cfg->kafka_trace_topic );
    if (dev->getIPAddress ( (int*) this->ip ) != network::pcap::IP_SUCCESS)
    {
        base::error("The device does not have an IP-Address!!!!");
    }
    else
    {
        base::log ("Got Device %s with IP %hhu.%hhu.%hhu.%hhu", 
                    this->dev_name, this->ip[0], this->ip[1], this->ip[2], this->ip[3]);
    }
}

sniffer::~sniffer ()
{
    delete dev;
    
    if (db)
        delete db;
    
    //if (prod)
    //    delete prod;
}

void* sniffer::run ()
{
    if (!dev->isOpened())
    {
        if (!dev->open())
        {
            base::error ( "Could not open device %s, Exiting the sniffing thread!", this->dev_name);
            return NULL;
        }
    }
    
    dev->setFilter ( PCAP_TEST_FILTER );
    dev->start_sniffing(-1, this);
    return NULL;
}

void sniffer::getApplicationName ( unsigned short port, char* application, int maxsize )
{
    char buffer[100];
    sprintf (buffer, "lsof -i :%hu | awk '{ print $1 }' | xargs | awk '{ print $2 }'", ntohs(port));
    FILE* p = popen ( buffer, "r" );
    
    if (!p)
        base::error ("Could not open pipe");
    
    char c; int index=0;
    while (feof(p) == 0)
    {
        c = fgetc ( p );
        if (isprint(c))
        {
            application[index] = c;
            index++;
            if (index >= maxsize)
                break;
        }
    }
    application[index]='\0';
    
    pclose(p);
}

void sniffer::recv_msg ( const struct pcap_pkthdr *h, 
                         const u_char *packet )
{
    char* buffer = (char*) malloc ( sizeof(struct network::trace) + 100 );
    char* application = buffer+sizeof(struct network::trace);
    unsigned int myip = *((unsigned int*) this->ip);
    bool sent=false;
    application[0]=0;
    struct network::trace* pkt = (struct network::trace*) buffer;
    /// version 1, deprecated because of using MySQL. 
    // intended for use with kafka, for having several "version" of this struct
    
    pkt->version=0x01; 
    pkt->length = h->len;
    pkt->timestamp = h->ts.tv_sec;
    pkt->nanostamp = h->ts.tv_usec;
    
    pkt->flags=0;
    
    struct ether_header* eptr = (struct ether_header*) packet;
    
    // Debug: output MAC_ADRESS
    //fprintf(stdout,"ethernet source: %s\n"
    //        ,ether_ntoa((const struct ether_addr *)&eptr->ether_shost));
    //fprintf(stdout,"eth destination: %s\n"
    //        ,ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));   
            
    // MAC-Adresses will not be saved
    memcpy ( &pkt->srcMac, &eptr->ether_shost, 6 );
    memcpy ( &pkt->destMac, &eptr->ether_dhost, 6 );
    
    //printf("%d -- %d\n", ETHERTYPE_IP, ntohs(eptr->ether_type));
    int offset = cfg->offset;
    
    short unsigned int type = *(short unsigned int*) (&eptr->ether_type + offset);
    
    bool vlan=false;
    if (ntohs(type) == 0x8100) // VIRTUAL LAN
    {
        vlan=true;
        offset+=4;        
    }
    
    /** Does only work when listening to //any// device.
        PCAP returns when listening to //any// an int on first 2 Bytes
        if its 4 it means, that the packet was sent by us **/
    /*short unsigned int linsent = *(short unsigned int*) (eptr);
    if (linsent == 4)
    {
        pkt->flags |= TRACE_SENT;
        sent = true;
    }*/
    

    
    if(ntohs (type) == ETHERTYPE_IP || vlan)
    {
        struct ip* ip_hdr = (struct ip*) (packet+ sizeof(struct ether_header) + offset);
        //fprintf(stdout, "From: %s\n", inet_ntoa(ip_hdr->ip_src));
        //fprintf(stdout, "To %s\n", inet_ntoa(ip_hdr->ip_dst));
        
        memcpy ( &pkt->srcIp, &ip_hdr->ip_src, 4 );
        memcpy ( &pkt->destIp, &ip_hdr->ip_dst, 4 );
                
        unsigned int srcIp = (unsigned int) ntohl( *(unsigned int*) &ip_hdr->ip_src);
        unsigned int destIp = (unsigned int) ntohl( *(unsigned int*) &ip_hdr->ip_dst);
        
        bool capture=true;
        if (srcIp == myip)
        {
            sent = true;
            pkt->flags |= TRACE_SENT;
        }
        else if (destIp == myip)
        {
            sent=false;
        }
        else
        {
            printf ("NOT SENT NOR RECV'd BY ME. NOT CAPTURING\n");
            capture=false;
        }
                
        unsigned char ip_length = ip_hdr->ip_hl * 4;
        if (ip_hdr->ip_p == IP_TCP && capture)
        {
            struct network::pcap::tcp* tcp_hdr = (struct network::pcap::tcp*) (packet + sizeof( struct ether_header ) + ip_length + offset);
            //fprintf(stdout, "From Port: %hu\nTo Port %hu\n\n", tcp_hdr->th_sport, tcp_hdr->th_dport);
            
            pkt->srcPort=(tcp_hdr->th_sport);
            pkt->destPort=(tcp_hdr->th_dport);
            
            pkt->flags |= TRACE_TCP;
            
        }
        /*else if (ip_hdr->ip_p == IP_UDP)
        {
            struct network::pcap::udp* udp_hdr = (struct network::pcap::udp*) (packet + sizeof( struct ether_header ) + ip_length + offset);
            //fprintf(stdout, "From Port: %hu\nTo Port %hu\n\n", udp_hdr->udp_sport, udp_hdr->udp_dport);
        
            pkt->srcPort=(udp_hdr->udp_sport);
            pkt->destPort=(udp_hdr->udp_dport);
        }*/
        else
        {
            printf ("NOT TCP -> not capturing\n");
        }
        
        if (sent)
            this->getApplicationName ( pkt->srcPort, application, 100);
        else
            this->getApplicationName ( pkt->destPort, application, 100);
        
        network::debug_trace ( pkt, application );
        //prod->send( buffer, sizeof(struct network::trace) + strlen(application));
        //db->storeTrace ( pkt, application );
    }
    else
    {
        printf ("Sorry, not capturing; NOT ETHERNET_IP\n");
    }
    
    static int i = 1;
    i++;
    std::cout << i << std::endl;
    free(buffer);
}


