#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>

#include <stdio.h>

/*ex)

#include <stdio.h>

int main()
{
    dsu *d = init_dsu(4);
    merge_dsu(d, 0, 1);
    assert(same_dsu(d, 0, 1) == true);
    merge_dsu(d, 1, 2);
    assert(same_dsu(d, 0, 2) == true);
    assert(size_dsu(d, 0) == 3);
    assert(same_dsu(d, 0, 3) == false);
}

*/

typedef struct result_groups_dsu
{
    int group_number;
    int *group_size;
    int **result;
} result_groups_dsu;

typedef struct dsu
{
    int _n;
    int *parent_or_size;
} dsu;

dsu *init_dsu(int n);
int merge_dsu(dsu *, int, int);
bool same_dsu(dsu *, int, int);
int leader_dsu(dsu *, int);
int size_dsu(dsu *, int);
void groups_dsu(dsu *);

dsu *init_dsu(int n)
{
    dsu *ret = malloc(sizeof(dsu));
    ret->_n = n;
    ret->parent_or_size = (int *)malloc(n * sizeof(int));
    memset(ret->parent_or_size, -1, n * sizeof(int));
    return ret;
}
int merge_dsu(dsu *d, int a, int b)
{
    assert(0 <= a && a < d->_n);
    assert(0 <= b && b < d->_n);
    int x = leader_dsu(d, a), y = leader_dsu(d, b);
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
bool same_dsu(dsu *d, int a, int b)
{
    assert(0 <= a && a < d->_n);
    assert(0 <= b && b < d->_n);
    return leader_dsu(d, a) == leader_dsu(d, b);
}

int leader_dsu(dsu *d, int a)
{
    assert(0 <= a && a < d->_n);
    if (d->parent_or_size[a] < 0)
        return a;
    return d->parent_or_size[a] = leader_dsu(d, d->parent_or_size[a]);
}

int size_dsu(dsu *d, int a)
{
    assert(0 <= a && a < d->_n);
    return -d->parent_or_size[leader_dsu(d, a)];
}

void groups_dsu(dsu *d)
{
    //TODO
}