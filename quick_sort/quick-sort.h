#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stdio.h>
#include <stdlib.h>

#define maxStackFram
typedef struct StackFrameQuickSort{
    int pc;
    int l, r;
}StackFrameQuickSort;
StackFrameQuickSort stk[100];
StackFrameQuickSort* top = stk - 1;


#endif
