#include <stdio.h>
#include "common.h"

void insert_sort_ascending(int *array, int count)
{
    int i;
    int valueToInsert;
    int holePosition;

    printf("----------------------------\n");
	printf("Insert sort(%d)\n", count);
	show_entry(array, count);
    printf("----------------------------\n");

    for (i = 1; i < count; i++) {
        valueToInsert = array[i];
        holePosition = i;
        while (holePosition > 0 && array[holePosition - 1] > valueToInsert) {
            array[holePosition] = array[holePosition - 1];
            holePosition--;
            compareCount++;
            actionCount++;
			show_entry(array, count);
        }
        array[holePosition] = valueToInsert;
		actionCount++;

        //printf("%4d: ", i + 1);
		show_entry(array, count);
    }

    printf("----------------------------\n");
	show_entry(array, count);
    printf("----------------------------\n");
}
