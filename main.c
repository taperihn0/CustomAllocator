#include "linalloc.h"
#include <stdio.h>

int main() {	
	Arena arena;
	prealloc_arena(&arena, PREALLOC_DEFAULT_SIZE);
	
	int* p = arena_alloc(&arena, sizeof(int) * 7);

	for (int i = 0; i < 7; i++) 
		p[i] = i * i;

	int *pr = arena_realloc_prev(&arena, sizeof(int) * 8);

	if (pr == NULL) {
		printf("pr = NULL\n");
		return 1;
	}

	for (int i = 0; i < 8; i++) {
		printf("%d ", pr[i]);
	}

	pr[7] = 100;

	putchar('\n');
	
	pr = arena_realloc(&arena, pr, sizeof(int) * 8, sizeof(int) * 9);
	
	pr[8] = 120;

	for (int i = 0; i < 9; i++)
		printf("%d ", pr[i]);

	putchar('\n');

	free_arena(&arena);
}