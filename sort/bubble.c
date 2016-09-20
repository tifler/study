#include <stdio.h>
#include "common.h"

void bubble_sort_ascending(int *array, int count)
{
    int i;
	int last;
    int swapCount;

    printf("----------------------------\n");
	printf("Bubble sort(%d)\n", count);
    show_entry(array, count);
    printf("----------------------------\n");

	last = count - 1;
	do {
		swapCount = 0;
		for (i = 0; i < last; i++) {
			compareCount++;
			if (array[i] > array[i + 1]) {
				swap(array, i, i + 1);
				actionCount++;
				swapCount++;
			}
			printf("%4d: ", compareCount);
			show_entry(array, count);
		}
		last--;
	} while (swapCount > 0);

    printf("----------------------------\n");
	show_entry(array, count);
    printf("----------------------------\n");
}
