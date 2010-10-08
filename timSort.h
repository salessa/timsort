#ifndef __TIM_SORT_H__
#define __TIM_SORT_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "myType.h"

void timSort(MY_TYPE *aArray, size_t aElementCnt, size_t aWidth, myCmpFunc *aCmpCb);

#endif
