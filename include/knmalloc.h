#ifndef KNMALLOC_H
#define KNMALLOC_H

#include <stddef.h>

void* knmalloc(size_t size);
void knfree(void* ptr);
void dumpAllocatedChunks();
void dumpAvailableChunks();

#endif // KNMALLOC_H
