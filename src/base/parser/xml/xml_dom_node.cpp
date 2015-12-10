/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#include <cstdlib>
#include "xml.h"

#include "string/cstring.h"

namespace base { namespace parser {

void dom_node::addChild ( dom_node* obj )
{
    dom_node* tmp=this->child;
    if (tmp==NULL)
    {
        this->child=obj;
    }
    else
    {
        while (tmp->next!=NULL)
            tmp=tmp->next;
        tmp->next=obj;
        obj->prev=tmp;
    }
}

void dom_node::addSibling ( dom_node* obj )
{
    dom_node* tmp=this;
    while (tmp->next!=NULL)
        tmp=tmp->next;
    tmp->next=obj;
    obj->prev=tmp;
}

void dom_node::addAttr ( const char* name, const char* value )
{
    dom_node_attr* obj=new dom_node_attr(name, value, this);
    this->addAttrNode(obj);    
}

void dom_node::addAttrNode ( dom_node_attr* obj )
{
    dom_node_attr* tmp=this->attr;
    if (tmp==NULL)
    {
        this->attr=obj;
    }
    
    while (tmp->next!=NULL)
        tmp=tmp->next;
    
    tmp->next=obj;
}

dom_node* dom_node::search ( const char* name )
{
    if (cstring::cstrcmp(this->name, name))
    {
        return this;
    }
    dom_node* tmp=this->next->search(name);
    if (tmp!=NULL)
        return tmp;
    tmp=this->child->search(name);
    if (tmp!=NULL)
        return tmp;
    return NULL;
}

dom_node_attr* dom_node::getAttrNode ( const char* name )
{
    dom_node_attr* tmp=this->attr;
    while (tmp!=NULL)
    {
        if (cstring::cstrcmp(tmp->name, name))
            return tmp;
    }
    return NULL;    
}
char* dom_node::getAttrValue ( const char* name )
{
    dom_node_attr* tmp=this->getAttrNode(name);
    return tmp->getValue(); 
}

bool dom_node::hasAttr ( const char* name )
{
    dom_node_attr* tmp=this->getAttrNode(name);
    if (tmp!=NULL)
        return true;
    else
        return false;   
}
bool dom_node::isAttr ( const char* name, const char* val )
{
    dom_node_attr* tmp=this->getAttrNode(name);
    if (tmp==NULL)
        return false;
    if (cstring::cstrcmp(val, tmp->value))
        return true;
    return false;
}

dom_node* dom_node::nextSibling ( const char* name )
{
    dom_node* tmp=this->next;
    if (tmp==NULL) return NULL;
    if (cstring::cstrcmp(name, tmp->name))
        return tmp;
    else
    {
        if (tmp->next!=NULL)
            return this->next->nextSibling(name);
    }
    return NULL;
}
dom_node* dom_node::prevSibling ( const char* name )
{
    dom_node* tmp=this->prev;
    if (tmp==NULL) return NULL;
    if (cstring::cstrcmp(name, tmp->name))
        return tmp;
    else
    {
        if (tmp->prev!=NULL)
            return this->prev->prevSibling(name);
    }
    return NULL;
}

dom_node* dom_node::nextSibling ()
{
    if (this->next)
        return this->next;
    return NULL;
}

dom_node* dom_node::prevSibling ()
{
    if (this->prev)
        return this->prev;
    return NULL;
}

dom_node* dom_node::getChild ( const char* name )
{
    dom_node* tmp=this->child;
    if (tmp==NULL) return NULL;
    if (cstring::cstrcmp(name, tmp->name))
        return tmp;
    else
    {
        if (tmp->next!=NULL)
            return this->next->nextSibling(name);
    }
    return NULL;
}

dom_node* dom_node::xpath ( const char* path )
{
    return NULL;
}   

char* dom_node_attr::getValue()
{
    return this->value;
}

char* dom_node_attr::getAttrValue ( const char* name )
{
    if (cstring::cstrcmp(this->name, name))
    {
        return this->getValue();
    }
    else
    {
        if (this->next)
            return this->next->getAttrValue(name);
    }
    
    return NULL;
}

dom_node_attr* dom_node_attr::getAttrNode( const char* name )
{
    if (cstring::cstrcmp(this->name, name))
    {
        return this;
    }
    else
    {
        if (this->next)
            return this->next->getAttrNode(name);
    }
    return NULL;
}

dom_node_attr* parsexml ( const char* str, int length, dom_node* prnt )
{
    enum xml::attr_parser state=BEGIN;
    int begin=0;
    char* myname;
    char* myattr;
    dom_node_attr* myfirst=NULL;
    dom_node_attr* last=NULL;
    for (int i=0; str[i] && i<=length; i++)
    {
        if (str[i]==' ' && state==BEGIN)
        {
            begin=1+i;
            state=NAME;
        }
        else if (str[i]==' ' && state==ATTR)
        {
            begin=1+i;
            state=NAME;
            myattr=(char*) malloc ( i-begin*sizeof(char) );
            cstring::strncpy( &str[begin], myattr, i-begin );
            
            dom_node_attr* attr=new dom_node_attr(myname, myattr, prnt);
            if (last)
            {
                last->next=attr;
                last=attr;
            }
            if (!myfirst)
            {
                myfirst=attr;
                last=attr;
            }
            
            free(myname);
            free(myattr);
        }
        else if (str[i]=='=' && state==NAME)
        {
            begin=1+i;
            state=ATTR;
            myname=(char*) malloc ( i-begin*sizeof(char) );
            cstring::strncpy( &str[begin], myname, i-begin );
        }
        else if (str[i]=='>')
        {
            break;
        }
    }

    return myfirst;
}
        
}}
