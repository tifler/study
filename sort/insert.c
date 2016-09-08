#include <stdio.h>
#include "common.h"

void insert_sort_ascending(int *array, int count)
{
    int i, j;
    int valueToInsert;
    int holePosition;
    int move, compare;

    move = compare = 0;

    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    for (i = 1; i < count; i++) {
        valueToInsert = array[i];
        holePosition = i;
        while (holePosition > 0 && array[holePosition - 1] > valueToInsert) {
            array[holePosition] = array[holePosition - 1];
            holePosition--;
            compare++;
            move++;
        }
        array[holePosition] = valueToInsert;
        move++;

        printf("Move=%d, Compare=%d\t", move, compare);

        for (j = 0; j < count; j++) {
            printf("%d ", array[j]);
        }
        printf("\n");
    }

    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
