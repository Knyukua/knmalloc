#include "knmalloc.h"

int main()
{
    void* someMem1 = knmalloc(150000);
    void* someMemT = knmalloc(10000);
    void* someMem2 = knmalloc(160000);
    void* someMem3 = knmalloc(320000);

    knfree(someMem3);
    knfree(someMem1);
    knfree(someMem2);
    knfree(someMemT);

    dumpAllocatedChunks();
    dumpAvailableChunks();
}
