AOJ:Depth First Search　(http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_B&lang=ja) での使用例　
http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=5179430#1

関数の説明
graph_init(頂点数); グラフの初期化
graph_push(グラフポインタ(?), 辺の始点, 辺の終点, 辺のコスト); グラフに辺を追加, 無向辺を追加する際は始点終点を入れ替えて計2度pushする
graph_to(ポインタ, 辺の始点, インデックス:i); 始点のi番目の隣接点を返す
graph_cost(ポインタ, 辺の始点, インデックス:i); 始点とi番目の隣接点を結ぶ辺のコストを返す
                        
また、type100行目付近のtypeof_graphにおいてコストの型を指定してるので適宜変更してください。
*そのため同時に型の違うグラフを持つことが出来ません(要検討)
                        
                        
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


#pragma region graph

typedef struct graph graph;

typedef long long typeof_graph;

typedef struct graph {
    int data_size;
    vector **vec_to;
    vector **vec_cost;
    int *ed_cnt;
    
    void (*push)(graph*, int, int, typeof_graph);
    int (*to)(graph*, int, int);
    typeof_graph (*cost)(graph*, int, int);
} graph;


void graph_push(graph *g, int from, int to, typeof_graph cost) {
    g->vec_to[from]->push(g->vec_to[from], (void*)&to);
    g->vec_cost[from]->push(g->vec_cost[from], (void*)&cost);
    g->ed_cnt[from]++;
}

int graph_to(graph *g, int from, int idx){
    return g->vec_to[from]->get(g->vec_to[from], idx);
}
typeof_graph graph_cost(graph *g, int from, int idx){
    return g->vec_to[from]->get(g->vec_to[from], idx);
}

graph *graph_init(int n) {
    graph *ret = (graph *)malloc(sizeof(graph));
    ret->data_size = sizeof(typeof_graph);
    ret->ed_cnt = (int *)malloc(n * sizeof(int));
    
    ret->vec_to = (vector **)malloc(n * sizeof(vector*));
    for(int i=0; i<n; i++){
        (ret->vec_to[i]) = vector_init(1, sizeof(int));
    }

    ret->vec_cost = (vector **)malloc(n * sizeof(vector*));
    for(int i=0; i<n; i++){
        (ret->vec_cost[i]) = vector_init(1, sizeof(typeof_graph));
    }
    
    ret->push = graph_push;
    ret->to = graph_to;
    ret->cost = graph_cost;
    return ret;
}

#pragma endregion graph
