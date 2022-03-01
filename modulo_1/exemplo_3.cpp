#include <cstdio>
#include <omp.h>

int main(){
#pragma omp parallel
     {        
          printf("hello world from %d \n", omp_get_thread_num());
     }
     
     return 0;
}