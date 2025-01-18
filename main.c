#include "linalloc.h"
#include "stackalloc.h"

int main() {	
	Stack* stack = malloc(sizeof(Stack));
	
	stack_prealloc(stack, _STACK_PREALLOC_SIZE);
	
	int* p = stack_alloc(stack, _STACK_PREALLOC_SIZE - DEFAULT_ALIGNMENT - 1);

	*p = 1;

	free_stack_all(stack);

	free(stack);
}