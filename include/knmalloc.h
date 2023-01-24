#ifndef KNMALLOC_H
#define KNMALLOC_H

#include <stddef.h>

/// @brief Allocates a memory chunk on the heap.
/// @param size the size of the memory chunk to be allocated.
/// @return On success returns pointer to the allocated memory chunk.
///         Returns NULL if size parameter is 0 or there is no big enough available chunk of memory.
void* knmalloc(size_t size);

/// @brief Frees the allocated memory chunk pointed by the ptr.
///        Does nothing if ptr is NULL or outside the "heap".
/// @param ptr pointer to the allocated memory chunk.
void knfree(void* ptr);

void dumpAllocatedChunks();
void dumpAvailableChunks();

#endif // KNMALLOC_H
