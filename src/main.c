#include <stdio.h>
#include <stdint.h>

#include "knmalloc.h"

uint32_t getUInt()
{
    uint32_t ret;
    while (!scanf("%u", &ret)) { while (getchar() != '\n'); }
    while (getchar() != '\n');
    return ret;
}

void fillSqMatrix(int** matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = i * size + j + 1;
        }
    }
}

void printSqMatrix(int** matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    printf("Input matrix size: ");
    uint32_t size = getUInt();

    int** matrix = (int**)knmalloc(size * sizeof(int*));
    for (int i = 0; i < size; i++)
        matrix[i] = (int*)knmalloc(size * sizeof(int));

    fillSqMatrix(matrix, size);
    printSqMatrix(matrix, size);

    for (int i = 0; i < size; i++)
        knfree(matrix[i]);
    knfree(matrix);

    dumpAllocatedChunks();
    dumpAvailableChunks();
}
