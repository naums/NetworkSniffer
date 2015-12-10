#ifndef NAUMS_BASE_GGRAPH
#define NAUMS_BASE_GGRAPH

#include <cstdio>
#include <cstring>

#include "structs.h"

#include "nodeIdentificator.h"
#include "list.h"
#include "arraylist.h"

#include "mgraph.h"

namespace compute{
    const char* AppNameById ( unsigned int id, base::ArrayList<struct application*>* appId );
}

namespace base {

/**
 * \class graph a template-graph-data-structure, that uses an adjacency matrix for saving the edges
 **/
template <typename t1>
class graph
{
    private:
        /**
         * adjacency matrix of the graph
         * payload[i*this->size + j] is the edge from i to j; i determines the row, j the column
         **/
        t1* payload;
        /// the size of the graph (number of nodes)
        int size;
        /// null-object of the graph (showing, that there is no edge i->j in payload)
        t1 null;
    
    public:
        /**
         * \brief initiates the adjacency matrix with null-elements, malloc's the memory
         * \param[in] s the number of nodes in the graph
         * \param[in] n the null-element in the graph
         **/
        graph( int s, t1 n )
        {
            this->size=s;
            this->payload = (t1*) malloc ( this->size*this->size * sizeof(t1) );
            this->null = n;
            
            t1* ptr = this->payload;
            for (int i=0; i<this->size*this->size; i++)
            {
                *ptr++ = this->null;
            }
        }
        
        /**
         * \brief frees the memory for the adjacency matrix
         **/
        ~graph()
        {
            free ( this->payload );
        }
        
        /**
         * \brief reads a graph from file
         * \param[in] filename the path to the file to be read
         * \param[out] nodeId nodeId Object with read names of the nodes
         * \return graph, that has been read
         * \node does expect integer (or bool) type
         **/
        static base::graph<t1>* fromFile ( const char* filename, NodeIdentificator** nodeId )
        {
            FILE* f = fopen(filename, "r");
            base::graph<t1>* result = fromFile_f( f, nodeId );
            fclose(f);
            return result;
        }        
        
        /**
         * \brief reads a graph from file
         * \param[in] f fileptr to the file to be read
         * \param[out] nodeId nodeId Object with read names of the nodes
         * \return graph, that has been read
         * \node does expect integer (or bool) type
         **/
        static base::graph<t1>* fromFile_f ( FILE* f, NodeIdentificator** nodeId )
        {
            int n, s;
            fscanf (f, "NULL: %d\n", &n);
            fscanf (f, "NODES: %d\n", &s);
            base::graph<t1>* result = new base::graph<t1>(s, n);
            
            *nodeId= new NodeIdentificator ( s,0 );
            NodeIdentificator* nn = *nodeId;
            char* buffer=(char*) malloc(100*sizeof(char));
            for (int i=1; i<s; i++)
            {
                int d=0;
                fscanf(f, "NAME[%d]: %s\n", &d, buffer);
                char* bb = (char*) malloc ( (strlen(buffer)+1) * sizeof(char));
                strncpy ( bb, buffer, strlen(buffer));
                bb[strlen(buffer)] = '\0';
                nn->addName(d, bb);
            }

            int val;
            int i,j;
            
            long int cpos = ftell(f);
            fseek ( f, 0, SEEK_END );
            long int end = ftell(f);
            fseek ( f, cpos, SEEK_SET );
            while (cpos < end)
            {
                fscanf (f, "%d -> %d: %d\n", &i, &j, &val);
                result->addEdge(i,j, val, true);
                cpos = ftell(f);
            }
            
            return result;
        }
        
        /**
         * \brief returns the value at the given index
         * \param[in] y the index
         * \return the payload-value
         **/
        t1 at ( int y )
        {
            return this->payload[y];
        }
        
        /**
         * \brief returns the value at the given index
         * \param[in] y the index
         * \return the payload-value
         **/
        t1 operator[] ( int y )
        {
            return this->at(y);
        }
        
        /**
         * \brief adds (or changes) an edge from node i to j with the value value into the graph
         * \param[in] i the number of the starting-node (starting with 0)
         * \param[in] j the number of the target-node
         * \param[in] value the value to be written as weight 
         * \param[in] directed whether the edge is directed or not
         * \note an undirected node is set in payload[i*this->size + j] AND payload[j*this->size + i]
         * \return true if sucessfully added, false otherwise
         **/
        bool addEdge ( int i, int j, t1 value, bool directed=false )
        {
            if (i<0 || i>=this->size || j<0 || j>=this->size)
                return false;
            
            this->payload[i*this->size + j] = value;
            
            if (!directed)
                this->payload[j*this->size + i] = value;
            return true;
        }
        
        /**
         * \brief removes the edge between i and j
         * \param[in] i the number of the starting-node (starting with 0)
         * \param[in] j the number of the target-node
         * \param[in] directed whether the edge is directed or not
         * \note if the edge was undirected, then it will be deleted two times!
         * \return true if sucessfully removed, false otherwise
         **/
        bool removeEdge ( int i, int j, bool directed=false )
        {
            return this->addEdge ( i, j, this->null, directed );
        }
        
        /** 
         * \brief returns whether the edge i->j exists
         * \param[in] i start-node
         * \param[in] j target-node
         * \return true if the edge i->j exists, false otherwise
         * \note will only return true if i->j exists; if j->i exists, but not the other way around, it returns false!
         **/
        bool isEdge ( int i, int j )
        {
            if (i<0 || i>=this->size || j<0 || j>=this->size)
                return false;
            
            return !(this->payload[i*this->size + j] == this->null);
        }
        
        void debug ()
        {
            for (int i=0; i<this->size; i++)
            {
                for (int j=0; j<this->size; j++)
                {
                    if (isEdge(i,j))
                        printf (" 1 ");
                    else
                        printf (" 0 ");
                }
                putchar('\n');
            }
        }
        
        /**
         * \brief returns the saved value for the edge i->j
         * \param[in] i start-node
         * \param[in] j target-node
         * \return the weight of the edge i->j
         **/
        t1 getValue ( int i, int j )
        {
            if (i<0 || i>=this->size || j<0 || j>=this->size)
                return false;
            
            return (this->payload[i*this->size + j]);
        }
        
        void removeLoops ()
        {
            for (int i=0; i<this->size; i++)
            {
                this->payload[i*this->size + i] = this->null;
            }
        }
        
        
        
        /**
         * \brief reduces the adj-matrix
         * \param[in] start the start value for the reduction (eg. 0 for addition)
         * \param[in] red a reduction-function, that takes the current reduction-result, the current value and its coordinates within the matrix
         * \return the result of the reduction function (eg. the sum of all elements)
         **/
        t1 reduce ( t1 start, t1 (*red) (t1, t1, int, int) )
        {
            t1 all=start;
            t1* ptr = this->payload;
            for (int i=0; i<this->size; i++)
            {
                for (int j=0; j<this->size; j++)
                {
                    all=red(all, *ptr, i, j);
                }
            }
            return all;
        }
        
        /**
         * \return the number of nodes
         **/
        int getSize()
        {
            return this->size;
        }
        
        int getEdgeNumber()
        {
            int num=0;
            for (int i=0; i<this->size; i++)
            {
                for (int j=i+1; j<this->size; j++)
                {
                    if (isEdge(i,j))
                        num++;
                }
            }
            return num;
        }
        
        /**
         * \brief the null-objet
         **/
        t1 getNull ()
        {
            return this->null;
        }
        
        /**
         * \brief writes the graph into a file
         * \param[in] filename of the file to be written
         * \note will overwrite the file, if exists
         * \sa toFile_f
         **/
        void toFile ( const char* filename, NodeIdentificator* nodeId )
        {
            FILE* f = fopen ( filename, "w+");
            this->toFile_f( f, nodeId );
            fclose(f);
        }
        
        /**
         * \brief writes the graph into a file
         * \param[in] f the fileptr to be written into
         * \note will overwrite the file, if exists
         * \sa toFile_f
         **/
        void toFile_f ( FILE* f, NodeIdentificator* nodeId )
        {
            fprintf (f, "NULL: %d\n", this->null);
            fprintf (f, "NODES: %d\n", this->size);
            for (int i=1; i<this->size; i++)
            {
                fprintf (f, "NAME[%d]: %s\n", i, nodeId->name(i));
            }
            
            for (int i=0; i<this->size; i++)
            {
                for (int j=0; j<this->size; j++)
                {
                    fprintf (f, "%d -> %d: %d\n", i, j, this->getValue(i,j));
                }
            }
        }
        
        /**
         * \brief clones the graph-object
         * \returns an identic clone of this object
         **/
        base::graph<t1>* clone ()
        {
            base::graph<t1>* tmp = new base::graph<t1>(this->size, this->null);
            t1* ptr1 = tmp->payload;
            t1* ptr2 = this->payload;
            
            for (int i=0; i<this->size*this->size; i++)
            {
                *ptr1 = *ptr2;
                ptr1++; ptr2++;
            }
            
            return tmp;
        }
             
        /**
         * \brief Depth-First-Search on this graph
         * \param[out] inv the "invisible" edges, that are not in the dfs-tree
         * \return the dfs-tree
         * \note edges in dfs-tree are pointing towards the leafs
         **/
        base::graph<t1>* dfs (base::graph<t1>* inv=NULL)
        {
            bool* visited = (bool*) malloc ( this->size* sizeof(bool) );
            base::graph<t1>* tree = new base::graph<t1>(this->size, this->null);
            bool* vptr = visited;
            bool* visEdge = (bool*) malloc ( this->size*this->size* sizeof(bool) );
            
            for (int i=0; i<this->size; i++)
            {
                *vptr = false; vptr++;
            }
            
            vptr = visEdge;
            for (int i=0; i<this->size*this->size; i++)
            {
                *vptr = false; vptr++;
            }
            
            for (int i=0; i<this->size; i++)
            {
                if (visited[i])
                    continue;
                visited[i] = true;
            
                this->dfs_inner ( i, tree, inv, visited, visEdge );
            }
            free(visited);
            free(visEdge);
            return tree;
        }
        
        /**
         * \brief inner routine for dfs
         * \param[in] node the current node
         * \param[in|out] tree the dfs-tree
         * \param[in|out] the invisible edges (not in dfs tree, but in original graph)
         * \param[in|out] visited array, symboling the visited nodes (true) and not visited pointing higher in the tree (towards the root)
         * \param[in|out] visEdge array, symboling what edge has been visited (i->j or j->i visited : visEdge[i*this->size+j] = true)
         **/
        void dfs_inner ( int node, base::graph<t1>* tree, base::graph<t1>* inv, bool* visited, bool* visEdge)
        {
            int i=node;
            for (int j=0; j<this->size; j++)
            {
                if (i==j or this->isEdge ( i, j ) == false or visEdge[i*this->size+j] == true)
                    continue;
                    
                if (visited[j])
                {
                    if (inv!=NULL)
                        inv->addEdge ( i,j, this->getValue(i,j), true );
                    visEdge[i*this->size+j] = true;
                    visEdge[j*this->size+i] = true;
                }
                else
                {
                    visited[j]=true;
                    visEdge[i*this->size+j] = true;
                    visEdge[j*this->size+i] = true;
                    tree->addEdge ( i, j, this->getValue(i,j), true ); 
                    this->dfs_inner ( j, tree, inv, visited, visEdge );
                }
            }
        }
        
        /**
         * \brief reverts the edges of the graph ((i->j) ==> (j->i))
         **/
        void reverseEdges ( ) 
        {
            base::graph<t1>* tmp = new base::graph<t1>(this->size, this->null);
            for (int i=0; i<this->size; i++)
            {
                for (int j=0; j<this->size; j++)
                {
                    if (this->isEdge ( i,j ) == true)
                    {
                        tmp->addEdge ( j,i, this->getValue ( i, j ), true );
                        this->removeEdge ( i,j );
                    }
                }
            }
            
            t1* ptr1 = tmp->payload;
            t1* ptr2 = this->payload;
            for (int i=0; i<this->size*this->size; i++)
            {
                *ptr2 = *ptr1;
                ptr1++; ptr2++;
            }
            
            delete tmp;
        }
        
        /**
         * \brief does a chain decomposition of this graph (MUST be DFS-tree!)
         * \param[in] inv the edges of the graph, not in dfs-tree
         * \return list of chains or list of list of edges
         * \note return value is: a list of chains, chains are a list of edges;
                 inv-edges must point towards the leafs, tree-edges towards the root!
         **/
        base::list<base::ArrayList<struct edge2*>*>* chainDecomposite ( base::graph<t1>* inv )
        {
            bool* visited = new bool[this->size];
            bool* v = visited;
            
            for (int i=0; i<this->size; i++)
            {
                *v=false; v++;
            }
            
            base::list<base::ArrayList<struct edge2*>*>* chainList = new base::list<base::ArrayList<struct edge2*>*>();
            for (int i=0; i<this->size; i++)
            {
                for (int j=0; j<this->size; j++)
                {
                    if (inv->isEdge ( i,j ))
                    {
                        base::ArrayList<struct edge2*>* cchain = new base::ArrayList<struct edge2*>();
                        chainList->add(cchain);
                        
                        struct edge2* tmp = new struct edge2;
                        tmp->start = i;
                        tmp->target = j;
                        tmp->value = inv->getValue ( i,j );
                     
                        cchain->add( tmp );
                        
                        visited[i] = true;
                        
                        this->chain_inner ( j, cchain, visited);
                    }
                }
            }
            return chainList;
        }
        
        /**
         * \brief returns with tree-nodes to the root node (or the first visited node)
         * \paran[in] currentNode the current node, to get away from
         * \param[in|out] chain the currentList of the found edges in the current chain so far
         * \param[in|out] visited array, showing what node has been visited so far
         **/
        void chain_inner ( int currentNode, base::ArrayList<struct edge2*>* chain, bool* visited )
        {
            if (visited[currentNode])
                return;
            
            visited[currentNode] = true;
            
            for (int i=0; i<this->size; i++)
            {
                if (this->isEdge( currentNode, i ) == true)
                {
                    struct edge2* tmp = new struct edge2;
                    tmp->start = currentNode;
                    tmp->target = i;
                    tmp->value = this->getValue ( currentNode, i );
                 
                    chain->add( tmp );
                    this->chain_inner ( i, chain, visited );
                    return;
                }
            }
            
            printf ("I am BUGO .AND. I am ERROR!!!!!\nYou should not get here! Ever.\n");
        }
        
        /**
         * \brief return all edges, that aren't in a chain but in the original graph
         * \param[in] chains the chain-decomposition of the graph
         * \return returns the graph, only containing the edges, that are not in a graph
         * \note does not change the payload of this graph-object
         **/
        base::graph<t1>* differenceChains ( base::list<base::ArrayList<struct edge2*>*>* chains )
        {
        	base::graph<t1>* bridges = this->clone();
        	 
        	base::list_node<base::ArrayList<struct edge2*>*>* currentChain = chains->getFirst();
        	base::ArrayList<struct edge2*>* chain;
        	while (currentChain!=NULL)
        	{
        	    chain = currentChain->getValue();
        	    for (int i=0; i<chain->getSize(); i++)
        	    {
        	        struct edge2* tmp = chain->getValue(i);
        	        int s = tmp->start;
        	        int t = tmp->target;
        	         
        	        if (bridges->isEdge ( s, t ))
        	        {
        	            bridges->removeEdge(s,t);
        	        }
        	    }
        	     
        	    currentChain = currentChain->next;
        	}
        	 
        	for (int i=0; i<bridges->getSize(); i++)
        	{
        	    for (int j=0; j<bridges->getSize(); j++)
        	    {
        	        if (bridges->isEdge(i,j))
        	            printf (" E:: %d -> %d\n", i,j);
        	    }
        	}
        	 
            return bridges;
        }
        
        /**
         * \brief finds cutVertices in the graph
         * \param[in] chains the chain-decomposition of the graph
         * \return list of the cutVertices integer
         * \note this graph-object contains ONLY bridges!
         **/
        base::ArrayList<int>* cutVertices ( base::list<base::ArrayList<struct edge2*>*>* chains )
        {
            base::ArrayList<int>* cv = new base::ArrayList<int>();
            // firstly all vertices adjacent to bridges
        	for (int i=0; i<this->getSize(); i++)
        	{
        	    for (int j=0; j<this->getSize(); j++)
        	    {
        	        if (this->isEdge(i,j))
        	        {
        	            if (cv->index(i) == -1)
        	                cv->add(i);
        	                
        	            if (cv->index(j) == -1)
        	                cv->add(j);
        	        }
        	    }
        	}
        	        	
        	// then every node, that is starting point of a cycle, but not the first one!
        	base::list_node<base::ArrayList<struct edge2*>*>* currentChain = chains->getFirst();
        	base::ArrayList<struct edge2*>* chain;
        	bool f = true;
        	while (currentChain!=NULL)
        	{
        	    if (!f)
        	    {
            	    chain = currentChain->getValue();
            	    
        	        struct edge2* first = chain->getValue(0);
                    struct edge2* last = chain->getValue ( chain->getSize() -1 );
                    
                    if (first->start == last->target)
                    {
                        if (cv->index(first->start) == -1)
                            cv->add(first->start);
                    }
                }
                f = false;            	     
        	    currentChain = currentChain->next;
        	}
        	
        	return cv;
        }

        /**
         * \brief writes the edges into a dot-file for visualisation with GraphViz-Tool
         * \param[in] filename the filepath to the file you want to be written
         * \param[in] nodeID Object containing the names of the nodes, for having named nodes
         * \param[in] directed whether the graph is supposed to be a directed graph
         * \param[in] weighted whether the weight should be written on the edges (not implemented)
         * \note www.graphviz.org
         **/
        void vis_f ( const char* filename, NodeIdentificator* nodeID, bool directed = false, bool weighted = false )
        {
            FILE* f = fopen ( filename, "w+" );
            if (directed)
                this->vis_dir ( f, nodeID, weighted);
            else
                this->vis ( f, nodeID, weighted );
            fclose (f);
        }
        
        /**
         * \brief writes the edges into a dot-file for visualisation with GraphViz-Tool
         * \param[in] filename the filepath to the file you want to be written
         * \param[in] nodeID Object containing the names of the nodes, for having named nodes
         * \param[in] directed whether the graph is supposed to be a directed graph
         * \param[in] bridge whether bridges should be highlighted
         * \param[in] bridges the bridge-map
         * \param[in] found contains the ips and portnumbers of the nodes (according to node-number)
         * \param[in] cutVertex contains the cutVertizes of the graph
         * \note www.graphviz.org
         **/
        void vis_f_ex ( const char* filename, NodeIdentificator* nodeID, bool directed = false, bool bridge=false, base::graph<t1>* bridges=NULL, base::ArrayList<long unsigned int>* found=NULL, base::ArrayList<int>* cutVertex=NULL )
        {
            FILE* f = fopen ( filename, "w+" );
            if (directed)
                this->vis_dir_ex ( f, nodeID, bridge, bridges, found, cutVertex);
            else
                this->vis_ex ( f, nodeID, bridge, bridges, found, cutVertex );
            fclose (f);
        }
        
        /**
         * \brief writes the edges into a dot-file for visualisation with GraphViz-Tool, UNDIRECTED
         * \param[in] f Filehandle to an opened, writeable, empty file
         * \param[in] nodeID Object containing the names of the nodes, for having named nodes
         * \param[in] bridge whether bridges should be highlighted
         * \param[in] bridges the bridge-map
         * \param[in] found contains the ips and portnumbers of the nodes (according to node-number)
         * \param[in] cutVertex contains the cutVertizes of the graph
         * \note www.graphviz.org
         **/
        void vis_ex ( FILE* f, NodeIdentificator* nodeID, bool bridge=false, base::graph<t1>* bridges=NULL, base::ArrayList<long unsigned int>* found=NULL, base::ArrayList<int>* cutVertex=NULL )
        {      
            bool b=false;
            fprintf ( f, "graph {\n" );
            if (cutVertex!=NULL)
            {
                for (int i=0; i<cutVertex->getSize(); i++)
                {
                    int node = cutVertex->getValue( i );
                    if (found==NULL)
                        fprintf (f, "  \"%s\" [fillcolor=%s, style=filled]\n", nodeID->name(node), "red");
                                                                                    // 005a46
                    else
                    {
                        unsigned short int port1;
                        unsigned long int i1;
                        unsigned int ip;
                        
                        i1=found->getValue(node);
                        ip = i1 >> 32; // higher 32 bit
                        port1 = i1 & 0xffff;          // lowest 16 bit
                        
                        int n = nodeID->getIdByIp ( ip );
                        
                        fprintf (f, "  \"%s-%hu\" [fillcolor=%s, style=filled]\n", nodeID->name(n), port1, "red");//005a46
                    }
                }    
                fprintf (f, "\n");        
            }
            
            for (int i=0; i< this->size; i++)
            {
                for (int j=i; j<this->size; j++)
                {
                    if (!bridge || bridges==NULL) 
                        b=false;
                    else
                    {
                        if (bridges->isEdge(i,j)==true)
                            b=true;
                    }
                    if (this->isEdge ( i,j ) == true)
                    {
                        bool isBridge=false;
                        if (b) 
                        {
                            if (bridges->isEdge ( i,j ))
                                isBridge=true;
                        }
                        
                        if (found==NULL)
                            fprintf (f, "  \"%s\" -- \"%s\" [color=%s]\n", nodeID->name(i), nodeID->name(j), (!isBridge ? "black" : "red"));// 005a46
                        else
                        {
                            unsigned short int port1, port2;
                            unsigned long int i1, i2;
                            unsigned int ip, ip2;
                            
                            i1=found->getValue(i);
                            i2=found->getValue(j);
                            ip = i1 >> 32; // higher 32 bit
                            port1 = i1 & 0xffff;          // lowest 16 bit
                            ip2 = i2 >> 32; // higher 32 bit
                            port2 = i2 & 0xffff;          // lowest 16 bit
                            
                            int n = nodeID->getIdByIp ( (ip) );
                            int m = nodeID->getIdByIp ( (ip2) );
                            
                            fprintf (f, "  \"%s-%hu\" -- \"%s-%hu\" [color=%s]\n", nodeID->name(n), port1, nodeID->name(m), port2, (!isBridge ? "black" : "red"));
                        }                                                                                                                           // 005a46
                    }
                }
            }
            fprintf ( f, "}");
        }
        
        /**
         * \brief writes the edges into a dot-file for visualisation with GraphViz-Tool; DIRECTED GRAPH
         * \param[in] f Filehandle to an opened, writeable, empty file
         * \param[in] nodeID Object containing the names of the nodes, for having named nodes
         * \param[in] bridge whether bridges should be highlighted
         * \param[in] bridges the bridge-map
         * \param[in] found contains the ips and portnumbers of the nodes (according to node-number)
         * \param[in] cutVertex contains the cutVertizes of the graph
         * \note www.graphviz.org
         **/
        void vis_dir_ex ( FILE* f, NodeIdentificator* nodeID, bool bridge=false, base::graph<t1>* bridges=NULL, base::ArrayList<long unsigned int>* found=NULL, base::ArrayList<int>* cutVertex=NULL )
        {   
            bool b = false;
            fprintf ( f, "digraph {\n" );
            if (cutVertex!=NULL)
            {
                for (int i=0; i<cutVertex->getSize(); i++)
                {
                    int node = cutVertex->getValue( i );
                    if (found==NULL)
                        fprintf (f, "  \"%s\" [fillcolor=%s, style=filled]\n", nodeID->name(node), "red"); // 005a46
                    else
                    {
                        unsigned short int port1;
                        unsigned long int i1;
                        unsigned int ip;
                        
                        i1=found->getValue(node);
                        ip = i1 >> 32; // higher 32 bit
                        port1 = i1 & 0xffff;          // lowest 16 bit
                        
                        int n = nodeID->getIdByIp ( ip );
                        
                        fprintf (f, "  \"%s-%hu\" [fillcolor=%s, style=filled]\n", nodeID->name(n), port1, "red"); // 005a46
                    }
                }    
                fprintf (f, "\n");        
            }
            
            for (int i=0; i< this->size; i++)
            {
                for (int j=0; j<this->size; j++)
                {
                    
                    if (!bridge || bridges==NULL) 
                        b=false;
                    else
                    {
                        if (bridges->isEdge(i,j)==true)
                            b=true;
                    }
                    if (this->isEdge ( i,j ) == true)
                    {                        
                        bool isBridge=false;
                        if (b) 
                        {
                            if (bridges->isEdge ( i,j ))
                                isBridge=true;
                        }
                        
                        if (found==NULL)
                            fprintf (f, "  \"%s\" -> \"%s\" [color=%s]\n", nodeID->name(i), nodeID->name(j), (!isBridge ? "black" : "red")); // 005a46
                        else
                        {
                            unsigned short int port1, port2;
                            unsigned long int i1, i2;
                            unsigned int ip, ip2;
                            
                            i1=found->getValue(i);
                            i2=found->getValue(j);
                            ip = i1 >> 32; // higher 32 bit
                            port1 = i1 & 0xffff;          // lowest 16 bit
                            ip2 = i2 >> 32; // higher 32 bit
                            port2 = i2 & 0xffff;          // lowest 16 bit
                            
                            int n = nodeID->getIdByIp ( ip );
                            int m = nodeID->getIdByIp ( ip2 );
                            
                            fprintf (f, "  \"%s-%hu\" -> \"%s-%hu\" [color=%s]\n", nodeID->name(n), port1, nodeID->name(m), port2, (!isBridge ? "black" : "red")); // 005a46
                        }
                    }
                }
            }
            fprintf ( f, "}");
        }
        
        

        /**
         * \brief writes the edges into a dot-file for visualisation with GraphViz-Tool, UNDIRECTED
         * \param[in] f Filehandle to an opened, writeable, empty file
         * \param[in] nodeID Object containing the names of the nodes, for having named nodes
         * \param[in] weighted whether the weight should be written on the edges (not implemented)
         * \note www.graphviz.org
         **/
        void vis ( FILE* f, NodeIdentificator* nodeID, bool weighted = false )
        {      
            fprintf ( f, "graph {\n" );
            for (int i=0; i< this->size; i++)
            {
                for (int j=i; j<this->size; j++)
                {
                    if (this->isEdge ( i,j ) == true)
                        fprintf (f, "  \"%s\" -- \"%s\"\n", nodeID->name(i), nodeID->name(j));
                }
            }
            fprintf ( f, "}");
        }
        
        /**
         * \brief writes the edges into a dot-file for visualisation with GraphViz-Tool; DIRECTED GRAPH
         * \param[in] f Filehandle to an opened, writeable, empty file
         * \param[in] nodeID Object containing the names of the nodes, for having named nodes
         * \param[in] weighted whether the weight should be written on the edges (not implemented)
         * \note www.graphviz.org
         **/
        void vis_dir ( FILE* f, NodeIdentificator* nodeID, bool weighted = false )
        {      
            fprintf ( f, "digraph {\n" );
            for (int i=0; i< this->size; i++)
            {
                for (int j=0; j<this->size; j++)
                {
                    if (this->isEdge ( i,j ) == true)
                        fprintf (f, "  \"%s\" -> \"%s\"\n", nodeID->name(i), nodeID->name(j));
                }
            }
            fprintf ( f, "}");
        }
        
        static void printChainDecomp ( base::list<base::ArrayList<struct edge2*>*>* chains, NodeIdentificator* nodeId, base::ArrayList<long unsigned int>* found=NULL )
        {
            base::list_node<base::ArrayList<struct edge2*>*>* cnode = chains->getFirst();
            while (cnode != NULL)
            {
                base::ArrayList<struct edge2*>* chain = cnode->getValue();
                for (int i=0; i<chain->getSize(); i++)
                {
                    struct edge2* tmp = chain->getValue ( i );
                    if (found)
                    {
                        unsigned short int port1, port2;
                        unsigned long int i1, i2;
                        unsigned int ip, ip2;
                        
                        i1=found->getValue(tmp->start);
                        i2=found->getValue(tmp->target);
                        ip = i1 >> 32; // higher 32 bit
                        port1 = i1 & 0xffff;          // lowest 16 bit
                        ip2 = i2 >> 32; // higher 32 bit
                        port2 = i2 & 0xffff;          // lowest 16 bit
                        
                        int n = nodeId->getIdByIp ( ip );
                        int m = nodeId->getIdByIp ( ip2 );
                        
                        printf ("{\"%s-%hu\" -> \"%s-%hu\"},", nodeId->name(n), port1, nodeId->name(m), port2);
                    }
                    else
                        printf ("{ %s -> %s }, ", nodeId->name(tmp->start), nodeId->name(tmp->target) );
                }
                printf ("\n");  
                cnode = cnode->next;
            }
        }
}; 



}

#endif
