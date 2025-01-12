#include "linalloc.h"
#include <sys/mman.h>

void arena_init(Arena* arena, void* buffer, size_t size) {
	arena->pbuff = (BYTE*)buffer;
	arena->offset = 0;
	arena->length = size;
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
}

size_t align_offset(size_t offset) {
	size_t mod = offset & (DEFAULT_ALIGNMENT - 1);
	return mod ? offset + (DEFAULT_ALIGNMENT - mod) : offset;
}

void* arena_alloc(Arena* arena, size_t size) {
	if (arena->offset + size >= arena->length)
		return NULL;

	size_t aligned_offset = align_offset(arena->offset);
	BYTE* res = arena->pbuff + aligned_offset;
	arena->offset = aligned_offset + size;

	return (void*)res;
}