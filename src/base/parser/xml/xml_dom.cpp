/****
 * Developed by Stefan Naumann, 2014
 * email: snaumann2912@yahoo.de
**/

#include <cstdlib>
#include "xml.h"

#include "string/cstring.h"

namespace base { namespace parser {

    dom_node* xml::search ( const char* name )
    {   
        if (this->tree)
            return this->tree->search(name);
        return NULL;
    }

    dom_node_attr* xml::getAttrNode ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->getAttrNode ( name );
        return NULL;
    }

    bool xml::hasAttr ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->hasAttr( name );
        return false;
    }

    bool xml::isAttr ( dom_node* elem, const char* name, const char* val )
    {
        if (elem)
            return elem->isAttr( name, val );
        return false;
    }

    dom_node* xml::nextSibling ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->nextSibling(name);
        return NULL;
    }
    
    dom_node* xml::prevSibling ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->prevSibling(name);
        return NULL;
    }

    dom_node* xml::child ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->getChild( name );
        return NULL;
    }
    
    void dom::changeRoot ( dom_node* rt )
    {
        this->root=rt;
    }
    
    dom_node* dom::search ( const char* name )
    {
        if (this->root)
            return this->root->search ( name );
        return NULL;
    }
    
    dom_node_attr* dom::getAttrNode ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->getAttrNode( name );
        return NULL;
    }
    
    bool dom::hasAttr ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->hasAttr ( name );
        return false;
    }
    
    bool dom::isAttr ( dom_node* elem, const char* name, const char* val )
    {
        if (elem)
            return elem->isAttr (name, val);
        return false;
    }
        
    dom_node* dom::nextSibling ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->nextSibling(name);
        return NULL;
    }
    
    dom_node* dom::prevSibling ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->prevSibling(name);
        return NULL;
    }
        
    dom_node* dom::child ( dom_node* elem, const char* name )
    {
        if (elem)
            return elem->getChild(name);
        return NULL;
    }
}}
