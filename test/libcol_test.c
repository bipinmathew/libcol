#include <stdio.h>
#include <string.h>
#include "col.h"

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
    col_int *p;
    col_uint *idx;

    col_int_init(&p);
    col_uint_init(&idx);

    col_uint_range(idx,0,100,2);
    col_int_rand(p,NULL,0,10,100);
    col_int_disp(p);

    col_int_rand(p,idx,0,10,100);
    col_int_disp(p);


    col_int_free(p);
    col_uint_free(idx);

}
