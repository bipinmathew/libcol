all: profile_col_int_range col_int_range_benchmark
profile_col_int_range: profile_col_int_range.c  
	gcc -O3 -I../ -L../ -o profile_col_int_range profile_col_int_range.c -lcol



col_int_range_benchmark: col_int_range_benchmark.c  
	gcc -O3 -I../ -L../ -o col_int_range_cachetest col_int_range_benchmark.c -lcol
