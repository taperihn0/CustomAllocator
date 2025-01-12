#pragma once
#include "common.h"

#ifndef PREALLOC_DEFAULT_SIZE
#	define PREALLOC_DEFAULT_SIZE 1024u
#endif

typedef struct _Arena {
	BYTE*  pbuff;
	size_t offset;
	size_t length; 
} Arena;

void arena_init(Arena* arena, void* buffer, size_t size);
void prealloc_arena(Arena* arena, size_t size);
void free_arena(Arena* arena);
void free_arena_state(Arena* arena);

void* arena_alloc(Arena* arena, size_t size);