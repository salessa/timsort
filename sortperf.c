#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#include "heapSort.h"
#include "heapSortLibc.h"

#include "timSort.h"
#include "quickSort.h"
#include "mergeSortLibc.h"

static int32_t verifyArrayIsSorted(uint32_t *aArray, int32_t aCount)
{
    int32_t i;

    for (i = 1; i < aCount; i++)
    {
        if (*(aArray + i - 1) > *(aArray + i)) return -1;
    }

    return 0;
}

static myCmpRc compareFunc(MY_TYPE aElem1, MY_TYPE aElem2)
{
    if (aElem1 > aElem2)
    {
        return MY_GREATER;
    }
    else if (aElem1 < aElem2)
    {
        return MY_LESS;
    }
    else
    {
        return MY_EQUAL;
    }
}

static uint32_t *createArray(uint32_t aSize)
{
    uint32_t  sCursor = 0;
    uint32_t *sArray  = (uint32_t *)malloc(aSize * sizeof(uint32_t));

    if (sArray == NULL) abort();

    while (sCursor < aSize)
    {
        sArray[sCursor] = generateValue();
        sCursor++;
    }

    return sArray;
}

typedef struct testConf
{
    int32_t      mCount;
    int32_t      mDoVerify;

    myPattern    mPattern;

    const char  *mAlgorithmName;
    void       (*mSortFunc)(MY_TYPE [], uint32_t, myCmpFunc *);
} testConf;

static void testConfInit(testConf *aConf)
{
    aConf->mCount         = -1;
    aConf->mDoVerify      = -1;
    aConf->mAlgorithmName = NULL;
    aConf->mSortFunc      = NULL;
}

static void printUsageAndExit(char *aProgramName)
{
    (void)fprintf(stderr, "Usage : %s [ options ]\n"
                          "  -c NUM   element count\n"
                          "  -v       verify sorting result\n"
                          "  -a NAME  sorting algorithm name to use\n"
                          "           NAME would be one of "
                          "tim, heap, heaplibc, quick, mergelibc.\n"
                          , aProgramName);
    exit(1);
}

void processArg(int32_t aArgc, char *aArgv[], testConf *aConf)
{
    int32_t i;

    for (i = 1; i < aArgc; i++)
    {
        if (strcmp(aArgv[i], "-v") == 0)
        {
            /* -v */
            if (aConf->mDoVerify < 0)
            {
                aConf->mDoVerify = 1;
            }
            else
            {
                (void)fprintf(stderr, "error : an option cannot be specified more than once.\n");
                exit(1);
            }
        }
        else if (strcmp(aArgv[i], "-c") == 0)
        {
            /* -c */
            if (aConf->mCount < 0)
            {
                if (i + 1 < aArgc)
                {
                    int32_t  sCount;
                    char    *sEndPtr = NULL;
                    sCount = strtol(aArgv[i + 1], &sEndPtr, 10);
                    if (errno == ERANGE)
                    {
                        (void)fprintf(stderr, "error : the value provided with "
                                              "'%s' is out of range.\n", aArgv[i]);
                    }
                    else
                    {
                        if (*sEndPtr != '\0')
                        {
                            (void)fprintf(stderr, "error : option '%s' only accepts integer.\n", aArgv[i]);
                            exit(1);
                        }
                    }

                    aConf->mCount = sCount;
                    i++;
                }
                else
                {
                    (void)fprintf(stderr, "error : option '%s' needs to be "
                                          "provided with a value.\n", aArgv[i]);
                    exit(1);
                }
            }
            else
            {
                (void)fprintf(stderr, "error : an option cannot be specified more than once.\n");
                exit(1);
            }
        }
        else if (strcmp(aArgv[i], "-a") == 0)
        {
            /* -a */
            if (aConf->mSortFunc == NULL)
            {
                if (i + 1 < aArgc)
                {
                    i++;

                    if (strcmp(aArgv[i], "tim") == 0)
                    {
                        aConf->mSortFunc      = timSort;
                        aConf->mAlgorithmName = "timsort";
                    }
                    else if (strcmp(aArgv[i], "heap") == 0)
                    {
                        aConf->mSortFunc      = heapSort;
                        aConf->mAlgorithmName = "heapsort";
                    }
                    else if (strcmp(aArgv[i], "quick") == 0)
                    {
                        aConf->mSortFunc      = quickSort;
                        aConf->mAlgorithmName = "quicksort";
                    }
                    else if (strcmp(aArgv[i], "mergelibc") == 0)
                    {
                        aConf->mSortFunc      = mergeSortLibc;
                        aConf->mAlgorithmName = "mergesort of libc";
                    }
                    else if (strcmp(aArgv[i], "heaplibc") == 0)
                    {
                        aConf->mSortFunc      = heapSortLibc;
                        aConf->mAlgorithmName = "heapsort of libc";
                    }
                    else
                    {
                        (void)fprintf(stderr, 
                                      "error : %s is not a supported sorting algorithm name.\n"
                                      "        available sorting algorithm name : "
                                      "tim, heap, heaplibc quick, mergelibc\n",
                                      aArgv[i]);
                        exit(1);
                    }
                }
                else
                {
                    (void)fprintf(stderr, 
                                  "error : option '%s' needs to be "
                                  "provided with a sorting algorithm name.\n", aArgv[i]);
                    (void)fprintf(stderr,
                                  "        available sorting algorithm name : "
                                  "tim, heap, heaplibc, quick, mergelibc\n");
                    exit(1);
                }
            }
            else
            {
                (void)fprintf(stderr, "error : an option cannot be specified more than once.\n");
                exit(1);
            }
        }
        else if (strcmp(aArgv[i], "-p") == 0)
        {
            if (aConf->mSourceDataPattern == PATTERN_NONE)
            {
                if (i + 1 < aArgc)
                {
                    i++;

                    if (strcmp(aArgv[i], "random") == 0)
                    {
                        aConf->mSourceDataPattern = PATTERN_RANDOM;
                    }
                    else (strcmp(aArgv[i], "sinwave") == 0)
                    {
                        aConf->mSourceDataPattern = PATTERN_SINWAVE;
                    }
                    else
                    {
                    }
                }
                else
                {
                    (void)fprintf(stderr, 
                                  "error : option '%s' needs to be "
                                  "provided with a source data pattern.\n", aArgv[i]);
                    (void)fprintf(stderr,
                                  "        available source data patterns are : "
                                  "random, ascending, descending, saw");
                    exit(1);
                }
            }
            else
            {
                (void)fprintf(stderr, "error : an option cannot be specified more than once.\n");
                exit(1);
            }
        }
        else
        {
            printUsageAndExit(aArgv[0]);
        }
    }

    if (aConf->mDoVerify < 0) aConf->mDoVerify = 0; /* do not verify unless -v is provided */
    if (aConf->mCount < 0) aConf->mCount = MY_DEFAULT_ELEMENT_CNT;
    if (aConf->mSortFunc == NULL)
    {
        (void)fprintf(stderr, "error : you must specify algorithm to use. \n");
        exit(1);
    }
}

static void dumpArray(uint32_t *aArray, int32_t aCount)
{
    uint32_t i;

    for (i = 0; i < aCount; i++)
    {
        (void)printf("%u\n", aArray[i]);
    }
}

int32_t main(int32_t aArgc, char *aArgv[])
{
    testConf        sConf;

    struct timeval  sStart, sEnd;
    int32_t         sSeconds, sUseconds;

    uint32_t       *sArray;

    testConfInit(&sConf);
    processArg(aArgc, aArgv, &sConf);

    sArray = createArraySinWave2(sConf.mCount, 10);
#if 0
    sArray = createArrayRandom(sConf.mCount);
#endif

    dumpArray(sArray, sConf.mCount);

    (void)fprintf(stderr, "Starting sorting %d integers with %s.....\n", sConf.mCount, sConf.mAlgorithmName);
    (void)gettimeofday(&sStart, NULL);

    (*sConf.mSortFunc)((MY_TYPE *)sArray, sConf.mCount, compareFunc);

    (void)gettimeofday(&sEnd, NULL);
    (void)fprintf(stderr, "Completed sorting...\n");

    sSeconds  = sEnd.tv_sec - sStart.tv_sec;
    sUseconds = sEnd.tv_usec - sStart.tv_usec;

    if (sUseconds < 0)
    {
        sSeconds--;
        sUseconds += 1000000;
    }

    (void)fprintf(stderr, "\nIt took %d.%06d seconds to sort %d integers.\n\n",
                  sSeconds, sUseconds, sConf.mCount);

    if (sConf.mDoVerify == 1)
    {
        (void)fprintf(stderr, "Checking if resulting array is correctly sorted...... ");

        if (verifyArrayIsSorted(sArray, sConf.mCount) == 0)
        {
            (void)fprintf(stderr, "OK\n");
        }
        else
        {
            (void)fprintf(stderr, "FAIL\n");
        }
    }

    destroyArray(sArray);

    return 0;
}
