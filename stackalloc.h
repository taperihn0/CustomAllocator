#pragma once
#include "common.h"

#ifndef _STACK_PREALLOC_SIZE
#	define _STACK_PREALLOC_SIZE 1024u
#endif

typedef struct _Stack {
	BYTE*  pbuff;
	size_t length;
	size_t offset;
} Stack;

void stack_init(Stack* stack, void* buffer, size_t size);
void stack_prealloc(Stack* stack, size_t size);
void free_stack_all(Stack* stack);
void free_stack(Stack* stack, void* ptr);

void* stack_alloc(Stack* stack, size_t size);
void* stack_realloc(Stack* stack, void* ptr, size_t old_size, size_t new_size);