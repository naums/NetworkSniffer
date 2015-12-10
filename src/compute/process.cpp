#ifndef NAUMS_COMPUTE_PROCESS
#define NAUMS_COMPUTE_PROCESS

#include "compute.h"
#include "../base/structures/list.h"
#include "../base/structures/arraylist.h"
#include "../base/structures/nodeIdentificator.h"
#include "../mysql/mysql.h"
#include "../config.h"

namespace compute
{
    void mkgraph_process ( base::list<struct edge*>* graph, mysql* db, config* cfg, NodeIdentificator* nodeId, const suseconds_t mu )
    {
        base::ArrayList<unsigned int>* marked = new base::ArrayList<unsigned int>(100, -1, 1000);
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
            suseconds_t microstamp = res[i]["microseconds"];
            
            if ((destPort > 11090 || destPort < 8080) && destPort != 3306)
                continue;
            
            // Zielknoten "outside" überspringen
            if (nodeId->getIdByIp ( destIp ) == 0)
                continue;
                
            // markierte Nachrichten überspringen
            if (marked->index(i) != -1)
                continue;
            
            struct edge* e = new struct edge;
            e->startIp = srcIp;
            
            // Startknoten outside Knoten -> Port ist egal
            if (nodeId->getIdByIp ( srcIp ) != 0)
                e->startPort = srcPort;
            else
                e->startPort = 0;
            e->targetIp = destIp;
            e->targetPort = destPort;
            
            graph->add ( e );
            
            mkgraph_process_inner ( graph, destIp, destPort, mu, timestamp, microstamp, &res, (1+i), marked, nodeId );
        }
    }

    void mkgraph_process_inner ( base::list<struct edge*>* graph, unsigned int startIp, short unsigned int startPort, const suseconds_t mu, time_t zeitstempel, suseconds_t micro, mysqlpp::StoreQueryResult* res, unsigned int index, base::ArrayList<unsigned int>* marked, NodeIdentificator* nodeId )
    {
        micro += mu;
        if (micro >= 1000000) 
        {
            micro -= 1000000;
            zeitstempel++;
        }
        
        mysqlpp::StoreQueryResult messageList = *res;
        for (unsigned int i=index; i<messageList.num_rows(); i++)
        {
            time_t timestamp = messageList[i]["timestamp"];
            suseconds_t microstamp = messageList[i]["microseconds"];
            unsigned int srcip = (ntohl ( messageList[i]["srcIp"] ));
            unsigned int destip = (ntohl ( messageList[i]["destIp"] ));
            unsigned short int destPort = (ntohs ( messageList[i]["destPort"] ));
            
            if ((destPort > 11090 || destPort < 8080) && destPort != 3306)
                continue;
            
            // Zeitstempel größer als Theshold -> abbruch
            if (timestamp > zeitstempel || (timestamp == zeitstempel && microstamp > micro))
                return;
                
            // markierte Nachricht überspringen
            if (marked->index ( i )!=-1)
                continue;
            
            // Nachricht wurde nicht vom Startknoten versendet
            if (srcip != startIp)
                continue;
                
                
            // Kante einfügen
            struct edge* e = new struct edge;
            e->startIp = startIp;

            // outside Knoten -> Port ist egal
            if (nodeId->getIdByIp ( srcip ) != 0)
                e->startPort = startPort;
            else
                e->startPort = 0;
            e->targetIp = destip;
            e->targetPort = destPort;
            
            graph->add ( e );
            
            // Nachricht markieren
            marked->add ( i );
            
            // rekursiver Abstieg, Zielknoten wird neuer Startknoten
            mkgraph_process_inner ( graph, destip, destPort, mu, timestamp, microstamp, res, (1+i), marked, nodeId );
        }
    }
}

#endif
