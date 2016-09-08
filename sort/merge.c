#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"

static int *merge(int *a, int ca, int *b, int cb)
{
    int ia, ib, ic;
    int count = ca + cb;
    int *c = malloc(sizeof(int) * count);

    ia = ib = ic = 0;

    while (ia < ca && ib < cb) {
        if (a[ia] > b[ib]) {
            c[ic++] = b[ib++];
        }
        else {
            c[ic++] = a[ia++];
        }
    }

    while (ia < ca) {
        c[ic++] = a[ia++];
    }

    while (ib < cb) {
        c[ic++] = b[ib++];
    }

    assert(ic == count);

    memcpy(a, c, sizeof(int) * count);
    free(c);

	show_entry(a, count);

    return a;
}

static int *recursive_merge_sort(int *array, int count)
{
    int *a, *b;

    if (count == 1)
        return array;

    a = recursive_merge_sort(&array[0], count / 2);
	show_entry(a, count / 2);
    b = recursive_merge_sort(&array[count / 2], count - (count / 2));
	show_entry(b, count - (count / 2));

    return merge(a, count / 2, b, count - (count / 2));
}

/***************************************************************************/

void merge_sort_ascending(int *array, int count)
{
    printf("----------------------------\n");
	printf("Merge sort\n");
	show_entry(array, count);
    printf("----------------------------\n");

    recursive_merge_sort(array, count);

    printf("----------------------------\n");
	show_entry(array, count);
    printf("----------------------------\n");
}
