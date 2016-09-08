#include <stdio.h>
#include <assert.h>

#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

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

    for (ia = 0; ia < count; ia++) {
        printf("%d ", a[ia]);
    }
    printf("\n");

    return a;
}

static int *recursive_merge_sort(int *array, int count)
{
    int *a, *b;

    if (count == 1)
        return array;

    a = recursive_merge_sort(&array[0], count / 2);
    b = recursive_merge_sort(&array[count / 2], count - (count / 2));

    return merge(a, count / 2, b, count - (count / 2));
}

static void merge_sort_ascending(int array[], int count)
{
    int i, j;
    int selectedValue;
    int selectedIndex;

    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    printf("----------------------------\n");

    recursive_merge_sort(array, count);

    printf("----------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

static int table[] = { 10, 25, 3, 100, 28, 59, 44, 30, 79, 1, 0 };

int main(void)
{
    merge_sort_ascending(table, ARRAY_SIZE(table));
    return 0;
}
