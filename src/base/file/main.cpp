#include "dir.h"
#include <stdio.h>
#include <stdlib.h>

using namespace base::file;

int main ( int argc, char* argv[] )
{
    Dir* d = Dir::open( argv[1] );
    const char* name;
    while ( (name = d -> next()) != NULL )
    {
        printf (" -->");
        if (d->type() == FT_DIR)
            printf (" DIR ");
        else if (d->type() == FT_REG)
            printf (" FIL ");
        else printf (" UNK ");
        printf (" %s \n", name);
    }
    d->close();
    
    return 0;
}
