#include "col.h"
#include "gperftools/profiler.h"
int main (void){
  col_int *a;
  int i;
  if(NO_ERROR!=col_int_init(&a)){
    return(1);
  }


  ProfilerStart("/tmp/profile_col_int_range.cpu");
  for(i=0;i<10000;i++){
    if(NO_ERROR!=col_int_range(a,0,10000000,1)){
      return(1);
    }
  }
  ProfilerStop();

  return(0);


}
