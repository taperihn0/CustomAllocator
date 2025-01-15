#include "linalloc.h"
#include <sys/mman.h>

void arena_init(Arena* arena, void* buffer, size_t size) {
	arena->pbuff = (BYTE*)buffer;
	arena->offset = 0;
	arena->length = size;
	arena->prev_offset = 0;
}

void prealloc_arena(Arena* arena, size_t size) {
	memset(arena, 0, sizeof(arena));
	arena->pbuff = mmap(NULL, size, 
						PROT_READ | PROT_WRITE, 
						MAP_PRIVATE | MAP_ANON,
						0, 0);
	arena->offset = 0;
	arena->length = size;
}

void free_arena(Arena* arena) {
	munmap(arena->pbuff, arena->length);
	memset(arena, 0, sizeof(arena));
}

void free_arena_state(Arena* arena) {
	arena->offset = 0;
	arena->prev_offset = 0;
}

size_t align_offset(size_t offset) {
	size_t mod = offset & (DEFAULT_ALIGNMENT - 1);
	return mod ? offset + (DEFAULT_ALIGNMENT - mod) : offset;
}

void* arena_alloc(Arena* arena, size_t size) {
	if (arena->offset + size >= arena->length || size == 0)
		return NULL;

	size_t aligned_offset = align_offset(arena->offset);
	arena->prev_offset = aligned_offset;
	BYTE* res = arena->pbuff + aligned_offset;
	arena->offset = aligned_offset + size;

	return (void*)res;
}

void* arena_realloc_prev(Arena* arena, size_t size) {
	if (arena->prev_offset + size >= arena->length)
		return NULL;

	BYTE* res = arena->pbuff + arena->prev_offset;
	arena->offset = arena->prev_offset + size;
	return (void*)res;
}

void* arena_realloc(Arena* arena, void* ptr, size_t old_size, size_t new_size) {
	BYTE* old_mem = (BYTE*)ptr;

	if (old_mem == NULL || new_size == 0)
		return NULL;
	else if (old_mem - arena->pbuff == arena->prev_offset) {
		return arena_realloc_prev(arena, new_size);
	} else if (arena->pbuff <= old_mem && old_mem < arena->pbuff + arena->prev_offset) {
		BYTE* new_mem = (BYTE*)arena_alloc(arena, new_size);
	
		if (new_mem == NULL)
			return NULL;

		size_t cp_size = MIN(new_size, old_size);
		memmove(new_mem, old_mem, cp_size);
		return (void*)new_mem;
	} 

	return NULL;
}