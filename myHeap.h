#ifndef __MY_HEAP_H__
#define __MY_HEAP_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "myType.h"

typedef struct myHeap myHeap;

typedef enum
{
    MY_HEAP_TYPE_MAX,
    MY_HEAP_TYPE_MIN
} myHeapType;

/*
 * type definitions of callback functions
 */
typedef int32_t myHeapIterateCb(MY_TYPE aElement);

/*
 * Creating / Destroying instance
 */
int32_t  myHeapCreate(myHeap **aHeap, myCmpFunc *aCmpCb, myHeapType aType, uint32_t aInitialHeapSize);
void     myHeapDestroy(myHeap *aHeap);

/*
 * Heap functions
 */
void     myHeapPushObject(myHeap *aHeap, MY_TYPE aObjectToPush);
int32_t myHeapPopObject(myHeap *aHeap, MY_TYPE *aPoppedObject);

/*
 * Unitility functions
 */
void     myHeapIterate(myHeap *aHeap, myHeapIterateCb *aCallback);

#endif
