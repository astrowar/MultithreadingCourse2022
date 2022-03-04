#include "WorkVisualizer.hpp"
#include <omp.h>
 
int main() {
	const int works_x = 20;
	const int works_y = 32;
	WorkVisualizer wv(640, 640, works_x, works_y);

loop_inicio:
	wv.reset();

	int i,j;
#pragma omp parallel for schedule(guided),  private(i)  
	for ( j = 0; j < works_y; ++j)
		for (i = 0; i < works_x; ++i)		
		{
	       wv.touch(i, j);			
		}	
	
	if (wv.quit) return 0 ;
	goto loop_inicio;
	return 0;
}


