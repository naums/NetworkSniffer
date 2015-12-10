#include "pcap/pcap.h"
#include "kafka/kuser.h"
#include "config.h"
#include "sniffer.h"
#include "compute/compute.h"

#include "base/structures/nodeIdentificator.h"
#include "base/structures/graph.h"
#include "base/structures/structs.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <unistd.h>

#define MYSQL_SERVER "127.0.0.1\0"
#define MYSQL_USERNAME "test"
#define MYSQL_PASSWD "passwd"
#define MYSQL_DBNAME "TRACE_VLAN_PSH"
#define MYSQL_TRACE "trace"
#define MYSQL_INIT "init"
#define MYSQL_APP "application"

using namespace network;

int main ( int argc, char** argv )
{
    printf ("Usage: %s [graphfile]\n", argv[0]);

    const char* graphfile = (argc>=2 ? argv[1] : NULL);
        
    // config
    const char* srv    = (MYSQL_SERVER);
    const char* usr    = (MYSQL_USERNAME);
    const char* passwd = (MYSQL_PASSWD);
    const char* dbname = (MYSQL_DBNAME);
    const char* trace  = (MYSQL_TRACE);
    const char* init   = (MYSQL_INIT);
    const char* app    = (MYSQL_APP);
 
    config* cfg = new config(srv, usr, passwd, dbname, trace, init, app);
    
    mysql* db = mysql::connect ( srv, usr, passwd, dbname);
    mysqlpp::StoreQueryResult res;

    base::graph<bool>* graph;
    NodeIdentificator* nodeId;
    
    if (graphfile == NULL)
    {
        mysqlpp::Query q1 (db->getConnection());
        q1 << "SELECT count(id) as numdev FROM " << cfg->mysql_init_table;
        mysqlpp::Query q2 (db->getConnection());
        q2 << "SELECT count(hostId) as numhost FROM " << cfg->mysql_init_table << "_hostname";
        
        res = db->query ( q1 );
        if (res.num_rows()<1)
            return 1;
        
        int numdev = (res[0]["numdev"] == mysqlpp::null ? 0 : res[0]["numdev"]) ;
        if (numdev<=0)
            return 2;
        
        res = db->query ( q2 );
        if (res.num_rows()<1)
            return 1;
        
        int numhost= (res[0]["numhost"] == mysqlpp::null ? 0 : res[0]["numhost"]) ;
        if (numdev<=0)
            return 2;
        
        // anzahl der hosts + outside
        nodeId = new NodeIdentificator (numhost, numdev);
        graph = new base::graph<bool> ( (numhost+1), false );
        
        // for naming the (computer) nodes
        mysqlpp::Query q3 (db->getConnection());
        q3 << "SELECT hostId, hostname FROM " << cfg->mysql_init_table << "_hostname";
        res = db->query ( q3 );
        for (unsigned int i=0; i<res.num_rows(); i++)
        {
            res[i]["hostname"].length();
            char* buff = (char*) malloc ( (res[i]["hostname"].length()+1) * sizeof(char));
            strncpy(buff, res[i]["hostname"].c_str(), res[i]["hostname"].length());
            buff[res[i]["hostname"].length()]='\0';
            nodeId->addName ( res[i]["hostId"], buff );
        }
        
        nodeId->debugNames();

        mysqlpp::Query q4 (db->getConnection());
        q4 << "SELECT hostId, ip FROM " << cfg->mysql_init_table << " ORDER BY ip ASC";
        res = db->query ( q4 );
        for (unsigned int i=0; i<res.num_rows(); i++)
        {
            nodeId->addDevice ( res[i]["hostId"], res[i]["ip"] );
        };
        
        compute::mkgraph_topo ( graph, db, cfg, nodeId );
        
    }
    else
    {
        graph = base::graph<bool>::fromFile ( graphfile, &nodeId );
    }
    
    /***
     ** TOPOLOGIEGRAPH
    ***/
    graph->removeLoops();
	graph->vis_f("dot/topo_graph.dot", nodeId);
	
	// do Depth First search
	base::graph<bool>* inv = new base::graph<bool>(graph->getSize(), graph->getNull());
	base::graph<bool>* dfs_tree = graph->dfs( inv );
		
    // reverse the edges of the dfs-tree and edges not in dfs-tree
	dfs_tree->reverseEdges();
	inv->reverseEdges();
	
	// write graphs to GRAPHVIZ-Files
	//dfs_tree->vis_f("dfs_tree.dot", nodeId, true);
	//inv->vis_f("inv.dot", nodeId, true);
	
	// Chain Decomposition
	base::list<base::ArrayList<struct base::edge2*>*>* chains;
	chains = dfs_tree->chainDecomposite(inv);
	
	// Subtract the chains from graph to get bridges
	// get CutVertices
	base::graph<bool>* bridges = graph->differenceChains ( chains );
	base::ArrayList<int>* cv = bridges->cutVertices ( chains );
	
	base::graph<bool>::printChainDecomp ( chains, nodeId );
	
	graph->vis_f_ex ( "dot/topo_ex.dot", nodeId, false, true, bridges, NULL, cv );
	
	printf ("CUT VERTICES:\n");
	for (int i=0; i<cv->getSize(); i++)
	{
	    printf (" %d, ", cv->getValue(i));
	}
	printf("\n");
	
	printf ("BRIDGES\n");
	for (int i=0; i<bridges->getSize(); i++)
	{
	    for (int j=0; j<bridges->getSize(); j++)
	    {
	        if (bridges->isEdge(i,j))
	            printf ("  %d --> %d \n", i,j );
	    }
	}
	
	delete bridges; 
	delete cv;
	delete graph;
	delete chains;
	delete dfs_tree;
	delete inv;
	
	/***
	 ** PROCESSES
	***/
	printf ("PROCESSES!!!\n");
	
	suseconds_t mu[] = {50000000, 5000000, 500000, 50000, 10000, 5000, 1000};
	for (int i=0; i<7; i++)
	{
	    base::list<struct edge*>* process_list = new base::list<struct edge*>();
	    base::ArrayList<long unsigned int>* found = new base::ArrayList<long unsigned int>(100);
        compute::mkgraph_process ( process_list, db, cfg, nodeId, mu[i] );
        base::graph<bool>* pgraph = compute::adjToGraph ( process_list, found );
        
        delete process_list;
        
        pgraph->removeLoops();
	    pgraph->vis_f_ex("dot/process_graph.dot", nodeId, true, false, NULL, found, NULL );
	
	    // do Depth First search
	    base::graph<bool>* pinv = new base::graph<bool>(pgraph->getSize(), pgraph->getNull());
	    base::graph<bool>* pdfs_tree = pgraph->dfs( pinv );
		
        // reverse the edges of the dfs-tree and edges not in dfs-tree
	    pdfs_tree->reverseEdges();
	    pinv->reverseEdges();
	
	    // write graphs to GRAPHVIZ-Files
	    pdfs_tree->vis_f_ex("dot/process_tree.dot", nodeId, true, false, NULL, found);
	    pinv->vis_f_ex("dot/process_inv.dot", nodeId, true, false, NULL, found);
	
	    // Chain Decomposition
	    base::list<base::ArrayList<struct base::edge2*>*>* pchains;
	    pchains = pdfs_tree->chainDecomposite(pinv);
	
	    // Subtract the chains from graph to get bridges
	    // get CutVertices
	    base::graph<bool>* pbridges = pgraph->differenceChains ( pchains );
	    base::ArrayList<int>* pcv = pbridges->cutVertices ( pchains );
	
	    //base::graph<bool>::printChainDecomp ( pchains, nodeId, found );
	
	    char buffer[20];
	    sprintf (buffer, "dot/process_ex_%ld.dot\0", mu[i] );
	    pgraph->vis_f_ex ( buffer, nodeId, false, true, pbridges, found, pcv );
	    
	    FILE* fprocess = fopen( "process.plot", "a");
	    fprintf ( fprocess, "%ld %d %d %d %d\n", mu[i], pgraph->getSize(), pgraph->getEdgeNumber(), pbridges->getEdgeNumber(), pcv->getSize() );
	    fclose(fprocess);
	    
    	delete pbridges; 
	    delete pcv;
	    delete pgraph;
	    delete pchains;
	    delete pdfs_tree;
	    delete pinv;
	}
	/*printf ("CUT VERTICES:\n");
	for (int i=0; i<pcv->getSize(); i++)
	{
	    printf (" %d, ", pcv->getValue(i));
	}
	printf("\n");
	
	printf ("BRIDGES\n");
	for (int i=0; i<pbridges->getSize(); i++)
	{
	    for (int j=0; j<pbridges->getSize(); j++)
	    {
	        if (pbridges->isEdge(i,j))
	            printf ("  %d --> %d \n", i,j );
	    }
	}*/
	
	/***
	 ** DEPENDANCY
	***/
	printf ("DEPENDANCY!!!\n");
	suseconds_t dmu[] = {/*50000000, 5000000, 500000,*/  50000, 10000, 5000, 1000};
	//int i=3;
	for (int i=0; i<4; i++)
	{
	    base::ArrayList<base::list<struct edge*>*>* dep_list = compute::mkgraph_dep ( db, cfg, nodeId, dmu[i] );
	    for (int j=0; j<dep_list->getSize(); j++)
	    {
    	    base::ArrayList<long unsigned int>* dfound = new base::ArrayList<long unsigned int>(100);
            base::graph<bool>* dgraph = compute::adjToGraph ( dep_list->getValue(j), dfound, true );
            printf ("[%d] %d, %d\n", j, dgraph->getSize(), dgraph->getEdgeNumber());
            //dgraph->debug();
            
            dgraph->removeLoops();
	        dgraph->vis_f_ex("dot/dep_graph.dot", nodeId, true, false, NULL, dfound, NULL );
	
	        // do Depth First search
	        base::graph<bool>* dinv = new base::graph<bool>(dgraph->getSize(), dgraph->getNull());
	        base::graph<bool>* ddfs_tree = dgraph->dfs( dinv );
		
            // reverse the edges of the dfs-tree and edges not in dfs-tree
	        ddfs_tree->reverseEdges();
	        dinv->reverseEdges();
	
	        // write graphs to GRAPHVIZ-Files
	        dgraph->vis_f_ex("dot/dep_tree.dot", nodeId, false, true, dinv);
	        dinv->vis_f("dot/dep_inv.dot", nodeId, true);
	
	        // Chain Decomposition
	        base::list<base::ArrayList<struct base::edge2*>*>* dchains;
	        dchains = ddfs_tree->chainDecomposite(dinv);
	
	        // Subtract the chains from graph to get bridges
	        // get CutVertices
	        base::graph<bool>* dbridges = dgraph->differenceChains ( dchains );
	        base::ArrayList<int>* dcv = dbridges->cutVertices ( dchains );
	
	        //base::graph<bool>::printChainDecomp ( pchains, nodeId );
	
	        char buffer[40];
	        sprintf (buffer, "dot/dep_ex_%ld_%d.dot", dmu[i], j);
	        dgraph->vis_f_ex ( buffer, nodeId, true, true, dbridges, dfound, dcv );
	    
	        FILE* fprocess = fopen( "dot/dep_syn.data", "a");
	        fprintf ( fprocess, "%ld %d %d %d %d %d\n", dmu[i], j, dgraph->getSize(), dgraph->getEdgeNumber(), dbridges->getEdgeNumber(), dcv->getSize() );
	        fclose(fprocess);
	
	        /*printf ("CUT VERTICES:\n");
	        for (int i=0; i<dcv->getSize(); i++)
	        {
	            printf (" %d, ", dcv->getValue(i));
	        }
	        printf("\n");
	
	        printf ("BRIDGES\n");
	        for (int i=0; i<dbridges->getSize(); i++)
	        {
	            for (int j=0; j<dbridges->getSize(); j++)
	            {
	                if (dbridges->isEdge(i,j))
	                    printf ("  %d --> %d \n", i,j );
	            }
	        }*/
	
	        delete dbridges; 
	        delete dcv;
	        delete dgraph;
	        delete dchains;
	        delete ddfs_tree;
	        delete dinv;
            delete dfound;
	    }
	    delete dep_list;

	}
	
    //base::graph<bool>::printChainDecomp ( chains, nodeId );

	delete nodeId;

	//delete db;
	//delete cfg;
    
}

