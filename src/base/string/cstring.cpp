/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#include "cstring.h"

namespace base { namespace cstring {

    bool cstrcmp ( const char* one, const char* two, bool exact)
    {
        int i=0;
        for (; one[i] && two[i]; i++)
        {
            if (one[i]!=two[i])
            {
                return false;
            }
        }
        
        if (exact)
        {
            if ((one[i] && !two[i]) || (!one[i] && two[i]))
            {
                return false;
            }
        }
        return true;
    }
    
    int strlen ( const char* one )
    {
        int i=0;
        for (; one[i]; i++);
        return i;
    }
    
    void strncpy ( const char* src, char* target, int length )
    {
        for (int i=0; src[i] && i<length; i++)
        {
            target[i]=src[i];
        }
    }
    
    int atoi ( const char* src, int length )
    {
        if (length==-1) length = strlen(src)+1;
        int result=0;
        for (int i=0; src[i] && i<length; i++)
        {
            if (src[i]>='0' && src[i]<='9')
            {
                result*=10;
                result+=src[i]-'0';
            }
        }
        return result;
    }

}}
