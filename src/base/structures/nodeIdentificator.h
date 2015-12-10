#ifndef NAUMS_NODE_IDENT
#define NAUMS_NODE_IDENT

#include <stdlib.h>

struct _ipname
{
    /// IP is 4 Bytes long; no need to address 1 Byte
    unsigned int ip;
    /// node-number
    int nodenumber;
};

class NodeIdentificator
{
    private:
        int size;
        /// array of names for the nodes
        char** names;
        /// for assigning ip-adresses to a node
        struct _ipname* ip;
        /// points to the end of the ip-array
        int ip_end;
    
    public:
        NodeIdentificator( int numHost, int numDev );
        ~NodeIdentificator();
    
        inline int getSize() { return this->ip_end; };
    
        /**
         * \briefs adds a name to a node
         * \param[in] i  the number of the node
         * \param[in] name the name for the selected node
         * \return true if there was no name set yet, false if there was a name
         **/
        bool addName( int i, char* name );
        
        /**
         * \brief returns the name for a node
         * \param[in] i the node, you want the name for
         * \return the name of the node (or "\0" if none is set);
         **/
        const char* name ( int i );
        
        void debugNames ();
        
        unsigned int getIpByIndex ( int i );
        unsigned int getIpById ( int id );
        
        /**
         * \brief adds a device (ip <-> node) to the ip-list
         * \param[in] i node number
         * \param[in] ip ip-address of the node
         * \warn please add the devices in a ascending sorted manner!
         **/
        void addDevice ( int i, unsigned int ip );
        
        int getIdByIp ( unsigned int ip );
        struct _ipname* getIpName ( unsigned int ip, int current, int stepsize );
        
        struct _ipname* getAllIPs ( int* num=NULL )
        {
            if (num!=NULL)
                *num = this->ip_end;
            return this->ip;
        }
};

#endif
