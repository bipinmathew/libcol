#ifndef __COL_ERRORS_H__
#define __COL_ERRORS_H__


#define LIBCOL_ERRORS \
 X(NO_ERROR, "No error.") \
 X(OUT_OF_MEMORY, "Failed to allocate memory.")


/* Append a NUM_ERRORS enum value to 
   get the number of errors defined
   if necessary
   */
#define X(a,b) a,
 typedef enum {LIBCOL_ERRORS NUM_ERRORS} col_error;
#undef X


#define X(a,b) b,
static const char* col_error_strings[] = {LIBCOL_ERRORS};
#undef X

#endif
