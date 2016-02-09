#include "col.h"
#include <stdio.h>
#include <time.h>
int main (void){
  col_int *a;
  unsigned int size;

  size=1000000;
  col_int_init(&a);

  if(NO_ERROR!=col_int_range(a,0,size,1)){
    return(1);
  }

  col_int_free(a);

  return(0);
}
