#include "linalloc.h"
#include <stdio.h>

int main() {	
	Arena arena;
	prealloc_arena(&arena, _ARENA_PREALLOC_DEFAULT_SIZE);
	free_arena_all(&arena);
}