#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "col.h"


/* Start Integer functions */
int col_int_init(col_int **p ){
    *p = NULL;
    if((*p = (col_int *)malloc(sizeof(col_int)))==NULL){
        return 1;
    }
    (*p)->d = NULL;
    (*p)->numrows = 0;
    (*p)->min = INT_MAX;
    (*p)->max = INT_MIN;
    col_int_realloc(*p, 1);
    return 0;
}


int col_int_get(const col_int *arr, unsigned int num, int *value){
    *value = arr->d[num];
    return 0;
}

int col_int_set(col_int *arr, unsigned int i, int value){
    unsigned int allocated,length;
    int rc;
    col_int_getallocated(arr,&allocated);
    while(allocated<i){
        if(0<(rc=col_int_realloc(arr,2*allocated))){
            return rc;
        }
        col_int_getallocated(arr,&allocated);
    }

    col_int_getlength(arr,&length);
    if(i>length){
        col_int_setlength(arr,i);
    }

    arr->d[i] = value;
    if(arr->min > value)
        arr->min = value;
    else if(arr->max<value)
        arr->max = value;
    return 0;
}

int col_int_sum(const col_int *arr, int *output){
    unsigned int len,i;
    int v;
    col_int_getlength(arr,&len);
    *output = 0;
    for(i=0;i<len;i++){
       col_int_get(arr,i,&v);
       *output+=v;
    }
    return 0;
}


/* int col_int_eq(const col_int *arr, const col_int *arr_out, int val){
    unsigned int len,i,j;
    int v;
    col_int_getlength(arr,&len);
    *output = 0;
    for(i=0;i<len;i++){
       col_int_get(arr,i,&v);
       if(val==v){
           col_int_set(arr_out,j++,i);
       }
    }
    return 0;
} */


void col_int_free(col_int *arr){
    free(arr->d);
    free(arr);
}


int col_int_realloc(col_int *arr,unsigned int allocate){
    /* unsigned int s; 
    s = arr->allocate; */
    if(NULL == (arr->d = realloc(arr->d,allocate*sizeof(int)))){
      return 1;
    }
    arr->_allocated=allocate;
    /* memset(&arr->d[s],0,(numrows-s)*sizeof(int)); */
    return 0;
}


int col_int_getallocated(const col_int *arr, unsigned int *len){
  *len = arr->_allocated;
  return 0;
}

int col_int_getlength(const col_int *arr, unsigned int *len){
  *len = arr->numrows;
  return 0;
}

int col_int_setlength(col_int *arr, unsigned int len){
  arr->numrows=len;
  return 0;
}


int col_int_rand(col_int *arr,const col_uint *idx, unsigned int min, unsigned int max, unsigned int num){
    unsigned int i;
    unsigned int idx_len,v;
    if(idx==NULL){
      for(i=0;i<num;i++){
          col_int_set(arr,i,min+(rand()%(max-min+1)));
      }
    }
    else{
      col_uint_getlength(idx,&idx_len);
      for(i=0;i<num;i++){
          col_uint_get(idx,i%idx_len,&v);
          col_int_set(arr,v,min+(rand()%(max-min+1)));
      }
    }
    return 0;
}


int col_int_disp(col_int *arr){
    unsigned int i;
    int v;
    printf(" ");
    for(i=0;i<arr->numrows;i++){
        col_int_get(arr,i,&v);
        printf(" %d",v);
    }
    printf("\n");
    return 0;
}

int col_int_range(col_int *arr, signed int l, signed int r, unsigned int step){
    unsigned int i,N;
    N = (0<(r-l)%step)+(unsigned int)((r-l)/step);
    for(i=0;i<=N;i++){
        col_int_set(arr,i,l+i*step);
    }
    return 0;
}



int col_int_min(const col_int *arr, int *value){
    *value = arr->min;
    return 0;
}


int col_int_max(const col_int *arr, int *value){
    *value = arr->max;
    return 0;
}

/* End Integer functions */

/* Start uint functions */


int col_uint_init(col_uint **p ){
    *p = NULL;
    *p = (col_uint *)malloc(sizeof(col_uint));
    if(*p==NULL){
        return 1;
    }
    (*p)->d = NULL;
    (*p)->numrows = 0;
    (*p)->min = UINT_MAX;
    (*p)->max = 0;
    col_uint_realloc(*p, 1);
    return 0;
}


int col_uint_get(const col_uint *arr, unsigned int num, unsigned int *value){
    *value = arr->d[num];
    return 0;
}

int col_uint_sum(const col_uint *arr, unsigned int *output){
    unsigned int len,i;
    unsigned int v;
    col_uint_getlength(arr,&len);
    *output = 0;
    for(i=0;i<len;i++){
       col_uint_get(arr,i,&v);
       *output+=v;
    }
    return 0;
}

int col_uint_set(col_uint *arr, unsigned int i, unsigned int value){
    int rc;
    unsigned int allocated,length;
    col_uint_getallocated(arr,&allocated);
    while(allocated<i){
        if(0<(rc=col_uint_realloc(arr,2*allocated))){
            return rc;
        }
        col_uint_getallocated(arr,&allocated);
    }
    col_uint_getlength(arr,&length);
    if(i>length){
        col_uint_setlength(arr,i);
    }

    arr->d[i] = value;
    if(arr->min > value)
        arr->min = value;
    else if(arr->max<value)
        arr->max = value;
    return 0;
}


void col_uint_free(col_uint *arr){
    free(arr->d);
    free(arr);
}


int col_uint_realloc(col_uint *arr,unsigned int allocate){
    /* unsigned int s;
    s = arr->allocate; */
    arr->d = (unsigned int *)realloc(arr->d,allocate*sizeof(unsigned int));
    if(arr->d==NULL){
      return 1;
    }
    arr->_allocated=allocate;
    /* memset(&arr->d[s],0,(numrows-s)*sizeof(unsigned int)); */
    return 0;
}

int col_uint_getallocated(const col_uint *arr, unsigned int *len){
  *len = arr->_allocated;
  return 0;
}

int col_uint_getlength(const col_uint *arr, unsigned int *len){
  *len = arr->numrows;
  return 0;
}

int col_uint_setlength(col_uint *arr, unsigned int len){
  arr->numrows=len;
  return 0;
}


int col_uint_disp(col_uint *arr){
    unsigned int i;
    unsigned int v;
    printf(" ");
    for(i=0;i<arr->numrows;i++){
        col_uint_get(arr,i,&v);
        printf(" %d",v);
    }
    printf("\n");
    return 0;
}

int col_uint_range(col_uint *arr, unsigned int l, unsigned int r, unsigned int step){
    unsigned int i,N;
    N = (0<(r-l)%step)+(unsigned int)((r-l)/step);
    for(i=0;i<=N;i++){
        col_uint_set(arr,i,l+i*step);
    }
    return 0;
}


int col_uint_min(const col_uint *arr, unsigned int *value){
    *value = arr->min;
    return 0;
}


int col_uint_max(const col_uint *arr, unsigned int *value){
    *value = arr->max;
    return 0;
}



/* End uint functions */


int col_double_init(col_double **p ){
    if((*p = (col_double *)malloc(sizeof(col_double)))==NULL){
        return 1;
    }
    (*p)->d = NULL;
    return 0;
}



int col_double_realloc(col_double *arr, unsigned int numrows){
    return NULL == (arr->d = realloc(arr->d,numrows*sizeof(double)));
}

void col_double_free(col_double *arr){
    free(arr->d);
    free(arr);
}

void col_str_free(col_str *arr){
    free(arr->offset);
    free(arr->d);
    free(arr);
}

int col_str_init(col_str **p ){
    if((*p = (col_str *)malloc(sizeof(col_str)))==NULL){
        return 1;
    }
    (*p)->d = NULL;
    (*p)->offset = NULL;
    (*p)->_numrows=0;
    (*p)->_numbytes=0;
    (*p)->numrows=0;
    (*p)->numbytes=0; 
    return 0;
}

int col_str_realloc(col_str *arr,unsigned int numrows){
    unsigned int avgstrlen;
    unsigned int _numbytes;

    if((arr->offset = realloc(arr->offset,numrows*sizeof(unsigned int)))==NULL){
        return 1;
    }
    arr->_numrows=numrows;
    if(arr->numrows==0)
        avgstrlen = 128;
    else
        avgstrlen = arr->numbytes / arr->numrows;

    _numbytes = numrows*avgstrlen*sizeof(char);
    if(arr->_numbytes < _numbytes){
        if((arr->d = realloc(arr->d,_numbytes))==NULL){
            return 1;
        }
        arr->_numbytes = _numbytes;
    }
    return 0;
}

