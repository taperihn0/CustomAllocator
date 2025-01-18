#pragma once
#include "common.h"

#ifndef _POOL_PREALLOC_SIZE
#	define _POOL_PREALLOC_SIZE 1024u
#endif

typedef struct Node Node;

struct Node {
	Node* next;
};

typedef struct _PoolMemory {
	BYTE*  pbuff;
	size_t length;
	size_t chunk_size;
	Node*  head;
} Pool;

void pool_init(Pool* pool, void* buffer, size_t size, size_t chunk_size);
void pool_prealloc(Pool* pool, size_t size, size_t chunk_size);
void pool_free_all(Pool* pool);
void pool_free(Pool* pool, void* ptr);

void* pool_alloc(Pool* pool);