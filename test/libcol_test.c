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
    col_uint_range(idx,2,10,3);

    col_uint_sum(idx,&value);
    CU_ASSERT(15==value);
    col_uint_free(idx);

}


void test_int_range(void)
{
    int value;

    col_int *idx;
    col_int_init(&idx);
    col_int_range(idx,-2,10,3);

    col_int_sum(idx,&value);
    CU_ASSERT(20==value);
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
    col_uint *idx;

    col_uint_init(&idx);
    col_uint_range(idx,2,100,3);
    col_uint_max(idx,&value);
    CU_ASSERT(98==value);
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



void test_col_int_subset_assign_scalar(void)
{
    int value, ret;
    unsigned int num;

    col_int *arr;
    col_uint *idx;
    int sum;

    col_int_init(&arr);
    col_uint_init(&idx);

    col_int_range(arr,0,10,1);
    col_int_length(arr,&num);

    col_uint_range(idx,0,2*num,3);
    ret = col_int_subset_assign_scalar(arr,idx,1);
    col_uint_range(idx,1,2*num,3);
    CU_ASSERT(0==col_int_subset_assign_scalar(arr,idx,-1)); 

    col_int_sum(arr,&sum);
    CU_ASSERT(15==sum);
    col_int_free(arr);
    col_uint_free(idx);

}

void test_col_int_select_scalar(void)
{
    int value, ret;
    unsigned int num;

    col_int *arr;
    col_uint *idx;
    int sum;

    col_int_init(&arr);
    col_uint_init(&idx);

    col_int_range(arr,0,10,1);
    col_int_length(arr,&num);

    col_uint_range(idx,0,num,2);
    CU_ASSERT(0 == col_int_subset_assign_scalar(arr,idx,1));

    col_int_select_scalar(arr,idx,1);

    col_int_free(arr);
    col_uint_free(idx);

}


void test_col_uint_subset_assign_scalar(void)
{
    int value, ret;
    unsigned int num;

    col_uint *arr;
    col_uint *idx;
    int sum;

    col_uint_init(&arr);
    col_uint_init(&idx);

    col_uint_range(arr,0,10,1);
    col_uint_length(arr,&num);

    col_uint_range(idx,0,2*num,3);
    ret = col_uint_subset_assign_scalar(arr,idx,1);
    col_uint_range(idx,1,2*num,3);
    CU_ASSERT(0==col_uint_subset_assign_scalar(arr,idx,-1)); 

    col_uint_sum(arr,&sum);
    CU_ASSERT(15==sum);
    col_uint_free(arr);
    col_uint_free(idx);

}

void test_col_uint_select_scalar(void)
{
    int value, ret;
    unsigned int num;

    col_uint *arr;
    col_uint *idx;
    int sum;

    col_uint_init(&arr);
    col_uint_init(&idx);

    col_uint_range(arr,0,10,1);
    col_uint_length(arr,&num);

    col_uint_range(idx,0,num,2);
    CU_ASSERT(0 == col_uint_subset_assign_scalar(arr,idx,1));

    col_uint_select_scalar(arr,idx,1);

    col_uint_free(arr);
    col_uint_free(idx);

}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite intSuite = NULL;
   CU_pSuite uintSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   intSuite = CU_add_suite("col_int Tests.", NULL, NULL);
   uintSuite = CU_add_suite("col_uint Tests.", NULL, NULL);
   if ((NULL == intSuite)||(NULL==uintSuite)) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (
        (NULL == CU_add_test(intSuite, "test of col_int_range", test_int_range))   ||
        (NULL == CU_add_test(intSuite, "test of col_int_min", test_int_min  ))    || 
        (NULL == CU_add_test(intSuite, "test of col_int_select_scalar", test_col_int_select_scalar ))   ||
        (NULL == CU_add_test(intSuite, "test of col_int_subset_assign_scalar", test_col_int_subset_assign_scalar ))   ||
        (NULL == CU_add_test(intSuite, "test of col_int_max", test_int_max  )) 
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }



   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (
        (NULL == CU_add_test(uintSuite, "test of col_uint_range", test_uint_range)) || 
        (NULL == CU_add_test(uintSuite, "test of col_uint_min", test_uint_min ))   ||
        (NULL == CU_add_test(uintSuite, "test of col_uint_max", test_uint_max ))   ||
        (NULL == CU_add_test(uintSuite, "test of col_uint_select_scalar", test_col_uint_select_scalar ))   ||
        (NULL == CU_add_test(uintSuite, "test of col_uint_subset_assign_scalar", test_col_uint_subset_assign_scalar ))
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
