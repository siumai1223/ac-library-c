#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
/* example

int main(){
    int a=1,b=7,c=3,d=4,e=5;
    seg* s=seg_init(5,NULL,seg_op_max,seg_e_max);
    s->update(s,0,a);
    s->update(s,1,b);
    s->update(s,2,c);
    s->update(s,3,d);
    s->update(s,4,e);
    for(int i=0;i<5;++i){
        printf("%lld ",s->get(s,i));
    }
    printf("\n");
    for(int i=0;i<5;++i){
        for(int j=i;j<5;++j){
            printf("%lld ",s->prod(s,i,j));
        }
        printf("\n");
    }
}

*/

#define INF (1LL<<62)

long long min(long long a, long long b){ return a < b ? a : b; }
long long max(long long a, long long b){ return a < b ? b : a; }
long long gcd(long long a,long long b){ return b ? gcd(b, a % b) : a; }

long long seg_e_max(){ return -INF; }
long long seg_e_min(){ return INF; }
long long seg_e_sum(){ return 0; }
long long seg_e_gcd(){ return 0; }

long long seg_op_max(long long a, long long b){ return max(a,b); }
long long seg_op_min(long long a, long long b){ return min(a,b); }
long long seg_op_sum(long long a, long long b){ return a+b; }
long long seg_op_gcd(long long a, long long b){ return gcd(a,b); }


typedef struct seg_tree seg;

typedef struct seg_tree 
{
    int _n;
    long long *node;
    long long (*op)(long long, long long);
    long long (*e)(void);
    long long (*get)(seg*, int);
    long long (*prod)(seg*, int, int);
    void (*update)(seg*, int, long long);
} seg;


long long seg_get(seg *S, int x)
{
    return S->node[S->_n / 2 + x];
}

long long seg_prod_inner(seg *S, int l, int r, int x, int from, int to)
{
    if (l <= from && to <= r ) return S->node[x];
    if (r < from || to < l ) return S->e();
    return  S->op(
                seg_prod_inner(S, l, r, 2 * x + 1, from, (from + to) / 2),
                seg_prod_inner(S, l, r, 2 * x + 2, (from + to) / 2 + 1, to)
            );
}
long long seg_prod(seg *S, int l, int r)
{
    return seg_prod_inner(S, l, r, 0, 0, S->_n / 2);
}

void seg_update(seg *S, int x, long long val)
{
    long long c = S->_n / 2 + x;
    S->node[c] = val;
    while (c > 0)
    {
        c--; 
        c /= 2;
        S->node[c] = S->op(S->node[2 * (c + 1)], S->node[2 * (c + 1) - 1]);
    }
}

seg *seg_init(int n, long long a[], long long (*op)(long long, long long), long long(*e)())
{
    seg *ret = (seg*)malloc(sizeof(seg));
    int x = 1;
    ret->_n = 0;
    while (x < n)
    {
        ret->_n += x;
        x *= 2;
    } 
    ret->_n += x;
    ret->node = (long long *)malloc(ret->_n * sizeof(long long));
    ret->op = op;
    ret->e = e;
    ret->get = seg_get;
    ret->prod = seg_prod;
    ret->update = seg_update;
    // 初期化時にNULLを入れると単位元で初期化されます
    if(a==NULL){
        for(int i = 0; i < x; i++){
            ret->node[x-1+i]=ret->e();
        }
    }else{
        for(int i = 0; i < x; i++){
            if (i < n) ret->node[x- 1 + i] = a[i];
            else ret->node[x - 1 + i] = ret->e();
        }
    }
    while (x /= 2) {
        for(int i = 0; i < x; i++){
            int p = 2 * (x + i);
            ret->node[x + i - 1] = ret->op(ret->node[p], ret->node[p - 1]);
        }
    }
    return ret;
}
