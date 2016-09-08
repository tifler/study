#include <stdio.h>
#include "common.h"

void bubble_sort_ascending(int *array, int count)
{
    int i, j;
    int swapCount;

    show_entry(array, count);
    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    printf("----------------------------\n");

    for (i = 0; i < count; i++) {
        swapCount = 0;
        for (j = i + 1; j < count; j++) {
            if (array[i] > array[j]) {
                swap(array, i, j);
                swapCount++;
            }
        }

        if (swapCount == 0)
            break;

        printf("%4d: ", i + 1);
        for (j = 0; j < count; j++) {
            printf("%d ", array[j]);
        }
        printf("\n");
    }

    printf("----------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
