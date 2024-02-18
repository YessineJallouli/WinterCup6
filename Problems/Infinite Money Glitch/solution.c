#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <math.h>

typedef struct graph_t
{
    int n;
    int m;
    struct edge_t** adjacencyList;
    struct edge_t** reverseList;
    int *outgoing_degree,*ingoing_degree,*in_capacity,*out_capacity;
} graph_t;

typedef struct conversion_t
{
    double rate;
    double fee;
} conversion_t;

typedef struct edge_t
{
    int v;
    struct conversion_t conversion;
} edge_t;

typedef struct init_result
{
    graph_t graph;
    bool result;
}init_result_t;

init_result_t init_graph(int n,int m)
{
    init_result_t result;
    result.graph.n=n;
    result.graph.m=m;
    result.graph.adjacencyList = (edge_t**) malloc(sizeof(edge_t*)*result.graph.n);
    if(!result.graph.adjacencyList)
    {
        result.result=true;
        return result;
    }
    result.graph.reverseList = (edge_t**) malloc(sizeof(edge_t*)*result.graph.n);
    if(!result.graph.reverseList)
    {
        result.result=true;
        return result;
    }
    result.graph.outgoing_degree = (int*) malloc(sizeof(int)*result.graph.n);
    if(!result.graph.outgoing_degree)
    {
        result.result=true;
        return result;
    }
    result.graph.ingoing_degree = (int*) malloc(sizeof(int)*result.graph.n);
    if(!result.graph.ingoing_degree)
    {
        result.result=true;
        return result;
    }
    result.graph.in_capacity= (int*) malloc(sizeof(int)*result.graph.n);
    if(!result.graph.in_capacity)
    {
        result.result=true;
        return result;
    }
    result.graph.out_capacity = (int*) malloc(sizeof(int)*result.graph.n);
    if(!result.graph.out_capacity)
    {
        result.result=true;
        return result;
    }
    memset(result.graph.adjacencyList,0,sizeof(edge_t*)*result.graph.n);
    memset(result.graph.reverseList,0,sizeof(edge_t*)*result.graph.n);
    memset(result.graph.outgoing_degree,0,sizeof(int)*result.graph.n);
    memset(result.graph.ingoing_degree,0,sizeof(int)*result.graph.n);
    memset(result.graph.in_capacity,0,sizeof(int)*result.graph.n);
    memset(result.graph.out_capacity,0,sizeof(int)*result.graph.n);
    result.result=false;
    return result;
}

int* push_back_int(int *array,int *size,int *capacity,int value)
{
    if(*size==*capacity)
    {
        *capacity*=2;
        if(!(*capacity))
            (*capacity)++;
        int *new_arr=(int*) realloc(array,sizeof(int)*(*capacity));
        if(!new_arr)
            return NULL;
        array = new_arr;
    }
    array[*size]=value;
    (*size)++;
    return array;
}

edge_t* push_back_edge(edge_t *array, int *size, int *capacity, edge_t value)
{
    if(*size == *capacity)
    {
        *capacity*=2;
        if(!(*capacity))
            (*capacity)++;
        edge_t * new_arr=(edge_t*) realloc(array,sizeof(edge_t)*(*capacity));
        if(!new_arr)
            return NULL;
        array= new_arr;
    }
    array[*size]=value;
    (*size)++;
    return array;
}

bool connect(graph_t *G,int a,int b,conversion_t conversion)
{
    edge_t* e1=push_back_edge(G->adjacencyList[a],G->outgoing_degree+a,G->out_capacity+a,(edge_t){b,conversion});
    if(!e1)
        return true;
    G->adjacencyList[a]=e1;
    edge_t* e2 =  push_back_edge(G->reverseList[b],G->ingoing_degree+b,G->in_capacity+b,(edge_t){a,conversion});
    if(!e2)
        return true;
    G->reverseList[b]=e2;
    return false;
}

void destroy_graph(graph_t *G)
{
    for(int i=0;i<G->n;i++)
    {
        free(G->adjacencyList[i]);
        free(G->reverseList[i]);
    }
    free(G->adjacencyList);
    free(G->reverseList);
    free(G->outgoing_degree);
    free(G->ingoing_degree);
    free(G->in_capacity);
    free(G->out_capacity);
}

double bellman_ford(graph_t *G,int s, int x)
{
    double *D = (double*) malloc(sizeof(double)*G->n);
    memset(D,0,sizeof(double)*G->n);
    if(!D)
        return NAN;
    D[s]=x;
    for(int i=0;i<G->n;i++) for(int u=0;u<G->n;u++) for(int j=0;j<G->outgoing_degree[u];j++)
    {
        int v=G->adjacencyList[u][j].v;
        conversion_t conversion=G->adjacencyList[u][j].conversion;
        D[v]=fmax(D[v],conversion.rate*(D[u]-conversion.fee));
    }
    double result=D[s];
    free(D);
    return result;
}

bool has_gained_money(graph_t *G,int s,int x)
{
    return bellman_ford(G,s,x) > x;
}

int minimal_required_money(graph_t *G,int s, int max_money)
{
    int a=0,b=max_money+1,c;
    while(a<b)
    {
        c=(a+b)/2;
        if(has_gained_money(G,s,c))
            b=c;
        else
            a=c+1;
    }
    return a;

}

int main()
{
    int n,m,x;
    scanf("%d %d %d",&n,&m,&x);
    init_result_t init=init_graph(n,m);
    if(init.result)
       return EXIT_FAILURE;
    graph_t G=init.graph;
    for(int i=0;i<m;i++)
    {
        int a,b;
        conversion_t conversion;
        scanf("%d %d %lf %lf",&a,&b,&conversion.rate,&conversion.fee);
        a--;
        b--;
        if(connect(&G,a,b,conversion))
            return EXIT_FAILURE;
    }
    int min_money=minimal_required_money(&G,0,x);
    if(min_money>x)
        min_money=-1;
    printf("%d\n",min_money);
    destroy_graph(&G);
}