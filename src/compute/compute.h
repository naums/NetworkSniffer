#ifndef NAUMS_RELIABLE_COMPUTE
#define NAUMS_RELIABLE_COMPUTE

#include "../kafka/kuser.h"
#include "../mysql/mysql.h"
#include "../pcap/trace.h"
#include "../base/structures/list.h"
#include "../config.h"
#include "../base/structures/graph.h"
#include "../base/structures/nodeIdentificator.h"
#include "../base/structures/structs.h"

namespace compute
{
    /**
     * \brief creates an graph-object with adjacancy-matrix from a list of adjacent nodes. Will save some nodes in found
     * \param[in] list list of adjacent nodes (contains e = {ip:port , ip:port})
     * \param[in|out] found will contain the found ip:port according to index of the node
     * \param[in] dir if set true the edges will be directed, if false undirected
     * \return the resultung graph-object
     * \note found must be alloc'd as ArrayList!
     **/
    base::graph<bool>* adjToGraph ( base::list<struct edge*>* list, base::ArrayList<long unsigned int>* found, bool dir=false );

    /**
     * \brief checks if a graph is already in a list of graph
     * \note the node-order must be the same
     * \param[in] graphs the list of graphs
     * \param[in] graph the graph to check with
     * \return true if the graph is in graphs-list, false otherwise
     **/
    bool inGraphList ( base::ArrayList<base::graph<bool>*>* graphs, base::graph<bool>* graph );
    
    /**
     * \brief creates a topology-graph where edges represent communication between computers
     * \param[in|out] graph the graph that represents the network topology
     * \param[in] db the database-connection
     * \param[in] cfg config object 
     * \param[in] nodeId node identificator to get names for nodes
     **/
    void mkgraph_topo ( base::graph<bool>* graph, mysql* db, config* cfg, NodeIdentificator* nodeId );
    
    
    /**
     * \brief creates the dependancy-lists
     * \param[in] db Database connection
     * \param[in] cfg config-object containing the relation-names
     * \param[in] nodeId overview over the computers and IP-Adresses in the network
     * \param[in] mu Threshold
     * \return list of adjacency-lists
     **/
    base::ArrayList<base::list<struct edge*>*>* mkgraph_dep ( mysql* db, config* cfg, NodeIdentificator* nodeId, const suseconds_t mu );
    /**
     * \brief follows the messages recursivly
     * \param[in] res Messagedata from the database
     * \param[in] index of the next to compute message
     * \param[in] nodeId overview over the computers and IP-Adresses in the network
     * \param[in] startIp the IP-Adress, the next message has to come from
     * \param[in] startPort the Port, the application listens to
     * \param[in] vorherIp the IP-Adress the previous message was sent from
     * \param[in] vorherPort the Port the previous message was sent from
     * \param[in] timestamp the timestamp of the previous message
     * \param[in] mikrostamp the mikroseconds the previous message was sent
     * \param[in] mu Threshold
     * \param[in|out] g the current list of edges
     **/
    void mkgraph_dep_inner ( mysqlpp::StoreQueryResult res, 
                             unsigned int index, 
                             NodeIdentificator* nodeId, 
                             unsigned int startIp, 
                             unsigned short int startPort, 
                             unsigned int vorherIp, 
                             unsigned short int vorherPort, 
                             time_t timestamp, 
                             suseconds_t mikrostamp, 
                             const suseconds_t mu, 
                             base::list<struct edge*>* g );


    /**
     * \brief follows recursively the messages
     * \param[in|out] graph graph in adjacency list presentation
     * \param[in] startIp the current IP the next message should start from
     * \param[in] startPort the port, the current applications listens to
     * \param[in] mu Threshold
     * \param[in] zeitstempel timestamp of the last message
     * \param[in] micro timestamp in microseconds of the last message
     * \param[in] res the message-data from the database
     * \param[in] index the next index in the message-data-liste
     * \param[in] marked contains the indizes of the marked messages
     **/
    void mkgraph_process_inner ( base::list<struct edge*>* graph, 
                                 unsigned int startIp, 
                                 short unsigned int startPort, 
                                 const suseconds_t mu, 
                                 time_t zeitstempel, 
                                 suseconds_t micro, 
                                 mysqlpp::StoreQueryResult* res, 
                                 unsigned int index, 
                                 base::ArrayList<unsigned int>* marked,
                                 NodeIdentificator* nodeId );
    /**
     * \brief computes the message-list into a process-topology graph
     * \param[in|out] graph the resulting graph in adjacency presentation
     * \param[in] db Database-Connection
     * \param[in] cfg Config-Object containing the MySQL-Relationnames
     * \param[in] nodeId Overview over IPs and computers
     * \param[in] mu Threshold
     **/
    void mkgraph_process ( base::list<struct edge*>* graph, mysql* db, config* cfg, NodeIdentificator* nodeId, const suseconds_t mu );
}

#endif
