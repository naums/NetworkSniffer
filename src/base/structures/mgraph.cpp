
#include "mgraph.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "nodeIdentificator.h"

namespace base
{
    mgraph::mgraph( int num, int numdev )
    {
        this->size=num;
        this->payload = (pqueue<short unsigned int, struct edge*>**) malloc ( this->size * this->size * sizeof(pqueue<short unsigned int, struct edge*>**) );
        
        this->names = (char**) malloc ( num * sizeof(char**) );
        char** nm = this->names;
        
        this->ip = (struct _ipname*) malloc ( numdev * sizeof(struct _ipname) );
        this->ip_end = 0;
        
        for (int i=0; i<this->size; i++)
        {
            *nm = NULL;
            nm++;
        }
        
        for (int i=0; i<this->size*this->size; i++)
        {
            this->payload[i] = new pqueue<short unsigned int, struct edge*> (); 
        }
    }
    
    mgraph::~mgraph ()
    {
        for (int i=0; i<this->size*this->size; i++)
        {
            this->payload[i]->clean();
            delete this->payload[i]; 
        }
        
        free(this->names);
        free(this->payload);
    }
    
    bool mgraph::addEdge ( int i, int j, short unsigned int p, time_t time, suseconds_t micro )
    {
        double t = -1;
        if (time>-1)
        { 
            t = time + ((double) micro / 1000000);
        }
        i = (i<0 ? 0 : i);
        j = (j<0 ? 0 : j );
        
        if (i < size && j < size)
        { 
            struct edge* tmp=NULL;
            if (((tmp=this->payload[i*size + j]->extract ( p ))!=NULL))
            {
                if (time > 0)
                {
                    tmp->count++;
                    tmp->sum += t;
                    tmp->avg = tmp->sum / tmp->count;
                }
                return true;
            }
            
            tmp = new struct edge;
            tmp->port = p;
            tmp->count = t>0 ? 1 : 0;
            tmp->sum = t>0 ? t : 0;
            tmp->avg = t>0 ? t : 0;
            this->payload[i*size +j]->add( p, tmp );
            
            return true;
        }
        return false;
    }
    
    bool mgraph::addEdge_prop ( int i, int j, bool up )
    {
        i = (i<0 ? 0 : i);
        j = (j<0 ? 0 : j);
        
        if (i < size && j < size)
        { 
            struct edge* tmp=NULL;
            if (((tmp=this->payload[i*size + j]->extract ( 0 ))!=NULL))
            {
                tmp->count++;
                tmp->sum += (up ? 1 : 0);
                tmp->avg = ((float)tmp->sum / (float)tmp->count) * 100.f;
                return true;
            }
            
            tmp = new struct edge;
            tmp->port = 0;
            tmp->count = 1;
            tmp->sum = (up ? 1 : 0);
            tmp->avg = tmp->sum;
            this->payload[i*size +j]->add( 0, tmp );
            
            return true;
        }
        return false;
    }

    double mgraph::getTime ( int i, int j, unsigned short p )
    {
        if (i < size && j < size)
        { 
            struct edge* tmp=NULL;
            if (((tmp=this->payload[i*size + j]->extract ( p ))!=NULL))
            {
                return tmp->avg;
            }
        }
        return -1;
    }
    
    bool mgraph::isEdge ( int i, int j )
    {
        if (i < size && j < size)
        { 
            struct edge* tmp=NULL;
            if (((tmp=this->payload[i*size + j]->minimum ( ))!=NULL))
            {
                return true;
            }
        }
        return false;
    }
    
    bool mgraph::dfs ( base::graph<bool>* dfs_tree, base::graph<bool>* invisible, int* roots )
    {
        if (!dfs_tree || !invisible)
            return false;
    
        // !important start at 1, because the "outside"-node is irrelevant here
        for (int i=1; i<this->size; i++)
        {
            // bidirectional graph -> only the upper triangle of the matrix is needed
            for (int j=i+1; j<this->size; j++)
            {
                // NODE NUMBERS ARE ONE LOWER!!!!
                invisible->addEdge((i-1), (j-1), true, false);
            }
        }
            
        bool* visited = (bool*) malloc ( this->size*sizeof(bool) );
        for (int i=0; i<this->size; i++)
        {
            visited[i] = false;
        }
        
        bool allvisited = false;
        int nextNode=1;
        int rIndex=0;
        while (!allvisited)
        {        
            roots[rIndex]=nextNode;
            rIndex++;
            
            this->dfs_inner ( nextNode, visited, dfs_tree, invisible );
            
            for (int i=1; i<this->size; i++)
            {
                if (visited[i]==false)
                {
                    nextNode=i;
                    break;
                }
            }
        }
        
        roots[rIndex]=-1;
        
        // Turn edges to have following attributes:
        //   - backedges (invisible) are directed away from the root
        //   - tree-edges are directed towards the root
        
        
        
        return true;
    }
    
    bool mgraph::dfs_inner ( int current, bool* visited, base::graph<bool>* dfs_tree, base::graph<bool>* invisible )
    {
        visited[current] = true;
        for (int j=1; j<this->size; j++)
        {
            if (visited[j] == true)
                continue;
            
            if (this->payload[current*this->size + j]->getSize() > 0)
            {
                invisible->removeEdge ( (current-1), (j-1) );
                dfs_tree->addEdge ( (current-1), (j-1), true );
                mgraph::dfs_inner ( j, visited, dfs_tree, invisible );
            }
        }
        
        return true;
    }
    
    void mgraph::vis_f ( const char* filename )
    {
        FILE* f = fopen ( filename, "w+" );
        this->vis (f, NULL);
        fclose (f);
    }
    
    void mgraph::vis ( FILE* f, NodeIdentificator* nodeId )
    {        
        fprintf ( f, "digraph {\n" );
        struct edge* tmp = NULL;
        for (int i=0; i< this->size; i++)
        {
            for (int j=0; j<this->size; j++)
            {
                for (int z=0; z<this->payload[i*this->size+j]->getSize(); z++)
                {
                    tmp = this->payload[i*this->size+j]->index ( z );
                    
                    fprintf ( f, "  \"%s\" -> \"%s\" [label=\"%hu; %.1f\"]\n", 
                              nodeId->name(i),nodeId->name(j), tmp->port, tmp->avg );
                    
                }
            }
        }
        fprintf ( f, "}");
    }
}

