#ifndef __MY_HEAP_SORT_H__
#define __MY_HEAP_SORT_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "myType.h"

void heapSort(MY_TYPE aArray[], uint32_t aElementCnt, myCmpFunc *aCmpCb);

#endif
