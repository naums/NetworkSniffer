#include "compute.h"

/// HACK! 
// I need to know some information about the network, because I don't have the application names 
// within the database, because I cannot get the applicationnames on the capture-PC - this PC 
// does not know the application names. unforunatly.
//#include "../../mytest/conf.h"

#include <iostream>

using namespace base;

namespace compute
{
    void mkgraph_topo ( base::graph<bool>* graph, mysql* db, config* cfg, NodeIdentificator* nodeID )
    {
        // every message represents an edge (if not already in the graph)
        mysqlpp::Query query_mesg (db->getConnection());
        query_mesg << "SELECT * FROM " << cfg->mysql_trace_table << " ORDER BY timestamp, microseconds ASC";
        mysqlpp::StoreQueryResult res = db->query ( query_mesg );
        for (unsigned int j=0; j<res.num_rows(); j++)
        {
            unsigned int srcIp = ntohl ( res[j]["srcip"] );
            unsigned int destIp = ntohl ( res[j]["destip"] );

            graph->addEdge ( nodeID->getIdByIp( srcIp ), nodeID->getIdByIp( destIp ), true );    
        }
    }
    
    base::graph<bool>* adjToGraph ( base::list<struct edge*>* list, base::ArrayList<long unsigned int>* found, bool dir )
    {   
        if (found==NULL)
            return NULL;
            
        int size=0;
        long unsigned int id=0;
        base::list_node<struct edge*>* tmp = list->getFirst();
        // count out, how many nodes there really are
        while ( tmp != NULL )
        {
            struct edge* e = tmp->getValue();
            id = ((long unsigned int) e->startIp << 32) + e->startPort;
            if (found->index(id) == -1)
            {
                size++;
                found->add(id);
            }
            
            id = ((long unsigned int) e->targetIp << 32) + e->targetPort;
            if (found->index(id) == -1)
            {
                size++;
                found->add(id);
            }
            
            tmp=tmp->next;
        }
        
        base::graph<bool>* graph = new base::graph<bool>( size, false );
        
        // fill the graph with edges
        long unsigned int srcid, destid;
        tmp = list->getFirst();
        while ( tmp != NULL )
        {
            struct edge* e = tmp->getValue();
            srcid = ((long unsigned int) e->startIp << 32) + e->startPort;
            destid = ((long unsigned int) e->targetIp << 32) + e->targetPort;
            
            // in theory, there should only be an ip+port once in found-ArrayList
            // so, the index can be used as node-number
            graph->addEdge ( found->index(srcid), found->index(destid), true, dir );
                        
            tmp=tmp->next;
        }
        
        
        
        return graph;
    }
            
    bool inGraphList ( base::ArrayList<base::graph<bool>*>* graphs, base::graph<bool>* graph)
    {
        base::graph<bool>* g;
        for (int i=0; i<graphs->getSize(); i++)
        {
            g = graphs->getValue(i);
            bool copy=true;
            for (int x=0; x<g->getSize() && copy; x++)
            {
                for (int y=0; y<g->getSize() && copy; y++)
                {
                    if (g->getValue(x,y) != graph->getValue(x,y))
                    {
                        copy=false;
                    }
                }
            }
            
            if (copy) return true;
        }
        
        return false;
    }
    
}
