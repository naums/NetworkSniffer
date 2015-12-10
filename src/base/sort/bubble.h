/****
 * Developed by Stefan Naumann, 2014
 * email: snaumann2912@yahoo.de
**/

#ifndef BASE_SORT_BUBBLE
#define BASE_SORT_BUBBLE

#include <cstdlib>

namespace base { namespace sort {

template <type t1> void _bubble ( t1* arr, int length )
{
    for (int i=0; i<length-1; i++)
    {
        if (arr[i]>arr[i+1])
        {
            t1 buff=arr[i];
            arr[i]=arr[i+1];
            arr[i+1]=buff;
        }
    }
}

}}

#endif
