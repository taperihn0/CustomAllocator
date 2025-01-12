#pragma once
#include <string.h>

#define BYTE char

#ifndef DEFAULT_ALIGNMENT
#	define DEFAULT_ALIGNMENT sizeof(void*)
#endif

#ifndef CACHELINE_SIZE
#	define CACHELINE_SIZE sysconf(_SC_LEVEL1_DCACHE_LINESIZE)
#endif