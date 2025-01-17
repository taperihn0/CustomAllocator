#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BYTE char

#ifndef DEFAULT_ALIGNMENT
#	define DEFAULT_ALIGNMENT sizeof(void*)
#endif

#ifndef CACHELINE_SIZE
#	define CACHELINE_SIZE sysconf(_SC_LEVEL1_DCACHE_LINESIZE)
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))

inline int _assert_error_terminate(const char* err) {
	printf("%s\n", err);
	exit(1);
	return 1;
}

#define ASSERT(val, err) (void)((val) || _assert_error_terminate((err)))