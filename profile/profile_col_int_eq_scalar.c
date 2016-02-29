#include "col.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int flush_cache(){
   const int size = 20*1024*1024; // Allocate 20M. Set much larger then L2
   char *c;
   int i,j;
   c = (char *)malloc(size);
   for (i = 0; i < 0xffff; i++)
     for (j = 0; j < size; j++)
       c[j] = i*j;

   free(c);
}


int main (void){
  col_int *a;
  col_uint *idx;
  int i;
  unsigned int NUMTRIALS = 10;

  clock_t start, diff;
  double range_msec, sum_msec;
  unsigned int size;
  int sum;

  size=1000;

  for(size=1000;size<1000000000;size*=10){
    range_msec = 0;
    for(i=0;i<NUMTRIALS;i++){
      flush_cache();
      if(NO_ERROR!=col_int_init(&a)){
        return(1);
      }


      if(NO_ERROR!=col_uint_init(&idx)){
        return(1);
      }

      start = clock();

        if(NO_ERROR!=col_int_rand(a,NULL,0,10,size)){
          return(1);
        }

      diff = clock()-start;
      range_msec += diff*1000 / CLOCKS_PER_SEC;



      start = clock();

        if(NO_ERROR!=col_int_eq_scalar(a,idx,7)){
          return(1);
        }

      diff = clock()-start;
      sum_msec += diff*1000 / CLOCKS_PER_SEC;



      col_int_free(a);
      col_uint_free(idx);
    }

    printf("size: %d, col_int_rand avg_time: %f, col_int_eq_scalar avg_time: %f\n",size,range_msec/NUMTRIALS,sum_msec/NUMTRIALS);
  }

  return(0);


}
