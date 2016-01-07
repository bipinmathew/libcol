#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "col.h"

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
void test_uint_range(void)
{
    unsigned int value;
    col_uint *idx;
    col_uint_init(&idx);
    col_uint_range(idx,2,10000,3);

    col_uint_sum(idx,&value);
    CU_ASSERT(16665000==value);
    col_uint_free(idx);

}


void test_int_range(void)
{
    int value;

    col_int *idx;
    col_int_init(&idx);
    col_int_range(idx,-10,10000,3);

    col_int_sum(idx,&value);
    CU_ASSERT(16664978==value);
    col_int_free(idx);

}



void test_uint_min(void)
{
    unsigned int value;
    unsigned int a1,an,d,N,ev;
    N = 100;
    a1 = 2;
    d = 3;
    an = a1+(N-1)*d;
    col_uint *idx;
    col_uint_init(&idx);
    col_uint_range(idx,a1,an,d);

    col_uint_min(idx,&value);
    CU_ASSERT(a1==value);
    col_uint_free(idx);

}

void test_uint_max(void)
{
    unsigned int value;
    unsigned int a1,an,d,N,ev;
    N = 100;
    a1 = 2;
    d = 3;
    an = a1+(N-1)*d;
    col_uint *idx;
    col_uint_init(&idx);
    col_uint_range(idx,a1,an,d);

    col_uint_max(idx,&value);
    CU_ASSERT(an==value);
    col_uint_free(idx);

}




void test_int_min(void)
{
    int value;
    unsigned int d,N;
    int a1,an,ev;

    N = 100;
    a1 = -10;
    d = 3;

    an = a1+(N-1)*d;
    col_int *idx;
    col_int_init(&idx);
    col_int_range(idx,a1,an,d);

    col_int_min(idx,&value);
    CU_ASSERT(a1==value);
    col_int_free(idx);

}

void test_int_max(void)
{
    int value;
    unsigned int d,N;
    int a1,an,ev;

    N = 100;
    a1 = -10;
    d = 3;

    an = a1+(N-1)*d;
    col_int *idx;
    col_int_init(&idx);
    col_int_range(idx,a1,an,d);

    col_int_max(idx,&value);
    CU_ASSERT(an==value);
    col_int_free(idx);

}





void testRAND(void)
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



/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", NULL, NULL);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test of col_uint_range", test_uint_range)) || 
       (NULL == CU_add_test(pSuite, "test of col_int_range", test_int_range))   ||
       (NULL == CU_add_test(pSuite, "test of col_uint_min", test_uint_min ))   ||
       (NULL == CU_add_test(pSuite, "test of col_int_min", test_int_min  ))    || 
       (NULL == CU_add_test(pSuite, "test of col_uint_max", test_uint_max ))   ||
       (NULL == CU_add_test(pSuite, "test of col_int_max", test_int_max  )) 
       )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

