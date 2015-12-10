#include <cstdio>

#include "parser/xml/xml.h"
#include "string/cstring.h"

int main ()
{
    base::parser::xml* myxml=base::parser::xml::parsexml("test.xml");
    
    delete myxml;
    return 0;
}
