#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<assert.h>
typedef long long ll;
#define INF (1LL<<62)
#define max(a,b) (a>b?a:b)
#define min(a,b) (a>b?b:a)
ll gcd(ll a,ll b){return b?gcd(b,a%b):a;}


// segtree ここから

// typedef struct S{ ll a,b; }S;
typedef ll S;

S seg_e_max(){ return -INF; }
S seg_e_min(){ return INF; }
S seg_e_sum(){ return 0; }
S seg_e_gcd(){ return 0; }

S seg_op_max(S a, S b){ return max(a,b); }
S seg_op_min(S a, S b){ return min(a,b); }
S seg_op_sum(S a, S b){ return a+b; }
S seg_op_gcd(S a, S b){ return gcd(a,b); }


typedef struct seg_tree seg;
typedef struct seg_tree {
    int _n,size,_log;
    S *d;
    S (*op)(S, S);
    S (*e)(void);
} seg;

int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

void seg_update(seg *s, int k){ 
    s->d[k] = s->op(s->d[2 * k], s->d[2 * k + 1]);
}
void seg_set(seg *s, int p, S x){
    assert(0 <= p && p < s->_n);
    p += s->size;
    s->d[p] = x;
    for(int i = 1; i <= s->_log; i++) seg_update(s, p >> i);
}
S seg_get(seg *s, int p){
    assert(0 <= p && p < s->_n);
    return s->d[p + s->size];
}
S seg_prod(seg *s, int l, int r){
    assert(0 <= l && l <= r && r <= s->_n);
    S sml = s->e(), smr = s->e();
    l += s->size;
    r += s->size;

    while(l < r){
        if(l & 1) sml = s->op(sml, s->d[l++]);
        if(r & 1) smr = s->op(s->d[--r], smr);
        l >>= 1;
        r >>= 1;
    }
    return s->op(sml, smr);
}
S seg_all_prod(seg * s){ return s->d[1]; }

seg *seg_init(int n, S a[], S (*op)(S,S), S(*e)()) {
    seg *ret = (seg*)malloc(sizeof(seg));
    ret->_n = n;
    ret->_log = ceil_pow2(n);
    ret->size = 1 << ret->_log;

    ret->d = (S *)malloc(2 * ret->size * sizeof(S));

    ret->op = op;
    ret->e = e;

    for(int i = 0; i < ret->size * 2; i++) ret->d[i] = ret->e();

    if(a != NULL){
        for(int i = 0; i < n; i++) ret->d[ret->size + i] = a[i];
    }
    for(int i = ret->size - 1; i>= 1; i--){
        seg_update(ret, i);
    }

    return ret;
}

int seg_max_right(seg *s, int l, bool (*f)(S)){
    assert(0 <= l && l <= s->_n);
    assert(f(s->e()));
    if(l == s->_n) return s->_n;
    l += s->size;
    S sm = s->e();
    do{
        while(l % 2 == 0) l >>= 1;
        if(!f(s->op(sm, s->d[l]))){
            while(l < s->size){
                l = (2 * l);
                if(f(s->op(sm, s->d[l]))){
                    sm = s->op(sm, s->d[l]);
                    l++;
                }
            }
            return l - s->size;
        }
        sm = s->op(sm, s->d[l]);
        l++;
    }while((l & -l) != l);
    return s->_n;
}

int seg_min_left(seg *s, int r, bool (*f)(S)){
    assert(0 <= r && r <= s->_n);
    assert(f(s->e()));
    if(r == 0) return 0;
    r += s->size;
    S sm = s->e();
    do{
        while(r > 1 && (r % 2)) r >>= 1;
        if(!f(s->op(s->d[r], sm))){
            while(r < s->size){
                r = (2 * r + 1);
                if(f(s->op(s->d[r], sm))){
                    sm = s->op(s->d[r], sm);
                    r--;
                }
            }
            return r + 1 - s->size;
        }
        sm = s->op(s->d[r], sm);
    }while((r & -r) != r);
    return 0;
}

// segtreeここまで


ll target;
bool f(S s){
    return s < target;
}

int main(void){
    int n,q;
    scanf("%d %d",&n,&q);
 
    S a[n];
    for(int i=0; i<n; i++){
        scanf("%lld",&a[i]);
    }
    seg *s=seg_init(n,a,seg_op_max,seg_e_max);

    ll p,x,y;
    for(int i=0; i<q; i++){
        scanf("%lld %lld %lld",&p,&x,&y);
        if(p==1){
            seg_set(s,x-1,y);
        }
        if(p==2){
            printf("%lld\n",seg_prod(s,x-1,y));
        }
        if(p==3){
            target = y;
            printf("%d\n",seg_max_right(s,x-1,f)+1);
        }
    }
    return 0;
}



/*
内部実装、使い方共にACL準拠になっています。

使用例: AtCoder Library Plactice Contest J問題 ACコード
https://atcoder.jp/contests/practice2/submissions/20840820

注意点
・seg_min_leftは未verifyです。
・同時に2種の方のセグ木を持つことができません。
・要素を2つ以上持つ型を乗せる場合はSを構造体として定義しなおし、op,eを適宜書けば大丈夫だと思われます(未検証)。

*/
