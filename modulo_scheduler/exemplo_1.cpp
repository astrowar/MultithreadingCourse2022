#include "WorkVisualizer.hpp"
#include <omp.h>
 
int main() {
	const int works_x = 32;
	const int works_y = 25;
	WorkVisualizer wv(640, 640, works_x, works_y);

loop_inicio:
	wv.reset();

	int i,j;
#pragma omp parallel for  schedule(dynamic,1)  collapse(2)
	for ( j = 0; j < works_y; ++j)
		for (i = 0; i < works_x; ++i)
		
		{
			wv.touch(i, j);
		}
	
	
	
	goto loop_inicio;
	return 0;
}


