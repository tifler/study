#include <stdio.h>
#include "common.h"

int bsearch(const int *array, int count, int key)
{
    printf("----------------------------\n");
	printf("binary search(%d units, key=%d)\n", count, key);
    show_entry(array, count);
    printf("----------------------------\n");
	
	int left = 0;
	int right = count - 1;
	for ( ; ; ) {
		int i = (left + right) / 2;
		printf("left=%d, right=%d, i=%d\n", left, right, i);
		if (i < left)
			break;

		if (i > right)
			break;

		compareCount++;
		int delta = array[i] - key;
		if (delta == 0)
			return i;

		if (delta < 0)
			left = i + 1;
		else
			right = i - 1;
	}

	return -1;
}
