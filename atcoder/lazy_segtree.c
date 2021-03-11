#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<assert.h>
typedef long long ll;
#define rep(i,l,r)for(ll i=(l);i<(r);i++)
#define repp(i,l,r,k)for(ll i=(l);i<(r);i+=(k))

//説明は後日追加します。


typedef struct lF{ll x,y;}lF;
typedef struct lS{ll x,y;}lS;

lS lseg_op(lS a, lS b){ 
    lS ret = {a.x +b.x, a.y+b.y};
    return ret;
}
lS lseg_e(){
    lS ret = {0, 0};
    return ret;
}
lS lseg_mapping(lF a, lS b){
    lS ret = {b.x + a.x*b.y, b.y};
    return ret;
}
lF lseg_composition(lF a, lF b){
    lF ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    return ret;
}
lF lseg_id(){
    lF ret = {0,0};
    return ret;
}

#pragma region lazy_segtree_static
typedef struct lazy_segtree lseg;
typedef struct lazy_segtree {
    int _n,size,_log;
    lS *d;
    lF *lz;

    lS (*op)(lS, lS);
    lS (*e)(void);
    lS (*mapping)(lF, lS);
    lF (*composition)(lF, lF);
    lF (*id)(void);

} lseg;

int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

void lseg_update(lseg *ls, int k){
    ls->d[k] = ls->op(ls->d[2 * k], ls->d[2 * k + 1]);
}
void lseg_all_apply(lseg *ls, int k, lF f){
    ls->d[k] = ls->mapping(f, ls->d[k]);
    if(k < ls->size) ls->lz[k] = ls->composition(f, ls->lz[k]);
}
void lseg_push(lseg *ls, int k){
    lseg_all_apply(ls, 2 * k, ls->lz[k]);
    lseg_all_apply(ls, 2 * k + 1, ls->lz[k]);
    ls->lz[k] = ls->id();
}


void lseg_set(lseg *ls, int p, lS x){
    assert(0 <= p && p < ls->size);
    p += ls->size;
    rrep(i,ls->_log,1) lseg_push(ls, p >> i);
    ls->d[p] = x;
    rep(i,1,ls->_log + 1) lseg_update(ls, p>>i);
}

lS lseg_get(lseg *ls, int p){
    assert(0 <= p && p < ls->size);
    p += ls->size;
    rrep(i,ls->_log,1) lseg_push(ls, p >> i);
    return ls->d[p];
}

lS lseg_prod(lseg *ls, int l, int r){
    assert(0 <= l && l <= r && r <= ls->_n);
    if(l == r) return ls->e();

    l += ls->size;
    r += ls->size;

    rrep(i,ls->_log,1){
        if(((l >> i) << i) != l) lseg_push(ls, l >> i);
        if(((r >> i) << i) != r) lseg_push(ls, r >> i);
    }

    lS sml = ls->e(), smr = ls->e();
    while(l < r){
        if(l & 1) sml = ls->op(sml, ls->d[l++]);
        if(r & 1) smr = ls->op(ls->d[--r], smr);
        l >>= 1;
        r >>= 1;
    }

    return ls->op(sml, smr);
}

lS lseg_all_prod(lseg *ls){return ls->d[1];}

void lseg_apply(lseg *ls, int l, int r, lF f){
    assert(0 <= l && l <= r && r <= ls->_n);
    if(l == r) return;

    l += ls->size;
    r += ls->size;

    rrep(i,ls->_log,1){
        if(((l >> i) << i) != l) lseg_push(ls, l >> i);
        if(((r >> i) << i) != r) lseg_push(ls, (r - 1) >> i);
    }

    {
        int l2 = l, r2 = r;
        while(l < r){
            if(l & 1) lseg_all_apply(ls, l++, f);
            if(r & 1) lseg_all_apply(ls, --r, f);
            l >>= 1;
            r >>= 1;
        }
        l = l2;
        r = r2;
    }

    rep(i,1,ls->_log + 1){
        if(((l >> i) << i) != l) lseg_update(ls, l >> i);
        if(((r >> i) << i) != r) lseg_update(ls, (r - 1) >> i);
    }
}


lseg *lseg_init(int n,
                lS a[], 
                lS (*op)(lS,lS), 
                lS(*e)(), 
                lS (*mapping)(lF, lS),
                lF (*composition)(lF, lF),
                lF (*id)()
                ) {
    lseg *ret = (lseg*)malloc(sizeof(lseg));
    ret->_n = n;
    ret->_log = ceil_pow2(n);
    ret->size = 1 << ret->_log;

    ret->d = (lS *)malloc(2 * ret->size * sizeof(lS));
    ret->lz = (lF *)malloc(ret->size * sizeof(lF));

    ret->op = op;
    ret->e = e;
    ret->mapping = mapping;
    ret->composition = composition;
    ret->id = id;

    rep(i,0,ret->size * 2) ret->d[i] = ret->e();
    rep(i,0,ret->size) ret->lz[i] = ret->id();

    if(a!=NULL){
        rep(i,0,n) ret->d[ret->size + i] = a[i];
    }
    rrep(i,ret->size-1,1){
        lseg_update(ret,i);
    }

    return ret;
}
#pragma endregion lazy_segtree_static


