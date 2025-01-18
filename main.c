#include "linalloc.h"
#include "stackalloc.h"
#include "poolalloc.h"

int main() {	
	Pool* pool = malloc(sizeof(Pool));
	
	pool_prealloc(pool, _POOL_PREALLOC_SIZE, 8);
	
	int* p = (int*)pool_alloc(pool);

	*p = 1;
	*(p + 1) = 2;

	pool_free_all(pool);

	free(pool);
}