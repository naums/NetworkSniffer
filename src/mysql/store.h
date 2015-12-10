#ifndef NAUMS_MYSQL_TRACE_STORE
#define NAUMS_MYSQL_TRACE_STORE

#include "mysql.h"
#include "../pcap/trace.h"
#include "../config.h"
#include "../base/structures/mgraph.h"

#define TRACE_QUERY_FORMAT "INSERT INTO %s (srcip, destip, srcmac, destmac, srcport, destport, sent, tcp, length, timestamp, nanoseconds) VALUES (%d, %d, %ld, %ld, %hu, %hu, %b, %b, %u, %d, %ld)\0"


/**
 * \brief wrapper to the mysql-object;
 **/
class dbstore
{
    private:
        /// mysql-database object
        mysql* db;
        /// config-object, saves the database-name and accountinfo and table-names
        config* cfg;
    
    public:
        /**
         * \brief connects to the database server and creates the internal mysql-datastructure
         * \param[in] c the config-object, which contains database-names and accountinfo and stuff
         * \return a valid object for using the MySQL-Database
         **/
        static dbstore* connect ( config* c );
        
        /**
         * \brief executes an arbitrary query and returns the results
         * \param[in] q the SQL-Query
         * \return results
         **/
        mysqlpp::StoreQueryResult query ( const char* q );
        
        /**
         * \brief stores the trace-object within the database 
         * \param[in] tr the trace-struct to be saved within the database
         **/
        void storeTrace ( network::trace* tr, const char* application );
        /**
         * \brief store the initial data into the database (ip-adresses & devicesnames & hostnames)
         * \param[in] buffer the buffer returned by network::pcap::start_info() 
         * \sa network::pcap::start_info() 
         **/
        void storeInitial ( const char* buffer );
        
        void storeRsrc ( /** TODO **/ );    
        
        mysql* getMySQL();
};

#endif
