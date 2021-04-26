#include <stdio.h>
#include <string.h>
#include "common.h"

/***************************************************************************/

int compareCount;
int actionCount;

static int tableTemplate[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
static int table[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

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
    search_func_t searches[] = {
		bsearch,
		bsearch_min,
	};

	for (int i = 0; i < ARRAY_SIZE(searches); i++) {
		int ret;
        compareCount = actionCount = 0;
		for (int j = 0; j < ARRAY_SIZE(tableTemplate); j++) {
			int key = tableTemplate[j];
			memcpy(table, tableTemplate, sizeof(tableTemplate));
			ret = searches[i](table, ARRAY_SIZE(tableTemplate), key);
			printf("(%c) Searched[%d] key=%d: ret=%d, Compare=%d, \n",
					ret < 0 ? '*' : ' ', i, key, ret, compareCount);
			key += 1;
			ret = searches[i](table, ARRAY_SIZE(tableTemplate), key);
			printf("(%c) Searched[%d] key=%d: ret=%d, Compare=%d, \n",
					ret < 0 ? '*' : ' ', i, key, ret, compareCount);
		}
#define	INVALID_KEY				(-5170)
		ret = searches[i](table, ARRAY_SIZE(tableTemplate), INVALID_KEY);
		printf("(%c) Searched[%d] key=%d: ret=%d, Compare=%d, \n",
				ret < 0 ? '*' : ' ', i, INVALID_KEY, ret, compareCount);
	}
    return 0;
}
