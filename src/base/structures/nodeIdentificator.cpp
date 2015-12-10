#include "nodeIdentificator.h"

#include <cstdio>
#include <cstring>

    NodeIdentificator::NodeIdentificator ( int numHost, int numDev )
    {
        this->size=numHost;
        this->names = (char**) malloc ( (numHost+1) * sizeof(char*) );
        char** nm = this->names;
        
        this->ip = (struct _ipname*) malloc ( numDev * sizeof(struct _ipname) );
        this->ip_end = 0;
        
        for (int i=0; i<=numHost; i++)
        {
            *nm = NULL;
            nm++;
        }
    }
    
    NodeIdentificator::~NodeIdentificator ()
    {
        for (int i=1; i<=this->size; i++)
        {
            free(this->names[i]);
        }
        free(this->names);
        free(this->ip);
    }

    bool NodeIdentificator::addName ( int i, char* name )
    {
        if (this->names[i] == NULL)
        {
            this->names[i] = name;
            return true;
        }
        this->names[i] = name;
        return false;
    }
    
    const char* NodeIdentificator::name ( int i )
    {
        if (i==0)
            return "outside\0";
        if (this->size<i)
            return "";
        if (this->names[i]!=NULL)
        {
            return ((const char*) this->names[i]);
        }
        return "\0";
    }
    
    void NodeIdentificator::debugNames ()
    {
        printf (" >> 0 -- <<Outside>>\n");
        for (int i=1; i<this->size; i++)
        {
            printf (" >> %d -- %s\n", i, this->names[i]);
        }
    }
    
    
    void NodeIdentificator::addDevice ( int i, unsigned int ip )
    {
        struct _ipname tmp;
        tmp.ip = ip;
        tmp.nodenumber = i;
        this->ip[this->ip_end] = tmp;
        this->ip_end ++;
    }
    
    unsigned int NodeIdentificator::getIpByIndex ( int i )
    {
        if (i <= -1 )
            return 0;
        
        if (i>this->ip_end)
            return 0;
        
        return this->ip[i].ip;
        
    }
    
    unsigned int NodeIdentificator::getIpById ( int id )
    {
        struct _ipname* tmp;
        for (int i=0; i<this->ip_end; i++)
        {
            if (this->ip[i].nodenumber == id)
            {
                tmp=&this->ip[i];
                break;
            }
        }
        
        if (tmp!=NULL)
            return tmp->nodenumber;
        return 0;
        
    }
    
    int NodeIdentificator::getIdByIp ( unsigned int ip )
    {
        struct _ipname* tmp = this->getIpName ( ip, this->ip_end / 2, this->ip_end/2.0 );
        if ( tmp != NULL)
        {
            return tmp->nodenumber;
        }
        return 0;
    }
    
    struct _ipname* NodeIdentificator::getIpName ( unsigned int ip, int current, int stepsize )
    {
        for (int i=0; i<this->ip_end; i++)
        {
            if (this->ip[i].ip == ip)
                return &this->ip[i];
        }
        return NULL;
    }
