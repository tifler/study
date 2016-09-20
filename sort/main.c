#include <stdio.h>
#include <string.h>
#include "common.h"

/***************************************************************************/

int compareCount;
int actionCount;

static int tableTemplate[] = { 10, 25, 3, 100, 28, 59, 44, 30, 79, 1, 0 };
static int table[] = { 10, 25, 3, 100, 28, 59, 44, 30, 79, 1, 0 };

/***************************************************************************/

void show_entry(int *array, int count)
{
    int i;
    for (i = 0; i < count; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
	int i;
    sort_func_t sorts[] = {
		insert_sort_ascending,
		bubble_sort_ascending,
		select_sort_ascending,
		merge_sort_ascending,
	};

	for (i = 0; i < ARRAY_SIZE(sorts); i++) {
        compareCount = actionCount = 0;
		memcpy(table, tableTemplate, sizeof(tableTemplate));
		sorts[i](table, ARRAY_SIZE(tableTemplate));
        printf("Compare=%d, Action=%d\n", compareCount, actionCount);
	}
    return 0;
}
