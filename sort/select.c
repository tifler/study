#include <stdio.h>

#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

static inline void swap(int array[], int a, int b)
{
    int tmp = array[a];
    array[a] = array[b];
    array[b] = tmp;
}

static void select_sort_ascending(int array[], int count)
{
    int i, j;
    int selectedValue;
    int selectedIndex;

    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    printf("----------------------------\n");

    for (i = 0; i < count; i++) {
        selectedValue = array[i];
        selectedIndex = i;
        for (j = i + 1; j < count; j++) {
            if (selectedValue > array[j]) {
                selectedValue = array[j];
                selectedIndex = j;
            }
        }
        if (i != selectedIndex)
            swap(array, i, selectedIndex);

        printf("%4d: ", i + 1);
        for (j = 0; j < count; j++) {
            printf("%d ", array[j]);
        }
        printf(": swap(%d, %d)\n", array[i], array[selectedIndex]);
    }

    printf("----------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

static int table[] = { 10, 25, 3, 100, 28, 59, 44, 30, 79, 1, 0 };

int main(void)
{
    select_sort_ascending(table, ARRAY_SIZE(table));
    return 0;
}
