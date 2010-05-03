#include "myHeap.h"
#include "heapSort.h"

void heapSort(MY_TYPE aArray[], uint32_t aElementCnt, myCmpFunc *aCmpCb)
{
    uint32_t  i;
    myHeap   *sHeap;
    MY_TYPE   sPoppedElement;

    if (myHeapCreate(&sHeap, aCmpCb, MY_HEAP_TYPE_MIN, aElementCnt) < 0)
    {
        abort();
    }

    for (i = 0; i < aElementCnt; i++)
    {
        myHeapPushObject(sHeap, aArray[i]);
    }

    for (i = 0; i < aElementCnt; i++)
    {
        if (myHeapPopObject(sHeap, &sPoppedElement) < 0)
        {
            abort();
        }

        aArray[i] = sPoppedElement;
    }
}
