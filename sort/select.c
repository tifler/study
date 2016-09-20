#include <stdio.h>
#include "common.h"

void select_sort_ascending(int *array, int count)
{
    int i, j;
    int selectedValue;
    int selectedIndex;

    printf("----------------------------\n");
	printf("Select sort(%d)\n", count);
	show_entry(array, count);
    printf("----------------------------\n");

    for (i = 0; i < count; i++) {
        selectedValue = array[i];
        selectedIndex = i;
        for (j = i + 1; j < count; j++) {
			compareCount++;
            if (selectedValue > array[j]) {
                selectedValue = array[j];
                selectedIndex = j;
				actionCount++;
            }
        }
        if (i != selectedIndex) {
            swap(array, i, selectedIndex);
			compareCount++;
			actionCount++;
            show_entry(array, count);
		}
    }

    printf("----------------------------\n");
	show_entry(array, count);
    printf("----------------------------\n");
}
