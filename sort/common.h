#ifndef __COMMON_H__
#define __COMMON_H__

#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

typedef void (*sort_func_t)(int *, int);

static inline void swap(int *array, int a, int b)
{
    int tmp = array[a];
    array[a] = array[b];
    array[b] = tmp;
}

void bubble_sort_ascending(int *, int);
void select_sort_ascending(int *, int);
void insert_sort_ascending(int *, int);
void merge_sort_ascending(int *, int);

void show_entry(int *array, int count);

#endif  /*__COMMON_H__*/
