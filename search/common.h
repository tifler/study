#ifndef __COMMON_H__
#define __COMMON_H__

#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

typedef int (*search_func_t)(const int *, int, int);

int bsearch(const int *, int, int);
int bsearch_min(const int *, int, int);

void show_entry(int *, int);
extern int compareCount;
extern int actionCount;

#endif  /*__COMMON_H__*/
