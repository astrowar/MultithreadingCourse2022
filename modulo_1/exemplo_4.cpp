#include <cstdio>
#include <omp.h>

int main(){
printf("[out parallel], actual %d,  num of threads %d \n", omp_get_thread_num() , omp_get_num_threads());

#pragma omp parallel
     {        
          printf("hello world from %d  max:%d \n", omp_get_thread_num() , omp_get_num_threads());
     }
     
     return 0;
}