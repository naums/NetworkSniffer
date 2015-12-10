#include "trace.h"

namespace network {

    namespace pcap {
        void parser::_recv_msg_cb ( u_char* usr, const struct pcap_pkthdr *h, const u_char *packet )
        {
             reinterpret_cast<parser*>(usr)->recv_msg(h, packet);
        }
    }

    void debug_trace ( struct trace* tr, char* application )
    {
        char* src = (char*) &tr->srcIp;
        char* dest = (char*) &tr->destIp;
        char* macs = (char*) &tr->srcMac;
        char* macd = (char*) &tr->destMac;
        printf("\n\
struct trace\n\
{\n\
    char version=%hhd\n\
    int srcIp=%hhu.%hhu.%hhu.%hhu\n\
    int destIp=%hhu.%hhu.%hhu.%hhu\n\
    char srcMac[6]=%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\n\
    char destMac[6]=%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\n\
    u_short srcPort=%hu\n\
    u_short destPort=%hu\n\
    char flags=%hhu\n\
    uint length=%u\n\
    time_t timestamp=%ld\n\
    suseconds_t nanostamp=%ld\n\
    application=%s\n\
};\n\n",
        tr->version,
        src[0], src[1], src[2], src[3],
        dest[0], dest[1], dest[2], dest[3],
        macs[0], macs[1], macs[2], macs[3], macs[4], macs[5],
        macd[0], macd[1], macd[2], macd[3], macd[4], macd[5],
        tr->srcPort,
        tr->destPort,
        tr->flags,
        tr->length,
        tr->timestamp,
        tr->nanostamp,
        application);
    }
    
}    
