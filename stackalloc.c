#include "stackalloc.h"

typedef struct _AllocHeader {
	uint8_t alignment;
} Header;

void stack_init(Stack* stack, void* buffer, size_t size) {
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

void free_stack_all(Stack* stack) {
	munmap(stack->pbuff, stack->length);
	memset(stack, 0, sizeof(Stack));
}

size_t align_offset(size_t offset) {
	size_t mod = offset & (DEFAULT_ALIGNMENT - 1);
	return offset + (DEFAULT_ALIGNMENT - mod);
}

void free_stack(Stack* stack, void* ptr) {
	if (ptr == NULL || !(stack->pbuff <= (BYTE*)ptr && (BYTE*)ptr < stack->pbuff + stack->length))
		return;

	ASSERT(sizeof(Header) == sizeof(BYTE));

	BYTE* fptr = (BYTE*)ptr;
	size_t curr_offset = fptr - stack->pbuff;
	
	Header* header = (Header*)(stack->bpuff + curr_offset - 1);
	
	stack->offset = curr_offset - 1 - header->alignment;
}

void* stack_alloc(Stack* stack, size_t size) {
	if (stack->offset + size >= stack->length || size == 0)
		return NULL;

	ASSERT(sizeof(Header) == sizeof(BYTE));

	size_t aligned_offset = align_offset(stack->offset);

	Header* header = (Header*)(stack->pbuff + aligned_offset - 1);
	header->alignment = aligned_offset - stack->offset - 1;
	
	stack->offset = aligned_offset + size;

	return stack->pbuff + aligned_offset;
}

void* stack_realloc(Stack* stack, void* ptr, size_t old_size, size_t new_size) {
	BYTE* old_mem = (BYTE*)ptr;

	if (old_mem == NULL || new_size == 0)
		return NULL;
	else if (stack->pbuff <= old_mem && old_mem < stack->pbuff + stack->offset) {
		BYTE* new_mem = (BYTE*)stack_alloc(stack, new_size);
	
		if (new_mem == NULL)
			return NULL;

		size_t cp_size = MIN(new_size, old_size);
		memmove(new_mem, old_mem, cp_size);
		return (void*)new_mem;
	} 

	return NULL;
}