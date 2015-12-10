/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#ifndef BASE_STRING_CSTRING
#define BASE_STRING_CSTRING

namespace base { namespace cstring {

    bool cstrcmp ( const char* one, const char* two, bool exact=true);
    int strlen ( const char* one );
    
    void strncpy ( const char* src, char* target, int length );
    int atoi ( const char* src, int length=-1 );
}}

#endif
