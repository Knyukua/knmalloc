#include "knmalloc.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define HEAP_CAPACITY 640000
#define CHUNK_CAPACITY 640000

char heap[HEAP_CAPACITY];

typedef struct {
    void* start;
    size_t size;
} Chunk;

typedef struct {
    Chunk chunks[CHUNK_CAPACITY];
    size_t amount;
    size_t memory;
} Summary; // name to be changed

Summary allocated = {{0}, 0, 0};
Summary available = {{heap, HEAP_CAPACITY}, 1, HEAP_CAPACITY};

int findAllocatedChunk(void* ptr)
{
    for (int i = 0; i < allocated.amount; i++)
    {
        if (allocated.chunks[i].start == ptr)
        {
            return i;
        }
    }
    return -1;
}

int findAvailableChunk(size_t size)
{
    for (int i = 0; i < available.amount; i++)
    {
        if (available.chunks[i].size >= size)
        {
            return i;
        }
    }
    return -1;
}

void pushBackAllocatedChunk(Chunk chunk)
{
    allocated.chunks[allocated.amount++] = chunk;
    allocated.memory += chunk.size;
}

void addAvailableChunk(void* ptr, size_t size)
{
    if (available.amount == 0)
    {
        available.amount++;
        available.chunks[0].start = ptr;
        available.chunks[0].size = size;
        return;
    }

    for (int i = 0; i < available.amount; i++)
    {
        Chunk* current = &available.chunks[i];

        if ((uintptr_t)ptr + size == (uintptr_t)current->start)
        {
            current->start = ptr;
            current->size += size;
            return;
        }        
        if ((uintptr_t)ptr == (uintptr_t)current->start + current->size)
        {
            current->size += size;
            Chunk* next = &available.chunks[i + 1];
            if ((uintptr_t)current->start + current->size == (uintptr_t)next->start)
            {
                next->start = current->start;
                next->size += current->size;
                available.amount--;
                memmove(current, next, (available.amount - i) * sizeof(Chunk));
            }
            return;
        }
        if ((uintptr_t)ptr < (uintptr_t)current->start)
        {
            memmove(current + 1, current, (available.amount - i) * sizeof(Chunk));
            current->start = ptr;
            current->size = size;
            available.amount++;
            return;
        }
    }
}

size_t removeAllocatedChunk(int index)
{
    Chunk* chunk = &allocated.chunks[index];
    size_t ret = chunk->size;

    allocated.amount--;
    allocated.memory -= allocated.chunks[index].size;
    memmove(chunk, chunk + 1, (allocated.amount - index) * sizeof(Chunk));

    return ret;
}

void reduceAvailableChunk(int index, size_t size)
{
    Chunk* current = &available.chunks[index];

    current->start = (void*)((uintptr_t)current->start + size);
    current->size -= size;
    
    if (current->size == 0)
    {
        available.amount--;
        memmove(current, current + 1, (available.amount - index) * sizeof(Chunk));
    }
}

void* knmalloc(size_t size)
{
    if (size == 0) return NULL;

    int index = findAvailableChunk(size);
    if (index == -1) return NULL;

    Chunk chunk = {available.chunks[index].start, size};
    pushBackAllocatedChunk(chunk);
    reduceAvailableChunk(index, size);
    
    return chunk.start;
}

void knfree(void* ptr)
{
    if (ptr == NULL) return;

    int index = findAllocatedChunk(ptr);
    if (index == -1) return;
    size_t chunkSize = removeAllocatedChunk(index);
    addAvailableChunk(ptr, chunkSize);
}

void dumpAllocatedChunks()
{
    if (allocated.amount == 0) return;

    printf("Found %d unreleased chunks:\n", allocated.amount);
    for (int i = 0; i < allocated.amount; i++)
    {
        const Chunk* current = &allocated.chunks[i];
        printf(" -- Chunk at 0x%p with size %d\n", current->start, current->size);
    }
}

void dumpAvailableChunks()
{
    if (available.amount == 0) return;

    printf("Available chunks:\n");
    for (int i = 0; i < available.amount; i++)
    {
        const Chunk* current = &available.chunks[i];
        printf(" -- Chunk at 0x%p with size %d\n", current->start, current->size);
    }
}
