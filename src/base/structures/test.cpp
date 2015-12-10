#include "arraylist.h"

#include <cstdio>

int main()
{
    base::ArrayList<int>* a = new base::ArrayList<int> ( );
    for (int i=0; i<1000; i++)
    {
        a->add(i);
    }
    
    for (int i=0; i<1000; i++)
    {
        printf ("-- %d\n", a->getValue(i));
    }
    
    delete a;
}
