#ifndef NAUMS_BASE_MGRAPH
#define NAUMS_BASE_MGRAPH

#include <cstdio>
#include "pqueue.h"
#include "graph.h"
#include "list.h"
#include "nodeIdentificator.h"

namespace base
{
#if 0
/**
 * \brief datastructure for the edges in the mgraph-structure
 **/
struct edge
{
    /// destination port (of the server)
    short unsigned int port;
    /// average response time
    double avg;
    /// sum of the response times
    double sum;
    /// number of response times in sum
    int count;
};

struct generic_edge
{
    int start, end;
};

struct _ipname
{
    /// IP is 4 Bytes long; no need to address 1 Byte
    unsigned int ip;
    /// node-number
    int nodenumber;
};

/**
 * \brief graph-structure with adjacency matrix and multiple edges between two nodes
 **/
class mgraph{
    private:
        /// number of nodes
        int size;
        /// adjacency matrix (with 0, 1 or n edges between two nodes)
        pqueue<short unsigned int, struct edge*>** payload;
        /// array of names for the nodes
        char** names;
        /// for assigning ip-adresses to a node
        struct _ipname* ip;
        /// points to the end of the ip-array
        int ip_end;

    public:
        /// constructor, mallocs the payload-array and every pqueue
        mgraph( int num, int numdevices );        
        /// destructor, deletes every pqueue object (including their payload) and frees the payload-array
        ~mgraph ();
        
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
        
        /**
         * \brief adds a edge between i->j with port and time
         * \param[in] i the node sending
         * \param[in] j the node receiving
         * \param[in] p the port used at the receiving end
         * \param[in] time the time for the response if none=-1
         **/
        bool addEdge ( int i, int j, short unsigned int p, time_t time, suseconds_t micro ); 
                
        /**
         * \brief adds a edge between i->j with information whether the message was received (connection is up)
         * \param[in] i the node sending
         * \param[in] j the node receiving
         * \param[in] up the connection is up or down?
         **/
        bool addEdge_prop ( int i, int j, bool up ); 
        
        /**
         * \brief returns the average response time for the edge i->j with port p
         * \param[in] i the node sending
         * \param[in] j the node receiving
         * \param[in] p the port used at the receiving end
         **/
        double getTime ( int i, int j, unsigned short p );      
        /**
         * \brief returns true if there is at least one edge between i->j
         * \param[in] i the node sending
         * \param[in] j the node receiving
         */
        bool isEdge ( int i, int j );
        
        int getSize()
        {
            return this->size;
        }
        
        /**
         * \brief depth first search, dividing the set of edges into the set, that are inside the DFS-Tree and the ones, that aren't
         * \param[out] dfs_tree the resulting set of edges, that are inside the DFS-tree (must be alloc'd)
         * \param[out] invisible the set of edges, that are not inside the DFS-tree (must be alloc'd)
         * \param[out] roots array, containing the identifiers of the root-elements of the DFS-Forest. (must be alloc'd, at least 1+(number of roots))
         * \return false if dfs_tree or invisible are NULL, true otherwise
         **/
        bool dfs ( base::graph<bool>* dfs_tree, base::graph<bool>* invisible, int* roots );
    private:
        /**
         * \brief inner function for recursive part of the dfs-algorithm
         * \param[in] current the node, which is currently computed
         * \param[in] visited array, which indicates, whether a node was already visited or not
         * \param[out] dfs_tree the resulting set of edges, that are inside the DFS-tree (must be alloc'd)
         * \param[out] invisible the set of edges, that are not inside the DFS-tree (must be alloc'd)
         * \return true
         **/
        bool dfs_inner ( int current, bool* visited, base::graph<bool>* dfs_tree, base::graph<bool>* invisible );
    public:
        /**
         * \brief brings the graph in the DOT-format for graphviz tool
         * \param[in] filename the file to be written (will be overwritten if already exists)
         **/
        void vis_f ( const char* filename );     
        /**
         * \brief brings the graph in the DOT-format for graphviz tool
         * \param[in] f the file-handle to write to
         **/   
        void vis ( FILE* f , NodeIdentificator* nodeId);
        
        
};
#endif
}

#endif
