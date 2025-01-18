#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BYTE char

#ifndef DEFAULT_ALIGNMENT
#	define DEFAULT_ALIGNMENT sizeof(void*)
#endif

#ifndef CACHELINE_SIZE
#	define CACHELINE_SIZE sysconf(_SC_LEVEL1_DCACHE_LINESIZE)
#endif

#ifndef UINT
#	define UINT unsigned int
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static inline int _assert_error_terminate(const char* err) {
	printf("%s\n", err);
	exit(1);
	return 1;
}

#define ASSERT(val, err) (void)((val) || _assert_error_terminate((err)))

static inline void _assert_buffer_validity(void* buffer, size_t size) {
	ASSERT(buffer != NULL && size > 0, "Invalid buffer");
}