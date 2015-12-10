#ifndef NAUMS_COMPUTE_DEPENDANCY
#define NAUMS_COMPUTE_DEPENDANCY

#include "compute.h"
#include "../base/structures/list.h"
#include "../base/structures/arraylist.h"
#include "../base/structures/nodeIdentificator.h"
#include "../mysql/mysql.h"
#include "../config.h"

namespace compute
{
    base::ArrayList<base::list<struct edge*>*>* mkgraph_dep ( mysql* db, config* cfg, NodeIdentificator* nodeId, const suseconds_t mu )
    {
        base::ArrayList<base::list<struct edge*>*>* graphs = new base::ArrayList<base::list<struct edge*>*>();
        mysqlpp::Query query_mesg (db->getConnection());
        query_mesg << "SELECT * FROM " << cfg->mysql_trace_table << " ORDER BY timestamp, microseconds ASC";
        mysqlpp::StoreQueryResult res = db->query ( query_mesg );
        for (unsigned int i=0; i<res.num_rows(); i++)
        {
            unsigned int srcIp = ntohl ( res[i]["srcip"] );
            unsigned short int srcPort = ntohs ( res[i]["srcport"] );
            unsigned int destIp = ntohl ( res[i]["destip"] );
            unsigned short int destPort = ntohs ( res[i]["destport"] );
            
            time_t timestamp = res[i]["timestamp"];
            suseconds_t mikrostamp = res[i]["microseconds"];
            
            // Einschränkung der Ports
            if ((destPort > 11090 || destPort < 8080) && destPort != 3306)
                continue;
            
            // 1. Nachricht ist nicht von "outside" -> überspringen
            if (nodeId->getIdByIp ( srcIp ) > 0) 
                continue;
            
            // Zielknoten darf nicht "outside" sein.
            if (nodeId->getIdByIp ( destIp ) > 0)
            {
                base::list<struct edge*>* g = new base::list<struct edge*>( );
                struct edge* e = new struct edge;
                e->startIp = srcIp;
                e->startPort = srcPort;
                e->targetIp = destIp;
                e->targetPort = destPort;
                
                g->add(e);
                
                static int z=0; z++;
                printf ("[%d] %d %hu -> %d %hu\n", z, srcIp, srcPort, destIp, destPort);
                
                mkgraph_dep_inner ( res, (1+i), nodeId, destIp, destPort, srcIp, srcPort, timestamp, mikrostamp, mu, g );
                
                graphs->add(g);
            }
        }
        
        return graphs;
    }
    
    void mkgraph_dep_inner ( mysqlpp::StoreQueryResult res, unsigned int index, NodeIdentificator* nodeId, unsigned int startIp, unsigned short int startPort, unsigned int vorherIp, unsigned short int vorherPort, time_t timestamp, suseconds_t mikrostamp, const suseconds_t mu, base::list<struct edge*>* g )
    {
        mikrostamp += mu;
        if (mikrostamp >= 1000000) 
        {
            mikrostamp -= 1000000;
            timestamp++;
        }
        
        for (unsigned int i=index; i<res.num_rows(); i++)
        {
            unsigned int srcIp = ntohl ( res[i]["srcip"] );
            unsigned short int srcPort = ntohs ( res[i]["srcport"] );
            unsigned int destIp = ntohl ( res[i]["destip"] );
            unsigned short int destPort = ntohs ( res[i]["destport"] );
            
            time_t ts = res[i]["timestamp"];
            suseconds_t ms = res[i]["microseconds"];

            if ((destPort > 11090 || destPort < 8080) && destPort != 3306)
                continue;

            // Die gefunde Nachricht ist außerhalb des Thresholds
            if (ts > timestamp || (ts == timestamp && ms > mikrostamp))
                return;

            // Die Nachricht ist nicht vom gewünschten Absender
            if (startIp != srcIp)
                continue;

            // Die Nachricht geht als Antwort zurück
            if (vorherIp == destIp && vorherPort == destPort)
                continue;
                
            struct edge* e = new struct edge;
            e->startIp = startIp;
            e->startPort = startPort;
            e->targetIp = destIp;
            e->targetPort = destPort;
            
            g->add(e);
            
            mkgraph_dep_inner ( res, (1+i), nodeId, destIp, destPort, srcIp, srcPort, ts, ms, mu, g );
        }
    }
}

#endif
