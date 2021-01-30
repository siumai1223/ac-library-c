AOJ:Depth First Search　(http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_B&lang=ja) での使用例　
http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=5179323#1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>

#pragma region vector

typedef struct vector vector;

typedef struct vector {
    int n, max, data_size;
    void *data;
    void (*push)(vector*, void*);
    void (*set)(vector*, int, void*);
    long long (*get)(vector*, int);
    double (*get_double)(vector*, int);
    int (*size)(vector*);
} vector;


void vector_push(vector *v, void *x) {
    if (v->n == v->max) {
        v->max *= 2;
        assert((v->data = realloc(v->data, v->max * v->data_size)) != NULL);
    }
    memcpy(v->data + v->data_size * v->n, x, v->data_size);
    v->n++;
}

void vector_set(vector *v, int idx, void *x) {
    memcpy(v->data + v->data_size * idx, x, v->data_size);
}

void *vector_get_internal(vector *v, int idx) {
    return (v->data + v->data_size * idx);
}

long long vector_get(vector *v, int idx){
    long long ret=0;
    if(v->data_size == 8) ret = *(long long*)vector_get_internal(v, idx);
    if(v->data_size == 4) ret = *(int*)vector_get_internal(v, idx);
    if(v->data_size == 2) ret = *(bool*)vector_get_internal(v, idx);
    return ret;
}

double vector_get_double(vector *v, int idx){
    assert(v->data_size == sizeof(double));
    return *(double*)vector_get(v, idx);
}

int vector_size(vector *v) {
    return v->n;
}

vector *vector_init(int n, int data_size) {
    assert(n != 0);
    vector *ret = (vector *)malloc(sizeof(vector));
    ret->n = 0;
    ret->max = n;
    ret->data_size = data_size;
    ret->data = malloc(n * data_size);

    ret->push=vector_push;
    ret->set=vector_set;
    ret->size=vector_size;
    ret->get = vector_get;
    ret->get_double = vector_get_double;

    memset(ret->data, 0, n * ret->data_size);
    return ret;
}

// void vector_free(vector *v){
//     free(v->data);
//     free(v->get);
//     free(v->get_double);
//     free(v->push);
//     free(v->set);
//     free(v->size);
//     free(v);
// }

#pragma endregion vector


#pragma region vector2
typedef struct vector2 vector2;

typedef struct vector2{
    int n, max, data_size;
    vector **vec;
    void (*push)(vector2*, int);
} vector2;


void vector2_push(vector2 *v2, int n){
    if (v2->n == v2->max) {
        v2->max *= 2;
        assert((v2->vec = realloc(v2->vec, v2->max * sizeof(vector))) != NULL);
        for(int i=v2->n; i<v2->max; i++){
            v2->vec[i + 1] = vector_init(1, v2->data_size);
        }
    }
    v2->n++;
}

vector2 *vector2_init(int n, int data_size) {
    assert(n != 0);
    vector2 *ret = (vector2 *)malloc(sizeof(vector2));
    ret->n = 0;
    ret->max = n;
    ret->data_size = data_size;
    ret->vec = (vector **)malloc(n * sizeof(vector*));
    for(int i=0; i<n; i++){
        (ret->vec[i]) = vector_init(1, sizeof(data_size));
    }
    ret->push = vector2_push;
    return ret;
}
#pragma endregion vector2


#pragma region graph

typedef struct graph graph;

typedef struct graph {
    int data_size;
    vector **vec_to;
    vector **vec_cost;
    int *ed_cnt;
    void (*push)(graph*, int, int, long long, bool);
    int (*to)(graph*, int, int);
    long long (*cost)(graph*, int, int);
    double (*cost_double)(graph*, int, int);
} graph;


void graph_push(graph *g, int from, int to, long long cost, bool is_dir) {
    g->vec_to[from]->push(g->vec_to[from], (void*)&to);
    if(g->data_size != sizeof(bool)){
        g->vec_cost[from]->push(g->vec_cost[from], (void*)&cost);
    } 
    g->ed_cnt[from]++;

    if(is_dir == false){
        g->vec_to[to]->push(g->vec_to[to], (void*)&from);
        if(g->data_size != sizeof(bool)){
            g->vec_cost[to]->push(g->vec_cost[to], (void*)&cost);
        } 
        g->ed_cnt[to]++;
    }
}

int graph_to(graph *g, int from, int idx){
    return g->vec_to[from]->get(g->vec_to[from], idx);
}
long long graph_cost(graph *g, int from, int idx){
    return g->vec_to[from]->get(g->vec_to[from], idx);
}
double graph_cost_double(graph *g, int from, int idx){
    return g->vec_to[from]->get_double(g->vec_to[from], idx);
}

graph *graph_init(int n, int data_size) {
    graph *ret = (graph *)malloc(sizeof(graph));
    ret->data_size = data_size;
    ret->vec_to = (vector **)malloc(n * sizeof(vector*));
    for(int i=0; i<n; i++){
        (ret->vec_to[i]) = vector_init(1, sizeof(int));
    }
    if(data_size != sizeof(bool)){
        ret->vec_cost = (vector **)malloc(n * sizeof(vector*));
        for(int i=0; i<n; i++){
            (ret->vec_cost[i]) = vector_init(1, sizeof(data_size));
        }
    }
    ret->ed_cnt = (int *)malloc(n * sizeof(int));

    ret->push = graph_push;
    ret->to = graph_to;
    ret->cost = graph_cost;
    ret->cost_double = graph_cost_double;
    return ret;
}

#pragma endregion graph

