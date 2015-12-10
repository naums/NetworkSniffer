/****
 * Developed by Stefan Naumann, 2014
 * email: snaumann2912@yahoo.de
**/

#include <cstdlib>
#include "xml.h"

#include "string/cstring.h"

namespace base { namespace parser {

    dom_node* xml::xpath ( const char* path )
    {
        return NULL;
    }

    xml* xml::parsexml( const char* filename )
    {
        dom* d=new dom();
        
        
        
        xml* x=new xml(d);    
        return x;
    }
        
    char* xml::text()
    {
        return NULL;
    }
    
    void xml::write( const char* filename )
    {
    
    }
    
    dom_node* dom::xpath ( const char* path )
    {
        return NULL;
    }
}}
