#include "myHeap.h"

struct myHeap
{
    myHeapType    mType;
    myCmpFunc    *mCmpCb;

    uint32_t      mElementCnt;
    uint32_t      mArraySize; /* unit : sizeof(MY_TYPE) */
    MY_TYPE      *mArray;
};

int32_t myHeapCreate(myHeap **aHeap, myCmpFunc *aCmpCb, myHeapType aType, uint32_t aInitialHeapSize)
{
    myHeap *sHeap = NULL;

    sHeap = (myHeap *)malloc(sizeof(myHeap));
    if (sHeap == NULL)
    {
        return -1;
    }
    else
    {
    }

    sHeap->mType       = aType;
    sHeap->mElementCnt = 0;
    sHeap->mCmpCb      = aCmpCb;
    sHeap->mArraySize  = aInitialHeapSize;

    sHeap->mArray = (MY_TYPE *)malloc(sizeof(MY_TYPE) * sHeap->mArraySize);
    if (sHeap->mArray == NULL)
    {
        free(sHeap);
        return -1;
    }
    else
    {
    }

    *aHeap = sHeap;

    return 0;
}

void myHeapDestroy(myHeap *aHeap)
{
    assert(aHeap->mElementCnt == 0);

#ifdef MY_DEBUG
    memset(aHeap->mArray, 0xDE, aHeap->mArraySize * sizeof(MY_TYPE));
#endif
    free(aHeap->mArray);

#ifdef MY_DEBUG
    memset(aHeap, 0xDE, sizeof(myHeap));
#endif
    free(aHeap);
}

static int32_t myHeapExpandArray(myHeap *aHeap)
{
    size_t   sNewArraySize;
    MY_TYPE *sTempArray;

    sNewArraySize = aHeap->mArraySize * 2;

    sTempArray = (MY_TYPE *)realloc(aHeap->mArray, sNewArraySize);

    if (sTempArray == NULL)
    {
        return -1;
    }
    else
    {
        aHeap->mArray     = sTempArray;
        aHeap->mArraySize = (uint32_t)sNewArraySize;
        return 0;
    }
}

static void myHeapSwapObjects(myHeap *aHeap, uint32_t aParentIndex, uint32_t aChildIndex)
{
    MY_TYPE sTmpObject;

    sTmpObject                  = aHeap->mArray[aParentIndex];
    aHeap->mArray[aParentIndex] = aHeap->mArray[aChildIndex];
    aHeap->mArray[aChildIndex]  = sTmpObject;
}

static void myHeapBubbleUp(myHeap *aHeap, uint32_t aStartIndex)
{
    uint32_t  sParentIndex;
    MY_TYPE   sParentObject;

    uint32_t  sChildIndex;
    MY_TYPE   sChildObject;

    if (aStartIndex == 0)
    {
        return;
    }
    else
    {
    }

    sParentIndex  = (aStartIndex - 1) / 2;
    sParentObject = aHeap->mArray[sParentIndex];

    sChildIndex   = aStartIndex;
    sChildObject  = aHeap->mArray[sChildIndex];

    if (aHeap->mType == MY_HEAP_TYPE_MAX)
    {
        /* MAX heap */

        if ((*aHeap->mCmpCb)(sParentObject, sChildObject) == MY_LESS)
        {
            myHeapSwapObjects(aHeap, sParentIndex, sChildIndex);
            myHeapBubbleUp(aHeap, sParentIndex);
        }
        else
        {
            return;
        }
    }
    else
    {
        /* MIN heap */

        if ((*aHeap->mCmpCb)(sParentObject, sChildObject) == MY_GREATER)
        {
            myHeapSwapObjects(aHeap, sParentIndex, sChildIndex);
            myHeapBubbleUp(aHeap, sParentIndex);
        }
        else
        {
            return;
        }
    }
}

void myHeapPushObject(myHeap *aHeap, MY_TYPE aObjectToPush)
{
    if (aHeap->mElementCnt >= aHeap->mArraySize)
    {
        assert(myHeapExpandArray(aHeap) == 0);
    }
    else
    {
    }

    aHeap->mArray[aHeap->mElementCnt] = aObjectToPush;

    myHeapBubbleUp(aHeap, aHeap->mElementCnt);

    aHeap->mElementCnt++;
}

static void myHeapBubbleDown(myHeap *aHeap, uint32_t aStartIndex);

static myCmpRc gBubbleDownCondition[] =
{
    MY_LESS,       /* MAX heap */
    MY_GREATER     /* MIN heap */
};

/*
 * aCondition is MY_LESS when the heap is MAX heap
 *               MY_GREATER when the heap is MIN heap
 */
static void myHeapBubbleDownInternal(myHeap    *aHeap,
                                     uint32_t   aParentIndex,
                                     uint32_t   aLeftChildIndex,
                                     uint32_t   aRightChildIndex,
                                     myCmpRc    aCondition)
{
    MY_TYPE sParentObject = aHeap->mArray[aParentIndex];
    MY_TYPE sLeftChildObject;
    MY_TYPE sRightChildObject;

    if (aLeftChildIndex >= aHeap->mElementCnt)
    {
        return;
    }
    else
    {
    }

    sLeftChildObject = aHeap->mArray[aLeftChildIndex];

    if ((*aHeap->mCmpCb)(sParentObject, sLeftChildObject) == aCondition)
    {
        if (aRightChildIndex >= aHeap->mElementCnt)
        {
            /* biggest is LeftChild */
            myHeapSwapObjects(aHeap, aParentIndex, aLeftChildIndex);
            myHeapBubbleDown(aHeap, aLeftChildIndex);
        }
        else
        {
            sRightChildObject = aHeap->mArray[aRightChildIndex];

            if ((*aHeap->mCmpCb)(sLeftChildObject, sRightChildObject) == aCondition)
            {
                /* biggest is RightChild */
                myHeapSwapObjects(aHeap, aParentIndex, aRightChildIndex);
                myHeapBubbleDown(aHeap, aRightChildIndex);
            }
            else
            {
                /* biggest is LeftChild */
                myHeapSwapObjects(aHeap, aParentIndex, aLeftChildIndex);
                myHeapBubbleDown(aHeap, aLeftChildIndex);
            }
        }
    }
    else
    {
        if (aRightChildIndex >= aHeap->mElementCnt)
        {
            return;
        }
        else
        {
            sRightChildObject = aHeap->mArray[aRightChildIndex];

            if ((*aHeap->mCmpCb)(sParentObject, sRightChildObject) == aCondition)
            {
                /* biggest is RightChild */
                myHeapSwapObjects(aHeap, aParentIndex, aRightChildIndex);
                myHeapBubbleDown(aHeap, aRightChildIndex);
            }
            else
            {
                /* biggest is Parent */
                return;
            }
        }
    }
}

static void myHeapBubbleDown(myHeap *aHeap, uint32_t aStartIndex)
{
    uint32_t  sParentIndex;
    uint32_t  sLeftChildIndex;
    uint32_t  sRightChildIndex;

    if (aHeap->mElementCnt <= aStartIndex)
    {
        return;
    }
    else
    {
    }

    sParentIndex      = aStartIndex;
    sLeftChildIndex   = sParentIndex * 2 + 1;
    sRightChildIndex  = sLeftChildIndex + 1;

    myHeapBubbleDownInternal(aHeap,
                             sParentIndex,
                             sLeftChildIndex,
                             sRightChildIndex,
                             gBubbleDownCondition[aHeap->mType]);
}

int32_t myHeapPopObject(myHeap *aHeap, MY_TYPE *aPoppedObject)
{
    if (aHeap->mElementCnt > 0)
    {
        *aPoppedObject   = aHeap->mArray[0];
        aHeap->mArray[0] = aHeap->mArray[aHeap->mElementCnt - 1];

        aHeap->mElementCnt--;

        myHeapBubbleDown(aHeap, 0);
    }
    else
    {
        return -1;
    }

    return 0;
}

void myHeapIterate(myHeap *aHeap, myHeapIterateCb *aCallback)
{
    uint32_t i;

    for (i = 0; i < aHeap->mElementCnt; i++)
    {
        (void)(*aCallback)(aHeap->mArray[i]);
    }
}

