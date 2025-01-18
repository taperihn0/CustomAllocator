#pragma once
#include "common.h"

#ifndef _ARENA_PREALLOC_DEFAULT_SIZE
#	define _ARENA_PREALLOC_DEFAULT_SIZE 1024u
#endif

typedef struct _Arena {
	BYTE*  pbuff;
	size_t offset;
	size_t length;
	size_t prev_offset;
} Arena;

void arena_init_copy(Arena* arena, void* buffer, size_t size);
void arena_init_external(Arena* arena, void* buffer, size_t size);
void arena_prealloc(Arena* arena, size_t size);
void arena_free_all(Arena* arena);
void arena_free_state(Arena* arena);

void* arena_alloc(Arena* arena, size_t size);
void* arena_realloc(Arena* arena, void* ptr, size_t old_size, size_t new_size);