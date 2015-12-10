#include "mysql.h"
#include <iostream>
#include <cstdio>

#include <netinet/ip.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../base/number/number.h"

using namespace base;

int main ()
{
    /**!!! Important! TODO
       vor der Verarbeitung Umwandlung der Integer-Daten in HOST-Sicht! **/
    mysql* db = mysql::connect ( "localhost", "root", "fg2sn", "test" );
    mysqlpp::StoreQueryResult res = db->query ( "SELECT * FROM trace" );
    for (size_t i = 0; i<res.num_rows(); i++)
    {
        unsigned int j = ntohl(res[i]["srcIp"]);
        unsigned int dport = ntohs ( res[i]["destport"] );
        unsigned int sport = ntohs ( res[i]["srcport"] );
        unsigned long int m = ( res[i]["srcmac"] );
        
        std::cout << ((j&0xff000000)>>24) << "." << ((j&0xff0000)>>16) << "." << ((j&0xff00)>>8) << "." << (j&0xff)  << " :: " << sport << " -> " << dport << std::endl;
        printf ("%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\n", 
                 (char) ((m&0xff0000000000)>>40), (char)((m&0xff00000000)>>32), (char)((m&0xff000000)>>24), 
                 (char)((m&0xff0000)>>16), (char)((m&0xff00)>>8), (char)(m&0xff));
    }
    
    delete db;
}

