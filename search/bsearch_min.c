#include <stdio.h>
#include "common.h"

/*
 * return found index or index of the lower bound which the key is in.
 */
int bsearch_min(const int *array, int count, int key)
{
    printf("----------------------------\n");
	printf("binary_min search(%d units, key=%d)\n", count, key);
    show_entry(array, count);
    printf("----------------------------\n");
	
	int left = 0;
	int right = count - 1;
	for ( ; ; ) {
		if (left > right) {
			printf("limited. left=%d > right=%d\n", left, right);
			if (right >= 0 && left < count)
				return right;
			break;
		}

		int i = (left + right) / 2;
		printf("left=%d, right=%d, i=%d\n", left, right, i);
#if	0
		if (i < left)
			break;

		if (i > right)
			break;
#endif	/*0*/

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
