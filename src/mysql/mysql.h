#ifndef NAUMS_MYSQL
#define NAUMS_MYSQL

#include <mysql++/mysql++.h>

/**
 * \brief wrapper to the MySQL++ Wrapper.
 **/
class mysql
{
    private:
        /// the mysql++ connection datastructure
        mysqlpp::Connection* conn;
        
        /// constructor, just creates the connection datastructure
        mysql ();
    public:
        /// destructor, deletes the connection datastructure
        ~mysql();
        
        /**
         * \brief creates a mysql-object and connects it to the database-server and database
         * \param[in] srv the servername of the MySQL-Database-Server
         * \param[in] user the username for the MySQL-Database
         * \param[in] passwd the passwd for the user-account on MySQL
         * \param[in] dbname name of the database
         * \return a valid object for using the MySQL-Database
         **/
        static mysql* connect ( const char* srv, const char* user, const char* passwd, const char* dbname );
        
        /**
         * \brief executes the SQL-Query and returns the results
         * \param[in] q the SQL-Query
         * \return results
         **/
        mysqlpp::StoreQueryResult query ( const char* q );
        /**
         * \brief executes the SQL-Query and returns the results
         * \param[in] q the MySQL-Query object
         * \return results
         **/
        mysqlpp::StoreQueryResult query ( mysqlpp::Query q );
        
        /**
         * \brief executes the SQL-Query
         * \param[in] q the SQL-Query
         **/
        void put ( const char* q );
        
        /**
         * \brief executes the SQL-Query and returns the results
         * \param[in] q the MySQL-Query object
         * \return results
         **/
        void put ( mysqlpp::Query q );
        
        /**
         * \brief returns the error-string of the connection-object(!)
         * \return errorstring of the connection-obj
         **/
        const char* error ();
        
        /**
         * \brief returns the mysqlpp-Connection object
         **/
        mysqlpp::Connection* getConnection ();
};

#endif
