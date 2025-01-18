#include "poolalloc.h"
#include <sys/mman.h>

void create_chunk_list(Pool* pool) {
	size_t chunk_count = pool->length / pool->chunk_size;

	for (UINT i = 0; i < chunk_count; i++) {
		Node* next = (Node*)(pool->pbuff + i * pool->chunk_size);
		next->next = pool->head;
		pool->head = next;
	}
}

void pool_init_copy(Pool* pool, void* buffer, size_t size, size_t chunk_size) {
	_assert_buffer_validity(buffer, size);

	pool_prealloc(pool, size, chunk_size);
	memcpy(pool->pbuff, buffer, size);
}

void pool_init_external(Pool* pool, void* buffer, size_t size, size_t chunk_size) {
	_assert_buffer_validity(buffer, size);

	pool->pbuff = (BYTE*)buffer;
	pool->length = size;
	pool->chunk_size = chunk_size;
	pool->head = NULL;

	create_chunk_list(pool);
}

void pool_prealloc(Pool* pool, size_t size, size_t chunk_size) {
	pool->pbuff = mmap(NULL, size, 
					   PROT_READ | PROT_WRITE, 
					   MAP_PRIVATE | MAP_ANON,
					   0, 0);
	pool->length = size;
	pool->chunk_size = chunk_size;
	pool->head = NULL;

	create_chunk_list(pool);
}

void pool_free_all(Pool* pool) {
	munmap(pool->pbuff, pool->length);
	memset(pool, 0, sizeof(Pool));
}

void pool_free(Pool* pool, void* ptr) {
	if (ptr == NULL || !(pool->pbuff <= (BYTE*)ptr && (BYTE*)ptr <= pool->pbuff + pool->length))
		return;

	Node* next = (Node*)ptr;
	next->next = pool->head;
	pool->head = next;
}

void* pool_alloc(Pool* pool) {
	if (pool->head == NULL)
		return NULL;

	void* res = (void*)pool->head;
	pool->head = pool->head->next;
	return res;
}
