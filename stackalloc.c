#include "stackalloc.h"
#include <sys/mman.h>

typedef struct _AllocHeader {
	uint8_t alignment;
} Header;

void stack_init_copy(Stack* stack, void* buffer, size_t size) {
	_assert_buffer_validity(buffer, size);

	stack_prealloc(stack, size);
	memcpy(stack->pbuff, buffer, size);
}

void stack_init_external(Stack* stack, void* buffer, size_t size) {
	_assert_buffer_validity(buffer, size);

	stack->pbuff = (BYTE*)buffer;
	stack->length = size;
	stack->offset = 0;
}

void stack_prealloc(Stack* stack, size_t size) {
	stack->pbuff = mmap(NULL, size, 
						PROT_READ | PROT_WRITE, 
						MAP_PRIVATE | MAP_ANON,
						0, 0);
	stack->offset = 0;
	stack->length = size;
}

void stack_free_all(Stack* stack) {
	munmap(stack->pbuff, stack->length);
	memset(stack, 0, sizeof(Stack));
}

size_t align_offset_forward(size_t offset) {
	size_t mod = offset & (DEFAULT_ALIGNMENT - 1);
	return offset + (DEFAULT_ALIGNMENT - mod);
}

void stack_free(Stack* stack, void* ptr) {
	if (ptr == NULL || !(stack->pbuff <= (BYTE*)ptr && (BYTE*)ptr < stack->pbuff + stack->length))
		return;

	ASSERT(sizeof(Header) == sizeof(BYTE), "Supporting only 1-byte header");

	BYTE* fptr = (BYTE*)ptr;
	size_t curr_offset = fptr - stack->pbuff;
	
	Header* header = (Header*)(stack->pbuff + curr_offset - 1);

	size_t new_offset = curr_offset - 1 - header->alignment;
	
	if (new_offset <= DEFAULT_ALIGNMENT)
		return; 

	stack->offset = new_offset;
}

void* stack_alloc(Stack* stack, size_t size) {
	if (size == 0)
		return NULL;

	ASSERT(sizeof(Header) == sizeof(BYTE), "Supporting only 1-byte header");

	size_t aligned_offset = align_offset_forward(stack->offset);

	if (aligned_offset + size >= stack->length)
		return NULL;

	Header* header = (Header*)(stack->pbuff + aligned_offset - 1);
	header->alignment = aligned_offset - stack->offset - 1;
	
	stack->offset = aligned_offset + size;

	return (void*)(stack->pbuff + aligned_offset);
}

void* stack_realloc(Stack* stack, void* ptr, size_t old_size, size_t new_size) {
	if (ptr == NULL || new_size == 0)
		return NULL;

	BYTE* old_mem = (BYTE*)ptr;

	if (stack->pbuff + DEFAULT_ALIGNMENT <= old_mem && old_mem < stack->pbuff + stack->offset) {
		BYTE* new_mem = (BYTE*)stack_alloc(stack, new_size);
	
		if (new_mem == NULL)
			return NULL;

		size_t cp_size = MIN(new_size, old_size);
		memcpy(new_mem, old_mem, cp_size);
		return (void*)new_mem;
	} 

	return NULL;
}