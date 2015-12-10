#include <iostream>
#include <cstdlib>
#include <cstring>

#include "../base/number/number.h"
#include "../base/debug/debug.h"
#include "pcap.h"

#include <netinet/ip.h>
#include <netinet/ether.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

namespace network { namespace pcap {

void list_devices ( bool descr )
{
    pcap_if_t * alldev;
    pcap_if_t * dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    pcap_findalldevs(&alldev, errbuf);
    if (alldev==NULL)
    {
        std::cout << "No devices found" << std::endl;
        return;
    }
    
    dev = alldev;
    for (; dev != NULL;)
    {
        std::cout << "   " << dev->name << std::endl;
        if (descr && dev->description != NULL)
            std::cout << "    >> " << dev->description << std::endl;
        
        pcap_addr_t* addr = dev->addresses;
        while (addr!=NULL)
        {
            if (addr->addr->sa_family == AF_INET)
            {
                printf ("Address: ");
                for (int i=2; i<6; i++)
                {
                    printf("%hhu.", addr->addr->sa_data[i]);
                } 
                printf("\n");
            }
            addr=addr->next;
        }
        
        //std::cout << std::endl;
        dev = dev->next;
    }
    //pcap_freealldevs(alldev);
}

char* start_info ()
{
    pcap_if_t * alldev;
    pcap_if_t * dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    pcap_findalldevs(&alldev, errbuf);
    if (alldev==NULL)
    {
        std::cout << "No devices found" << std::endl;
        return NULL;
    }
    
    dev=alldev;
    int num=0;
    while ( dev!=NULL )
    {   
        if ((dev->flags&PCAP_IF_LOOPBACK)==PCAP_IF_LOOPBACK)
        {
            dev=dev->next;
            continue;
        }
        pcap_addr_t* addr = dev->addresses;
        while (addr!=NULL)
        {
            if (addr->addr->sa_family == AF_INET)
            {
                num++;
            }
            
            addr=addr->next;
        }
        
        dev=dev->next;
    }
        
    /// see http://linux.die.net/man/2/gethostname
    char hostname[65];
    gethostname(hostname, 65);
    char* h = hostname;
    
    char* buffer = (char*) malloc ( strlen(hostname) + num*(16) );
    short int i=0;
    for (; *h; i++)
    {
        buffer[i] = *h;
        h++;
    }
    buffer[i]='\0';
    i++;
    
    int* ptr_b = (int*) &buffer[i];
    *ptr_b = num;
    i+=4;
    
    dev = alldev;
    for (; dev != NULL;)
    {                
        if ((dev->flags&PCAP_IF_LOOPBACK)==PCAP_IF_LOOPBACK)
        {
            dev=dev->next;
            continue;
        }
        pcap_addr_t* addr = dev->addresses;
        while (addr!=NULL)
        {
            if (addr->addr->sa_family == AF_INET)
            {
                strcpy ( &buffer[i], dev->name );
                i+=strlen(dev->name);
                buffer[i]='\0';
                i++;
            
                for (int x=0; x<4; x++)
                    buffer[i+x] = addr->addr->sa_data[2+x];
                i+=4;
                break;
            }
            addr=addr->next;
        }
        
        //std::cout << std::endl;
        dev = dev->next;
    }
    //pcap_freealldevs(alldev);
    
    return buffer;
}

device::device ( const char* dev )
{
    this->dev = dev;
}
device::~device ()
{
    if (this->handle!=NULL)
        this->close();
}

bool device::open()
{
    char errbuff[PCAP_ERRBUF_SIZE];
    this->handle = pcap_open_live ( this->dev, BUFSIZ, 1, 1000,
                                    errbuff);
    if (handle==NULL)
    {
       std::cout << "Could not open device " << dev << " because: " 
                 << errbuff << std::endl;
       return false;
    }
    if (pcap_datalink(handle) != DLT_EN10MB) {
        std::cout << "Device " << this->dev  
                  << " doesn't provide Ethernet headers - not supported" 
                  << std::endl;
        return false;
    }
    
    return true;
}

bool device::close()
{
    pcap_close(this->handle);
    this->handle = NULL;
    return true;
}

bool device::isOpened()
{
    return (this->handle!=NULL);
}

enum ip_state device::getIPAddress( int* ip )
{
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 maskp;
    
    if (pcap_lookupnet(this->dev, (bpf_u_int32*) &ip, &maskp, errbuf) == 0)
    {
        return IP_SUCCESS;
    }
    else
    {
        base::log ( "IP-Adresse konnte nicht bestimmt werden: %s", errbuf );
        return IP_OTHERWISE;
    }
}

bool device::setFilter ( const char* filter )
{
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 maskp;
    bpf_u_int32 ip;
    struct bpf_program fp;
    
    if (pcap_lookupnet(this->dev, &ip, &maskp, errbuf) == 0)
    {
        if (pcap_compile(this->handle, &fp, filter, 1, maskp) == 0)
        {
            if (pcap_setfilter(this->handle, &fp) == 0)
            {
                pcap_freecode(&fp);
                return true;
            }
            else
            {
                base::log ( "FILTER: filter could not be set: %s", pcap_geterr(this->handle));
                return false;
            }
        }
        else
        {
            base::log ( "FILTER: filter could not be compiled: %s", pcap_geterr(this->handle));
            return false;
        }
    }
    else
    {
        base::log ( "FILTER: IP-Address could not be determined: %s", errbuf );
        return false;
    }
}

bool device::start_sniffing( int count )
{
    // start sniffing
    pcap_loop(this->handle, count, &device::_recv_msg_cb, 
              reinterpret_cast<unsigned char*>(this));
    return true;
}

bool device::start_sniffing ( int count, parser* ps )
{
    pcap_loop(this->handle, count, &parser::_recv_msg_cb, 
              reinterpret_cast<unsigned char*>(ps));
    return true;
}

void device::_recv_msg_cb ( u_char* usr, 
                            const struct pcap_pkthdr *h, 
                            const u_char *packet )
{
     reinterpret_cast<device*>(usr)->recv_msg(h, packet);
}

void device::recv_msg ( const struct pcap_pkthdr *h, 
                        const u_char *packet )
{
    std::cout << "Size: "<<h->len << std::endl;
    struct ether_header* eptr = (struct ether_header*) packet;
    
    fprintf(stdout,"ethernet source: %s\n"
            ,ether_ntoa((const struct ether_addr *)&eptr->ether_shost));
    fprintf(stdout,"eth destination: %s\n"
            ,ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));   
            
    if(ntohs (eptr->ether_type) == ETHERTYPE_IP)
    {
        struct ip* ip_hdr = (struct ip*) (packet+ sizeof(struct ether_header));
        fprintf(stdout, "From: %s\n", inet_ntoa(ip_hdr->ip_src));
        fprintf(stdout, "To %s\n", inet_ntoa(ip_hdr->ip_dst));
        unsigned char ip_length = ip_hdr->ip_hl * 4;
        if (ip_hdr->ip_p == IP_TCP)
        {
            struct tcp* tcp_hdr = (struct tcp*) (packet + sizeof( struct ether_header) + ip_length);
            fprintf(stdout, "From Port: %hu\nTo Port %hu\n\n", tcp_hdr->th_sport, tcp_hdr->th_dport);
        }
        else if (ip_hdr->ip_p == IP_UDP)
        {
            struct udp* udp_hdr = (struct udp*) (packet + sizeof( struct ether_header) + ip_length);
            fprintf(stdout, "From Port: %hu\nTo Port %hu\n\n", udp_hdr->udp_sport, udp_hdr->udp_dport);
        }
        else
        {
            printf ("NOT UDP, NOT TCP -> not capturing\n");
        }
    }
   
}

}}

