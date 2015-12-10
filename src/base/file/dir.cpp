#include "dir.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

namespace base { namespace file {

    Dir::Dir( DIR* hd )
    {
        this->hdir = hd;
        hfile = NULL;
    }

    Dir::~Dir()
    {
        closedir ( hdir );
    }

    const char* Dir::read()
    {
        while ((this->hfile = readdir(this->hdir)) != NULL)
        {
            return this->hfile->d_name;
        }
        return NULL;
    }
    
    const char* Dir::next ()
    {
        return this->read();
    }

    Dir* Dir::open( const char* filepath )
    {
        DIR* dp = opendir ( filepath );
        if (!dp) 
            return NULL;
        
        return new Dir ( dp );
    }
    
    enum filetype Dir::type ( )
    {
        return (enum filetype) this->hfile->d_type;
    }

    void Dir::close()
    {
        closedir ( hdir );
    }
            

}}
