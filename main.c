#include "linalloc.h"
#include <stdio.h>

int main() {
	printf("Hello, world!\n");
	
	Arena arena;
	prealloc_arena(&arena, PREALLOC_DEFAULT_SIZE);

	free_arena(&arena);
}