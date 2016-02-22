#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "col_errors.h"


/**
 * @brief A structure to hold columns of type int.
 */
typedef struct col_int{
    unsigned int numrows; /**< number of rows in column */
    unsigned int _allocated; /**< number of rows allocated */
    int min;
    int max;
    int *d; /**< pointer to the actual data */
} col_int;


/**
 * @brief A structure to hold columns of type boolean.
 */
typedef struct col_boolean{
    unsigned int numrows; /**< number of rows in column */
    unsigned int _allocated; /**< number of rows allocated */
    unsigned char *d; /**< pointer to the actual data */
} col_boolean;


/**
 * @brief A structure to hold columns of type int.
 */
typedef struct col_uint{
    unsigned int numrows; /**< number of rows in column */
    unsigned int _allocated; /**< number of rows allocated */
    unsigned int min;
    unsigned int max;
    int sorted;
    int uniq;
    unsigned int *d; /**< pointer to the actual data */
} col_uint;



/**
 * @brief A structure to hold columns of type double.
 */
typedef struct col_double{
    unsigned int numrows; /**< number of rows in column */
    double *d; /**< pointer to the actual data */
} col_double;

/**
 * @brief A structure to hold columns of type str.
 */
typedef struct col_str{
    unsigned int _numrows; /**< internal count of number of rows in column */
    unsigned int numrows; /**<  actual number of rows in column */
    unsigned int _numbytes; /**< internal count of bytes in column */
    unsigned int numbytes; /**< total number of bytes loaded */
    unsigned int *offset; /**< array containing offsets to the end of each string. For example d[offset[i]-1] is the last character of the ith record for this column of strings.*/
    unsigned char *d; /**< pointer to the actual data */
} col_str;


const char * col_get_error_string(col_error num);


col_error col_int_init(col_int **p ) __attribute__((warn_unused_result));
void col_int_free(col_int *arr);
int col_int_get(const col_int *arr, unsigned int num, int *value);
col_error col_int_set(col_int *arr, unsigned int num, int value) __attribute__((warn_unused_result));
col_error col_int_sum(const col_int *arr, int *output);
int col_int_disp(col_int *arr);
int col_int_length(const col_int *arr, unsigned int *len);
col_error col_int_rand(col_int *arr,const col_uint *idx, int min, int max, unsigned int num) __attribute__((warn_unused_result));
col_error col_int_range(col_int *arr, int l, int r, int step) __attribute__((warn_unused_result));
int col_int_min(const col_int *arr, int *value);
int col_int_max(const col_int *arr, int *value);
col_error col_int_subset_assign_scalar(col_int *arr, const col_uint * idx, int value);
col_error col_int_select_scalar (const col_int * arr, col_uint * idx, int value);




col_error col_uint_init(col_uint **p ) __attribute__((warn_unused_result));
void col_uint_free(col_uint *arr);
int col_uint_get(const col_uint *arr, unsigned int num, unsigned int *value);
col_error col_uint_set(col_uint *arr, unsigned int num, unsigned int value) __attribute__((warn_unused_result));
int col_uint_sum(const col_uint *arr, unsigned int *output);
int col_uint_disp(col_uint *arr);
int col_uint_length(const col_uint *arr, unsigned int *len);
col_error col_uint_range(col_uint *arr, unsigned int l, unsigned int r, unsigned int step);
int col_uint_min(const col_uint *arr, unsigned int *value);
int col_uint_max(const col_uint *arr, unsigned int *value);
col_error col_uint_subset_assign_scalar (col_uint * arr, const col_uint * idx, unsigned int value);
col_error col_uint_select_scalar (const col_uint * arr, col_uint * idx, unsigned int value);



col_error col_boolean_init(col_boolean **p ) __attribute__((warn_unused_result));
void col_boolean_free(col_boolean *arr);



int col_double_init(col_double **p ) __attribute__((warn_unused_result));

int col_double_realloc(col_double *arr,unsigned int numrows);

void col_double_free(col_double *arr);

void col_str_free(col_str *arr);

int col_str_init(col_str **p) __attribute__((warn_unused_result));

int col_str_realloc(col_str *arr,unsigned int numrows);

#endif
