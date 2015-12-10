#ifndef NAUMS_NW_STRUCTS
#define NAUMS_NW_STRUCTS

namespace base{
    struct edge2
    {
        int start, target, value;
    };
}

struct application
{
    char name[11];
    short unsigned int port;
    unsigned int hostId;
    unsigned int id;
    unsigned int ip;
};

struct edge
{
    unsigned int startIp; unsigned short int startPort;
    unsigned int targetIp; unsigned short int targetPort;
};

#endif
