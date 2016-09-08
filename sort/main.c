#include "common.h"

static int table[] = { 10, 25, 3, 100, 28, 59, 44, 30, 79, 1, 0 };

void show_entry(int *array, int count)
{
    int i;
    for (i = 0; i < count; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main(const char **argv, int argc)
{
    sort_func_t sort = bubble_sort_ascending;
    sort(table, ARRAY_SIZE(table));
    return 0;
}
