#include "linalloc.h"
#include <sys/mman.h>

void arena_init_copy(Arena* arena, void* buffer, size_t size) {
	_assert_buffer_validity(buffer, size);
	
	arena_prealloc(arena, size);
	memcpy(arena->pbuff, buffer, size);
}

void arena_init_external(Arena* arena, void* buffer, size_t size) {
	_assert_buffer_validity(buffer, size);

	arena->pbuff = (BYTE*)buffer;
	arena->offset = 0;
	arena->length = size;
	arena->prev_offset = 0;
}

void arena_prealloc(Arena* arena, size_t size) {
	arena->pbuff = mmap(NULL, size, 
						PROT_READ | PROT_WRITE, 
						MAP_PRIVATE | MAP_ANON,
						0, 0);
	arena->offset = arena->prev_offset = 0;
	arena->length = size;
}

void arena_free_all(Arena* arena) {
	munmap(arena->pbuff, arena->length);
	memset(arena, 0, sizeof(Arena));
}

void arena_free_state(Arena* arena) {
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

void* arena_realloc(Arena* arena, void* ptr, size_t old_size, size_t new_size) {
	BYTE* old_mem = (BYTE*)ptr;

	if (old_mem == NULL || new_size == 0)
		return NULL;
	else if (arena->pbuff <= old_mem && old_mem <= arena->pbuff + arena->prev_offset) {
		BYTE* new_mem = (BYTE*)arena_alloc(arena, new_size);
	
		if (new_mem == NULL)
			return NULL;

		size_t cp_size = MIN(new_size, old_size);
		memmove(new_mem, old_mem, cp_size);
		return (void*)new_mem;
	} 

	return NULL;
}