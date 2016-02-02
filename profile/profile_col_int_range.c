#include "col.h"
#include <stdio.h>
#include <time.h>
int main (void){
  col_int *a;
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
      if(NO_ERROR!=col_int_init(&a)){
        return(1);
      }

      start = clock();

        if(NO_ERROR!=col_int_range(a,0,size,1)){
          return(1);
        }

      diff = clock()-start;
      range_msec += diff*1000 / CLOCKS_PER_SEC;



      start = clock();

        if(NO_ERROR!=col_int_sum(a,&sum)){
          return(1);
        }

      diff = clock()-start;
      sum_msec += diff*1000 / CLOCKS_PER_SEC;



      col_int_free(a);
    }

    printf("size: %d, range_avg_time: %f, sum_avg_time: %f\n",size,range_msec/NUMTRIALS,sum_msec/NUMTRIALS);
  }

  return(0);


}
