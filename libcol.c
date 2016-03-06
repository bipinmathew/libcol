#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <emmintrin.h>
#include "col.h"


/* private functions */

static col_error col_int__set (col_int * arr, unsigned int i, int value);
static col_error col_double__set (col_double * arr, unsigned int i, double value);
static col_error col_uint__realloc(col_uint *arr,unsigned int * numrows) __attribute__((warn_unused_result));
static col_error col_int__realloc(col_int *arr,unsigned int * numrows) __attribute__((warn_unused_result));
static col_error col_double__realloc(col_double *arr,unsigned int * numrows) __attribute__((warn_unused_result));
static int col_uint__getallocated(const col_uint *arr, unsigned int *len);
static int col_int__getallocated(const col_int *arr, unsigned int *len);
static int col_double__getallocated(const col_double *arr, unsigned int *len);
static int col_int__setlength(col_int *arr, unsigned int len);
static int col_uint__setlength(col_uint *arr, unsigned int len);
static int col_int__reset (col_int * p);
static int col_double__reset (col_double * p);
static int col_uint__reset (col_uint * p);
static col_error col_int_init__helper (col_int ** p, unsigned int allocate);
static col_error col_double_init__helper (col_double ** p, unsigned int allocate);


static int col_int__eq(int here, int there){
  return here==there;
}

static int col_int__lt(int here, int there){
  return here<there;
}

static int col_int__gt(int here, int there){
  return here>there;
}

static int col_int__lteq(int here, int there){
  return here<=there;
}

static int col_int__gteq(int here, int there){
  return here>=there;
}



/* end private functions */

/* Start universal functions */

const char * col_get_error_string(col_error num){
  return col_error_strings[num];
}

/* End type-less functions. */

/* Start Integer functions */

int
col_int__reset (col_int *p){
  p->numrows = 0;
  p->min = INT_MAX;
  p->max = INT_MIN;
  return 0;
}



col_error
col_int_init__helper (col_int ** p, unsigned int allocate)
{
  *p = NULL;

  if ((*p = (col_int *) malloc (sizeof (col_int))) == NULL)
    {
      return 1;
    }
  col_int__reset(*p);
  (*p)->d = NULL;
  (*p)->_allocated = 0;
  return col_int__realloc (*p, &allocate);
}


col_error
col_int_init (col_int ** p)
{
  *p = NULL;
  unsigned int allocate;

  allocate = 4096;
  return col_int_init__helper(p,allocate);
}


col_error
col_int_init_scalar (col_int ** p, int val)
{
  *p = NULL;
  unsigned int allocate;
  col_error ret_val;

  allocate = 1;
  if(0==(ret_val=col_int_init__helper(p,allocate))){
    ret_val = col_int_set(*p,0,val);
  }
  return ret_val;
}


int
col_int_get (const col_int * arr, unsigned int num, int *value)
{
  *value = arr->d[num];
  return 0;
}

col_error col_int__set(col_int *arr, unsigned int i, int value){
  arr->d[i] = value;
  return NO_ERROR;
}

col_error
col_int_set (col_int * arr, unsigned int i, int value)
{
  unsigned int allocated, length;
  col_error rc;
  col_int__getallocated (arr, &allocated);
  if(allocated < i){
    allocated=i;
    if (NO_ERROR != (rc = col_int__realloc (arr, &allocated))){
        return rc;
    }
  }

  col_int_length (arr, &length);
  if (i > length)
    {
      col_int__setlength (arr, i);
    }

  col_int__set(arr,i,value);
  if (arr->min > value)
    arr->min = value;
  else if (arr->max < value)
    arr->max = value;
  return NO_ERROR;
}

col_error
col_int_subset_assign_scalar (col_int * arr, const col_uint * idx, int value)
{
  unsigned int i, arr_len, idx_len, idx_val;
  unsigned int arr_allocated,idx_max;
  col_error e;

  col_int_length(arr,&arr_len);

  col_uint_max(idx,&idx_max);
  col_int__getallocated(arr,&arr_allocated);
  if(arr_allocated<idx_max){
    if(NO_ERROR!=(e=col_int__realloc(arr,&idx_max))){
      return e;
    }
  }
  
  if(arr_allocated>arr_len){
    memset(&arr->d[arr_len+1],0,(arr_allocated-arr_len-1)*sizeof(int));
  }
    

  col_uint_length(idx,&idx_len);
  for(i=0;i<=idx_len;i++){    
    col_uint_get(idx,i,&idx_val);
    if(NO_ERROR!=(e=col_int_set(arr,idx_val,value))){
      return e;
    }
  }

  return NO_ERROR;
}

col_error 
col_int_eq_scalar (const col_int * arr, col_uint * idx, int value){
  col_int_select_scalar (arr, idx, col_int__eq,value);
}

col_error 
col_int_lt_scalar (const col_int * arr, col_uint * idx, int value){
  col_int_select_scalar (arr, idx, col_int__lt,value);
}

col_error 
col_int_gt_scalar (const col_int * arr, col_uint * idx, int value){
  col_int_select_scalar (arr, idx, col_int__gt,value);
}

col_error 
col_int_lteq_scalar (const col_int * arr, col_uint * idx, int value){
  col_int_select_scalar (arr, idx, col_int__lteq,value);
}

col_error 
col_int_gteq_scalar (const col_int * arr, col_uint * idx, int value){
  col_int_select_scalar (arr, idx, col_int__gteq,value);
}


col_error 
col_int_select_scalar (const col_int * arr, col_uint * idx, int (*func)(int here,int there), int there){
  unsigned int i,k,arr_len;
  int val;
  col_error e;
  col_uint__reset(idx);
  col_int_length(arr,&arr_len);
  k=0;
  for(i=0;i<=arr_len;i++){
    col_int_get(arr,i,&val);
    if(func(val,there)){
      if(NO_ERROR != (e = col_uint_set(idx,k++,i))){
        return e;
      }
    }
  }
  return NO_ERROR;
}


col_error
col_int_sum (const col_int * arr, int *output)
{
  unsigned int len, i;
  int v;
  col_int_length (arr, &len);
  *output = 0;
  __m128i xmm0, accumulator;
  __m128i* src;
  static const unsigned int BLOCKSIZE=4;
  unsigned int remainder;
  int accint[BLOCKSIZE];

  /* If we have fewer than 1k numbers do naive computation. else
     use xmm0 */
  if(len<1024){
    for (i = 0; i <= len; i++){
      col_int_get (arr, i, &v);
      *output += v;
    }
  }
  else{
    remainder = len%BLOCKSIZE;

    src = (__m128i*)arr->d;
    accumulator = _mm_loadu_si128(src);

    for(i=BLOCKSIZE;i<len-remainder;i+=BLOCKSIZE){
      xmm0 = _mm_loadu_si128(++src);
      accumulator = _mm_add_epi32(accumulator,xmm0);
    }

    _mm_store_si128((__m128i*)accint,accumulator);

    for(i=0;i<BLOCKSIZE;i++){
      *output += accint[i];
    }
    for(i=len-remainder;i<=len;i++){
      *output += arr->d[i];
    }
  }
  return NO_ERROR;
}


void
col_int_free (col_int * arr)
{
  free (arr->d);
  free (arr);
}


col_error
col_int__realloc (col_int * arr, unsigned int * numrows)
{
  unsigned int allocate;


  for( allocate = 4096 ; allocate < (*numrows); allocate <<=1);


  if (NULL == (arr->d = realloc (arr->d, allocate * sizeof (int)))){
    return OUT_OF_MEMORY;
  }

  arr->_allocated = allocate;
  (*numrows) = allocate;
  return NO_ERROR;
}


int
col_int__getallocated (const col_int * arr, unsigned int *len)
{
  *len = arr->_allocated;
  return 0;
}

int
col_int_length (const col_int * arr, unsigned int *len)
{
  *len = arr->numrows;
  return 0;
}

int
col_int__setlength (col_int * arr, unsigned int len)
{
  arr->numrows = len;
  return 0;
}


col_error
col_int_rand (col_int * arr, const col_uint * idx, int min, int max, unsigned int num)
{
  unsigned int i;
  unsigned int idx_len, v;
  col_error e;
  if (idx == NULL)
    {
      for (i = 0; i < num; i++)
	{
	  if(NO_ERROR!=(e=col_int_set (arr, i, min + (rand () % (max - min + 1))))){
            return e;
          }
	}
    }
  else
    {
      col_uint_length (idx, &idx_len);
      for (i = 0; i < idx_len; i++)
	{
	  col_uint_get (idx, i , &v);
	  if(NO_ERROR!=(e=col_int_set (arr, v, min + (rand () % (max - min + 1))))){
            return e;
          }
	}
    }
  return NO_ERROR;
}


int
col_int_disp (col_int * arr)
{
  unsigned int i,numrows;
  int v;

  col_int_length(arr,&numrows);
  printf (" ");
  for (i = 0; i <= numrows; i++)
    {
      col_int_get (arr, i, &v);
      printf (" %d", v);
    }
  printf ("\n");
  return 0;
}

col_error
col_int_range (col_int * arr, int l, int r, int step)
{
  static const unsigned int BLOCKSIZE = 4;
  unsigned int i;
  unsigned int allocate;
  col_error rc;
  unsigned int num_iter;
  int last_val;
  __m128i* src;

  int xint[BLOCKSIZE];
  int mint[BLOCKSIZE];

  __m128i mask,xmm0;

  
  if(0>   (r-l)/step){
    return LIBCOL_INVALID_RANGE;
  }
  else{
    num_iter = (r-l)/step;
  }

  col_int__reset(arr);

  // Allocate at least the number of rows.
  allocate=num_iter;


  if (0 < (rc = col_int__realloc (arr, &allocate))){
    return rc;
  }



  for(i=0;i<BLOCKSIZE;i++){
    xint[i]=l+i*step;
    mint[i]=BLOCKSIZE*step;
    col_int__set(arr,i,xint[i]);
  }


  xmm0 = _mm_loadu_si128((__m128i*)xint);
  mask = _mm_loadu_si128((__m128i*)mint);
  src = (__m128i*) &arr->d[BLOCKSIZE];

  for(i=BLOCKSIZE;i<=num_iter;i+=BLOCKSIZE){
    xmm0 = _mm_add_epi32(xmm0,mask);
    _mm_store_si128(src,xmm0);
    ++src;
  }

  col_int__setlength(arr,num_iter);
  last_val = l+num_iter*step;

  arr->min = (step > 0 ) ? l : last_val;
  arr->max = (step > 0 ) ? last_val : l;
  return NO_ERROR;
}



int
col_int_min (const col_int * arr, int *value)
{
  *value = arr->min;
  return 0;
}


int
col_int_max (const col_int * arr, int *value)
{
  *value = arr->max;
  return 0;
}

/* End Integer functions */

/* Start uint functions */

int
col_uint__reset (col_uint *p){
  p->numrows = 0;
  p->min = UINT_MAX;
  p->max = 0;
  return 0;
}

col_error
col_uint_init (col_uint ** p)
{
  *p = NULL;

  unsigned int allocate;

  allocate = 4096;

  *p = (col_uint *) malloc (sizeof (col_uint));
  if (*p == NULL)
    {
      return 1;
    }
  col_uint__reset (*p);
  (*p)->d = NULL;
  (*p)->_allocated = 0;
  return col_uint__realloc (*p, &allocate);
}


int
col_uint_get (const col_uint * arr, unsigned int num, unsigned int *value)
{
  *value = arr->d[num];
  return 0;
}

int
col_uint_sum (const col_uint * arr, unsigned int *output)
{
  unsigned int len, i;
  unsigned int v;
  col_uint_length (arr, &len);
  *output = 0;
  for (i = 0; i <= len; i++)
    {
      col_uint_get (arr, i, &v);
      *output += v;
    }
  return 0;
}

col_error
col_uint_set (col_uint * arr, unsigned int i, unsigned int value)
{
  int rc;
  unsigned int allocated, length;
  col_uint__getallocated (arr, &allocated);


  if(allocated < i){
    allocated=i;
    if (NO_ERROR != (rc = col_uint__realloc (arr, &allocated))){
        return rc;
    }
  }


  col_uint_length (arr, &length);
  if (i > length)
    {
      col_uint__setlength (arr, i);
    }
  col_uint_length (arr, &length);

  arr->d[i] = value;
  if (arr->min > value)
    arr->min = value;
  else if (arr->max < value)
    arr->max = value;
  return NO_ERROR;
}

col_error
col_uint_subset_assign_scalar (col_uint * arr, const col_uint * idx, unsigned int value)
{

  unsigned int i, arr_len, idx_len, idx_val;
  unsigned int arr_allocated,idx_max;
  col_error e;

  col_uint_length(arr,&arr_len);

  col_uint_max(idx,&idx_max);
  col_uint__getallocated(arr,&arr_allocated);
  if(arr_allocated<idx_max){
    if(NO_ERROR!=(e=col_uint__realloc(arr,&idx_max))){
      return e;
    }
  }
  
  if(arr_allocated>arr_len){
    memset(&arr->d[arr_len+1],0,(arr_allocated-arr_len-1)*sizeof(unsigned int));
  }
    

  col_uint_length(idx,&idx_len);
  for(i=0;i<=idx_len;i++){    
    col_uint_get(idx,i,&idx_val);
    if(NO_ERROR!=(e=col_uint_set(arr,idx_val,value))){
      return e;
    }
  }

  return NO_ERROR;

}

col_error 
col_uint_select_scalar (const col_uint * arr, col_uint * idx, unsigned int value){
  unsigned int i,k,arr_len;
  unsigned int val;
  col_error e;
  col_uint__reset(idx);
  col_uint_length(arr,&arr_len);
  k=0;
  for(i=0;i<=arr_len;i++){
    col_uint_get(arr,i,&val);
    if(val==value){
      if(NO_ERROR!=(e=col_uint_set(idx,k++,i))){
        return e;
      }
    }
  }
  return 0;
}


void
col_uint_free (col_uint * arr)
{
  free (arr->d);
  free (arr);
}


col_error
col_uint__realloc (col_uint * arr, unsigned int * numrows)
{

  unsigned int allocate;


  for( allocate = 4096 ; allocate < (*numrows); allocate <<=1);


  if (NULL == (arr->d = realloc (arr->d, allocate * sizeof (unsigned int)))){
    return OUT_OF_MEMORY;
  }

  arr->_allocated = allocate;
  (*numrows) = allocate;
  return NO_ERROR;

}

int
col_uint__getallocated (const col_uint * arr, unsigned int *len)
{
  *len = arr->_allocated;
  return 0;
}

int
col_uint_length (const col_uint * arr, unsigned int *len)
{
  *len = arr->numrows;
  return 0;
}

int
col_uint__setlength (col_uint * arr, unsigned int len)
{
  arr->numrows = len;
  return 0;
}


int
col_uint_disp (col_uint * arr)
{
  unsigned int i;
  unsigned int v;
  unsigned int numrows;
  col_uint_length(arr,&numrows);
  printf (" ");
  for (i = 0; i <= numrows; i++)
    {
      col_uint_get (arr, i, &v);
      printf (" %d", v);
    }
  printf ("\n");
  return 0;
}

col_error
col_uint_range (col_uint * arr, unsigned int l, unsigned int r,
		unsigned int step)
{
  unsigned int i, v;
  col_error e;

  i = 0;
  col_uint__reset(arr);
  for(v=l;v<=r; v+=step){
    if(NO_ERROR!=(e=col_uint_set (arr, i++ , v))){
      return e;
    }
  }
  return NO_ERROR;
}


int
col_uint_min (const col_uint * arr, unsigned int *value)
{
  *value = arr->min;
  return 0;
}


int
col_uint_max (const col_uint * arr, unsigned int *value)
{
  *value = arr->max;
  return 0;
}



/* End uint functions */



/* Start Boolean functions */


int
col_boolean__reset (col_boolean *p){
  p->numrows = 0;
  return 0;
}


col_error col_boolean__set(col_int *arr, unsigned int i, int value){
  unsigned int byte;
  unsigned char bit;
  byte = i/8;
  bit = i%8;

  arr->d[i] = value;
  return NO_ERROR;
}


col_error
col_boolean__realloc (col_boolean * arr, unsigned int * numrows)
{
  unsigned int allocate;


  for( allocate = 4096 ; allocate < (*numrows); allocate <<=1);


  if (NULL == (arr->d = realloc (arr->d, 1+(allocate/8) ) )){
    return OUT_OF_MEMORY;
  }

  arr->_allocated = allocate;
  (*numrows) = allocate;
  return NO_ERROR;
}

col_error col_boolean_init(col_boolean **p ){
  *p = NULL;
  unsigned int allocate;

  allocate = 4096;
  if ((*p = (col_boolean *) malloc (sizeof (col_boolean))) == NULL)
    {
      return 1;
    }
  col_boolean__reset(*p);
  (*p)->d = NULL;
  (*p)->_allocated = 0;
  return col_boolean__realloc (*p, &allocate);
} 

void col_boolean_free(col_boolean *arr){
  free (arr->d);
  free (arr);
}

/* End Boolean functions */

int
col_double__reset (col_double *p){
  p->numrows = 0;
  p->min = INFINITY;
  p->max = -INFINITY;
  return 0;
}



col_error
col_double__realloc (col_double * arr, unsigned int * numrows)
{
  unsigned int allocate;


  for( allocate = 4096 ; allocate < (*numrows); allocate <<=1);


  if (NULL == (arr->d = realloc (arr->d, allocate * sizeof (double)))){
    return OUT_OF_MEMORY;
  }

  arr->_allocated = allocate;
  (*numrows) = allocate;
  return NO_ERROR;
}


col_error col_double_init__helper (col_double ** p, unsigned int allocate)
{
  *p = NULL;
  if ((*p = (col_double *) malloc (sizeof (col_double))) == NULL)
    {
      return 1;
    }

  col_double__reset(*p);
  (*p)->d = NULL;
  (*p)->_allocated = 0;
  return col_double__realloc (*p, &allocate);
}


col_error col_double_init (col_double ** p)
{
  *p = NULL;
  unsigned int allocate;

  allocate = 4096;
  return col_double_init__helper(p,allocate);
}


col_error
col_double_init_scalar (col_double ** p, double val)
{
  *p = NULL;
  unsigned int allocate;
  col_error ret_val;

  allocate = 1;
  if(0==(ret_val=col_double_init__helper(p,allocate))){
    ret_val = col_double_set(*p,0,val);
  }
  return ret_val;
}

int
col_double_realloc (col_double * arr, unsigned int numrows)
{
  return NULL == (arr->d = realloc (arr->d, numrows * sizeof (double)));
}

void
col_double_free (col_double * arr)
{
  free (arr->d);
  free (arr);
}


col_error col_double_set (col_double * arr, unsigned int i, double value)
{
  unsigned int allocated, length;
  col_error rc;
  col_double__getallocated (arr, &allocated);
  if(allocated < i){
    allocated=i;
    if (NO_ERROR != (rc = col_double__realloc (arr, &allocated))){
        return rc;
    }
  }

  col_double_length (arr, &length);
  if (i > length)
    {
      col_double__setlength (arr, i);
    }

  col_double__set(arr,i,value);
  if (arr->min > value)
    arr->min = value;
  else if (arr->max < value)
    arr->max = value;
  return NO_ERROR;
}

col_error col_double__set(col_double *arr, unsigned int i, double value){
  arr->d[i] = value;
  return NO_ERROR;
}


int
col_double__getallocated (const col_double * arr, unsigned int *len)
{
  *len = arr->_allocated;
  return 0;
}

int
col_double_length (const col_double * arr, unsigned int *len)
{
  *len = arr->numrows;
  return 0;
}

int
col_double__setlength (col_double * arr, unsigned int len)
{
  arr->numrows = len;
  return 0;
}





void
col_str_free (col_str * arr)
{
  free (arr->offset);
  free (arr->d);
  free (arr);
}

int
col_str_init (col_str ** p)
{
  if ((*p = (col_str *) malloc (sizeof (col_str))) == NULL)
    {
      return 1;
    }
  (*p)->d = NULL;
  (*p)->offset = NULL;
  (*p)->_numrows = 0;
  (*p)->_numbytes = 0;
  (*p)->numrows = 0;
  (*p)->numbytes = 0;
  return 0;
}

int
col_str_realloc (col_str * arr, unsigned int numrows)
{
  unsigned int avgstrlen;
  unsigned int _numbytes;

  if ((arr->offset =
       realloc (arr->offset, numrows * sizeof (unsigned int))) == NULL)
    {
      return 1;
    }
  arr->_numrows = numrows;
  if (arr->numrows == 0)
    avgstrlen = 128;
  else
    avgstrlen = arr->numbytes / arr->numrows;

  _numbytes = numrows * avgstrlen * sizeof (char);
  if (arr->_numbytes < _numbytes)
    {
      if ((arr->d = realloc (arr->d, _numbytes)) == NULL)
	{
	  return 1;
	}
      arr->_numbytes = _numbytes;
    }
  return 0;
}
