#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>

/*

example)

int main()
{
    dsu *d = dsu_init(4);
    dsu_merge(d, 0, 1);
    assert(dsu_same(d, 0, 1) == true);
    dsu_merge(d, 1, 2);
    assert(dsu_same(d, 0, 2) == true);
    assert(dsu_size(d, 0) == 3);
    assert(dsu_same(d, 0, 3) == false);
}

*/

typedef struct dsu
{
    int _n;
    int *parent_or_size;
} dsu;

dsu *dsu_init(int n);
int dsu_merge(dsu *, int, int);
bool dsu_same(dsu *, int, int);
int dsu_leader(dsu *, int);
int dsu_size(dsu *, int);
void dsu_groups(dsu *);

dsu *dsu_init(int n)
{
    dsu *ret = malloc(sizeof(dsu));
    ret->_n = n;
    ret->parent_or_size = (int *)malloc(n * sizeof(int));
    memset(ret->parent_or_size, -1, n * sizeof(int));
    return ret;
}
int dsu_merge(dsu *d, int a, int b)
{
    assert(0 <= a && a < d->_n);
    assert(0 <= b && b < d->_n);
    int x = dsu_leader(d, a), y = dsu_leader(d, b);
    if (x == y)
        return x;
    if (-d->parent_or_size[x] < -d->parent_or_size[y])
    {
        int tmp = x;
        x = y;
        y = tmp;
    }
    d->parent_or_size[x] += d->parent_or_size[y];
    d->parent_or_size[y] = x;
    return x;
}
bool dsu_same(dsu *d, int a, int b)
{
    assert(0 <= a && a < d->_n);
    assert(0 <= b && b < d->_n);
    return dsu_leader(d, a) == dsu_leader(d, b);
}

int dsu_leader(dsu *d, int a)
{
    assert(0 <= a && a < d->_n);
    if (d->parent_or_size[a] < 0)
        return a;
    return d->parent_or_size[a] = dsu_leader(d, d->parent_or_size[a]);
}

int dsu_size(dsu *d, int a)
{
    assert(0 <= a && a < d->_n);
    return -d->parent_or_size[dsu_leader(d, a)];
}

void dsu_groups(dsu *d)
{
    //TODO
}