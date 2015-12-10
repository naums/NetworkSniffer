#include "store.h"
#include "../base/debug/debug.h"

#if defined(__linux__)
    #include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
    #include <sys/endian.h>
#elif defined(__OpenBSD__)
    #include <sys/types.h>
    #define be16toh(x) betoh16(x)
    #define be32toh(x) betoh32(x)
    #define be64toh(x) betoh64(x)
#endif

#include <cstdlib>

dbstore* dbstore::connect ( config* c )
{
    dbstore* tmp = new dbstore();
    tmp->cfg = c;
    tmp->db = mysql::connect ( c->mysql_server, c->mysql_username, 
                               c->mysql_passwd, c->mysql_dbname );
    if (tmp->db)
        return tmp;
    else
    {
        base::log ("No DB Connection, %s, %s, (pass), %s", c->mysql_server, c->mysql_username, 
                              c->mysql_dbname );
        return NULL;
    }
}

mysqlpp::StoreQueryResult dbstore::query ( const char* q )
{
    return this->db->query( q );
}

void dbstore::storeTrace ( network::trace* tr, const char* application )
{
    unsigned long int srcMac = (((long int) tr->srcMac[0]) << 40) + 
                      (((long int) tr->srcMac[1]) << 32) + 
                      (((long int) tr->srcMac[2]) << 24) + 
                      (((long int) tr->srcMac[3]) << 16) + 
                      (((long int) tr->srcMac[4]) <<  8) + 
                      (((long int) tr->srcMac[5]));
    unsigned long int destMac = (((long int) tr->destMac[0]) << 40) + 
                       (((long int) tr->destMac[1]) << 32) + 
                       (((long int) tr->destMac[2]) << 24) + 
                       (((long int) tr->destMac[3]) << 16) + 
                       (((long int) tr->destMac[4]) << 8) + 
                       (((long int) tr->destMac[5]));
                
    mysqlpp::Query q (db->getConnection());
    q << "INSERT INTO " << cfg->mysql_trace_table 
      << " (hostId, srcip, destip, srcport, destport, length, timestamp, microseconds, application)"
      << std::endl<< " VALUES ( "
      << this->cfg->hostId << "," << tr->srcIp << ","<< tr->destIp //<< "," << srcMac << ","<< destMac 
      << ","<< tr->srcPort << ","<< tr->destPort << ","<< tr->length  << "," 
      << tr->timestamp << "," << tr->nanostamp << ",'" << application << "')";
    db->put ( q );
    
    //std::cout << q.str() << std::endl;
}

void dbstore::storeInitial ( const char* buffer )
{
    int i = strlen ( buffer );
    const char* hostname = buffer;

    // inserting the hostname into the database (and registering the hostId)
    mysqlpp::Query q2 (db->getConnection());
    q2 << "INSERT INTO " << cfg->mysql_init_table << "_hostname (hostname) VALUES "
       << "( '" << hostname << "')"; 
    db->put ( q2 );
    
    mysqlpp::Query q (db->getConnection());
    q << "SELECT hostId FROM " << cfg->mysql_init_table<<"_hostname WHERE hostname='"<< hostname<<"'";
    mysqlpp::StoreQueryResult res = db->query ( q );
    if (res.num_rows()<1)
        return;
    
    this->cfg->hostId = res[0]["hostId"];
    
    printf ("Hostname: %s, %d:::\n", hostname, this->cfg->hostId);
    
    // calculating how many devices are saved
    i++;
    int* ptr_b = (int*) &buffer[i];
    int num = *ptr_b;
    i+=4;
    int j;
    
    // inserting the devices into the database-table
    for (int z = 0; z<num; z++)
    {
        j = i + strlen ( &buffer[i] ) + 1;
        unsigned int ip = *(unsigned int*) & buffer[j];
        ip = htonl ( ip );
        //unsigned int ip = ((unsigned int)buffer[j] << 24) + ((unsigned int)buffer[j+1] << 16) + ((unsigned int)buffer[j+2] << 8) + (unsigned int)buffer[j+3];
        printf (" - %s -- %u\n", &buffer[i], ip);
        
        mysqlpp::Query q3 (db->getConnection());
        q3 << "INSERT INTO " << cfg->mysql_init_table << "(hostID, devicename, ip) VALUES "
           << "( " << this->cfg->hostId << ",'" << &buffer[i] << "'," << ip << ")"; 
        db->put ( q3 );
        
        i=j+4;
    }
}

mysql* dbstore::getMySQL()
{
    return this->db;
}
