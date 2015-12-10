#include "mysql.h"
        
#include <iostream>        
        
mysql::mysql ()
{
    conn = new mysqlpp::Connection ( false );
}

mysql::~mysql()
{
    delete conn;
}

mysql* mysql::connect ( const char* srv, const char* user, const char* passwd, const char* dbname )
{
    mysql* tmp = new mysql();
    if (tmp->conn->connect ( dbname, srv, user, passwd ))
    {
        return tmp;
    }
    else
    {
        delete tmp;
        return NULL;
    }
}

mysqlpp::StoreQueryResult mysql::query ( const char* q )
{
    mysqlpp::Query qu=conn->query( q );
    mysqlpp::StoreQueryResult res = qu.store();
    //std::cout << qu.error() << std::endl;
    return res;
}

mysqlpp::StoreQueryResult mysql::query  ( mysqlpp::Query q )
{
    mysqlpp::StoreQueryResult res = q.store();
    return res;
}

void mysql::put ( const char* q )
{
    mysqlpp::Query qu=conn->query( q );
    qu.store();
    //std::cout << qu.error() << std::endl;
}

mysqlpp::Connection* mysql::getConnection ()
{
    return conn;
}

void mysql::put ( mysqlpp::Query q )
{
    q.store();
}

const char* mysql::error ()
{
    return conn->error();
}

