#include "number.h"
#include <cstdio>
#include <cstdlib>

namespace base {

    char* btohex ( const unsigned char byte )
    {
        char* res = (char* ) malloc ( 2 * sizeof(char));
        char table[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
        
        unsigned char lb = (byte) & 15;
        unsigned char hb = (byte>>4);
        
        res[1] = table[lb];
        res[0] = table[hb];
        
        /*putchar(res[0]); putchar(res[1]);
        putchar('\n');*/
        
        return res;    
    }
    
    char* btodec ( const unsigned char byte )
    {
        unsigned char buff = byte;
        char* res = (char*) malloc ( 4 * sizeof(char) );
        
        for (int i=2; i>=0; i--)
        {
            res[i] = '0' + (buff % 10);
            buff /= 10;
        }
        
        res[3] = '\0';
        
        return res;         
    }
    
}
