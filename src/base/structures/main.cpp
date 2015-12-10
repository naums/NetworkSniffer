#include "mgraph.h"

using namespace base;

int main ()
{
    mgraph* tmp = new mgraph ( 3 );
    tmp->addName ( 0, "suse\0");
    tmp->addName ( 1, "mint\0");
    tmp->addName ( 2, "raspbry\0");    
    
	tmp->addEdge(1,2,30, 10.0);
	tmp->addEdge(2,0, 50, 2.0);
	tmp->addEdge(0,1, 500, 2.0);
	tmp->addEdge(0,1, 505, 5.0);
	tmp->addEdge(0,1, 500, 7.0);

	tmp->vis_f("test.dot");

	delete tmp;
}
