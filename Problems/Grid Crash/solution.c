//
// Created by ramizouari on 06/02/24.
//
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdbool.h>
#include <tgmath.h>
#include <string.h>
#include <stdio.h>

typedef enum color_t
{
    empty, black, white
}color_t;

typedef struct grid_t
{
    int n,m;
    int64_t content;
} grid_t;

const int64_t empty_grid=0;
const int64_t color_width=0b11;

color_t get_color(grid_t* G,int i,int j)
{
    if(i<0||i>=G->n||j<0||j>=G->m)
        return empty;
    int64_t order=color_width&(G->content>>(2*i*G->m+2*j));
    return (color_t) order;
}

void set_color(grid_t *G,int i,int j,color_t c)
{
    if(i<0||i>=G->n||j<0||j>=G->m)
        return;
    G->content&=~(color_width<<(2*i*G->m+2*j));
    G->content|=(int64_t)c<<(2*i*G->m+2*j);
}

color_t char_to_color(char c)
{
    switch (c)
    {
        case '.':
            return empty;
        case 'B':
            return black;
        case 'W':
            return white;
    }
}

char color_to_char(color_t color)
{
    switch (color)
    {
        case empty:
            return '.';
        case white:
            return 'W';
        case black:
            return 'B';
    }
}

bool fwrite_grid(grid_t grid, FILE* file)
{
    char* str= (char*) malloc(sizeof(char)*(grid.m+1));
    memset(str,0,sizeof(char)*(grid.m+1));
    if(!str)
        return true;
    for(int i=grid.n-1;i>=0;i--)
    {
        for(int j=0;j<grid.m;j++)
            str[j]=color_to_char(get_color(&grid,i,j));
        fwrite(str,sizeof(char),grid.m,file);
        fputc('\n',file);
    }

    return false;
}

void set_color_with_string(grid_t *G,char** grid_string, int n,int m)
{
    G->n=n;
    G->m=m;
    G->content=empty_grid;
    for(int i=0;i<n;i++) for(int j=0;j<m;j++)
        set_color(G,i,j, char_to_color(grid_string[i][j]));
}

typedef struct pair_linked_list_t
{
    int i,j;
    struct pair_linked_list_t *next;
}pair_linked_list_t;

pair_linked_list_t* init_pair_linked_list()
{
    return NULL;
}

pair_linked_list_t* add_pair_head(pair_linked_list_t *L, int i, int j)
{
    pair_linked_list_t *new_node=(pair_linked_list_t*)malloc(sizeof(pair_linked_list_t));
    if(!new_node)
        return NULL;
    new_node->i=i;
    new_node->j=j;
    new_node->next=L;
    return new_node;
}

pair_linked_list_t* add_pair_tail(pair_linked_list_t *L, int i, int j)
{
    pair_linked_list_t *new_node=(pair_linked_list_t*)malloc(sizeof(pair_linked_list_t));
    if(!new_node)
        return NULL;
    new_node->i=i;
    new_node->j=j;
    new_node->next=NULL;
    if(!L)
        return new_node;
    pair_linked_list_t *current=L;
    while(current->next)
        current=current->next;
    current->next=new_node;
    return L;
}

pair_linked_list_t* add_pair_tail_get_tail(pair_linked_list_t *L, int i, int j)
{
    pair_linked_list_t *new_node=(pair_linked_list_t*)malloc(sizeof(pair_linked_list_t));
    if(!new_node)
        return NULL;
    new_node->i=i;
    new_node->j=j;
    new_node->next=NULL;
    if(!L)
        return new_node;
    pair_linked_list_t *current=L;
    while(current->next)
        current=current->next;
    current->next=new_node;
    return new_node;
}

pair_linked_list_t* remove_pair_head(pair_linked_list_t *L)
{
    pair_linked_list_t *next=L->next;
    free(L);
    return next;
}

void destroy_pair_linked_list(pair_linked_list_t *L)
{
    while(L)
        L=remove_pair_head(L);
}

typedef struct pair_queue_t
{
    pair_linked_list_t *head,*tail;
    int size;
} pair_queue_t;

pair_queue_t init_pair_queue()
{
    pair_queue_t Q;
    Q.head=init_pair_linked_list();
    Q.size=0;
    return Q;
}

void push_element_pair_queue(pair_queue_t * queue, int i,int j)
{
    if(!queue->head)
    {
        queue->head=add_pair_head(queue->head,i,j);
        queue->tail=queue->head;
    }
    else
        queue->tail=add_pair_tail_get_tail(queue->tail,i,j);
    queue->size++;
}

void pop_element_pair_queue(pair_queue_t *queue, int *i,int *j)
{
    *i=queue->head->i;
    *j=queue->head->j;
    queue->head=remove_pair_head(queue->head);
    queue->size--;
}

bool is_pair_queue_empty(pair_queue_t *Q)
{
    return Q->head==NULL;
}

typedef struct pair_t
{
    int x,y;
} pair_t;
const pair_t du[4]={ {1,0},{-1,0},{0,1},{0,-1}};
bool is_direction_admissible(int i,int j,int n,int m, int d)
{
    return i+du[d].x>=0&&i+du[d].x<n&&j+du[d].y>=0&&j+du[d].y<m;
}

int min(int a,int b)
{
    return a<b?a:b;
}

void postProcess(grid_t*G)
{
    int *emptyCount = (int*)malloc(sizeof(int)*G->m);
    memset(emptyCount,0,sizeof(int)*G->m);
    for(int j=0;j<G->m;j++)
    {
        for(int i=0;i<G->n;i++)
        {
            if(get_color(G,i,j)==empty)
                emptyCount[j]++;
            else
                set_color(G,i-emptyCount[j],j, get_color(G,i,j));
        }
        for(int i=G->n-emptyCount[j];i<G->n;i++)
            set_color(G,i,j,empty);
    }
    int allEmptyCount=0;
    for(int j=0;j<G->m;j++)
    {
        if(emptyCount[j]==G->n)
            allEmptyCount++;
        else for(int i=0;i<G->n;i++)
            set_color(G,i,j-allEmptyCount, get_color(G,i,j));
    }
    for(int j=G->m-allEmptyCount;j<G->m;j++)  for(int i=0;i<G->n;i++)
        set_color(G,i,j,empty);
    int maximalEmpty=0;
    for(int j=0; j < G->m;j++)
        maximalEmpty=min(maximalEmpty,emptyCount[j]);
    int newRows=G->n-maximalEmpty;
    int newCols=G->m-allEmptyCount;
    free(emptyCount);
    if(newRows == G->n && newCols == G->m)
        return;
    grid_t new_grid;
    new_grid.content=empty_grid;
    new_grid.n=newRows;
    new_grid.m=newCols;
    for(int i=0;i<newRows;i++) for(int j=0;j<newCols;j++)
        set_color(&new_grid,i,j,get_color(G,i,j));
    *G=new_grid;
}

grid_t crash_cell(grid_t G,int i,int j)
{
    color_t color=get_color(&G,i,j);
    if(color==empty)
        return G;
    pair_queue_t Q=init_pair_queue();
    push_element_pair_queue(&Q,i,j);
    color_t old_color=color;
    set_color(&G,i,j,empty);
    while(!is_pair_queue_empty(&Q))
    {
        pop_element_pair_queue(&Q,&i,&j);
        for(int d=0;d<4;d++) if(is_direction_admissible(i,j,G.n,G.m,d))
        {
            int x=du[d].x,y=du[d].y;
            if(get_color(&G,i+x,j+y)==old_color)
            {
                push_element_pair_queue(&Q, i + x, j + y);
                set_color(&G,i+x,j+y,empty);
            }
        }
    }
    postProcess(&G);
    return G;
}
const int N_max=5,M_max=5;

void set_color_with_fixed_string(grid_t *G,char grid_string[N_max][M_max+1], int n,int m)
{
    G->n=n;
    G->m=m;
    G->content=empty_grid;
    for(int i=0;i<n;i++) for(int j=0;j<m;j++)
        set_color(G,i,j, char_to_color(grid_string[i][j]));
}

typedef struct grid_linked_list_t
{
    grid_t grid;
    int value;
    struct grid_linked_list_t *next;
}grid_linked_list_t;

grid_linked_list_t* init_grid_linked_list()
{
    return NULL;
}

bool is_grid_eq(grid_t x, grid_t y)
{
    return x.n==y.n&&x.m && x.content==y.content;
}

grid_linked_list_t* search_grid_element(grid_linked_list_t* list, grid_t grid)
{
    while(list && !is_grid_eq(list->grid,grid))
        list=list->next;
    return list;
}

typedef struct upsert_result_t
{
    grid_linked_list_t* list;
    bool inserted;
}upsert_result_t;

upsert_result_t upsert_grid_element(grid_linked_list_t **list,grid_t grid,int value)
{
    upsert_result_t result;
    result.inserted=false;
    grid_linked_list_t* search_result= search_grid_element(*list,grid);
    if(search_result)
    {
        search_result->value = value;
        result.list=*list;
    }
    else
    {
        grid_linked_list_t *new_node=(grid_linked_list_t*)malloc(sizeof(grid_linked_list_t));
        if(!new_node)
        {
            result.list=NULL;
            return result;
        }
        result.inserted=true;
        new_node->grid=grid;
        new_node->value=value;
        new_node->next=*list;
        *list=new_node;
        search_result=new_node;
        result.list=search_result;
    }
    return result;
}

grid_linked_list_t* remove_grid_head(grid_linked_list_t *L)
{
    grid_linked_list_t *next=L->next;
    free(L);
    return next;
}

void destroy_grid_linked_list(grid_linked_list_t *L)
{
    while(L)
        L=remove_grid_head(L);
}

typedef struct grid_map_t
{
    grid_linked_list_t **hashmap;
    int64_t x;
    int64_t mod;
    int64_t capacity;
}grid_map_t;

grid_map_t init_grid_map(int64_t mod, int64_t capacity)
{
    grid_map_t mapper;
    mapper.mod=mod;
    mapper.capacity=capacity;
    mapper.hashmap = (grid_linked_list_t**) malloc(sizeof(grid_linked_list_t*)*capacity);
    memset(mapper.hashmap,0,sizeof(grid_linked_list_t*)*capacity);
    return mapper;
}

void destroy_grid_map(grid_map_t *grid_map)
{
    for(int i=0;i<grid_map->capacity;i++)
        destroy_grid_linked_list(grid_map->hashmap[i]);
    free(grid_map->hashmap);
    grid_map->capacity=0;
    grid_map->hashmap=NULL;
}

bool is_empty_grid(grid_t grid)
{
    return grid.content==empty_grid;
}

typedef struct grid_queue_t
{
    grid_linked_list_t *head,*tail;
    int size;
} grid_queue_t;

grid_queue_t init_grid_queue()
{
    grid_queue_t Q;
    Q.head=init_grid_linked_list();
    Q.size=0;
    return Q;
}

bool is_grid_queue_empty(grid_queue_t *Q)
{
    return Q->head==NULL;
}

void destroy_grid_queue(grid_queue_t *queue)
{
    destroy_grid_linked_list(queue->head);
    queue->head=NULL;
    queue->size=0;
}


grid_linked_list_t* add_grid_head(grid_linked_list_t *L, grid_t grid, int value)
{
    grid_linked_list_t *new_node=(grid_linked_list_t*)malloc(sizeof(grid_linked_list_t));
    if(!new_node)
        return NULL;
    new_node->grid=grid;
    new_node->value=value;
    new_node->next=L;
    return new_node;
}


grid_linked_list_t* add_grid_tail_get_tail(grid_linked_list_t *L, grid_t grid, int value)
{
    grid_linked_list_t *new_node=(grid_linked_list_t*)malloc(sizeof(grid_linked_list_t));
    if(!new_node)
        return NULL;
    new_node->grid=grid;
    new_node->value=value;
    new_node->next=NULL;
    if(!L)
        return new_node;
    grid_linked_list_t *current=L;
    while(current->next)
        current=current->next;
    current->next=new_node;
    return new_node;
}

void push_element_grid_queue(grid_queue_t * queue, grid_t grid, int value)
{
    if(!queue->head)
    {
        queue->head=add_grid_head(queue->head,grid,value);
        queue->tail=queue->head;
    }
    else
        queue->tail=add_grid_tail_get_tail(queue->tail,grid,value);
    queue->size++;
}

void pop_element_grid_queue(grid_queue_t *queue, grid_t *grid, int *value)
{
    *grid=queue->head->grid;
    *value=queue->head->value;
    queue->head=remove_grid_head(queue->head);
    queue->size--;
}


int minimum_moves(grid_t G,int64_t mod, int64_t capacity)
{
    grid_map_t mapper=init_grid_map(mod,capacity);
    grid_linked_list_t *L = mapper.hashmap[G.content%capacity];
    upsert_result_t result;
    result=upsert_grid_element(&L,G,0);
    L=result.list;
    //Seems to be uniform enough
    mapper.hashmap[G.content%capacity]=L;
    grid_queue_t Q=init_grid_queue();
    push_element_grid_queue(&Q,G,0);
    grid_t grid;
    int value;
    bool found=false;
    while(!is_grid_queue_empty(&Q))
    {
        pop_element_grid_queue(&Q,&grid,&value);
        for(int i=0;i<grid.n;i++) for(int j=0;j<grid.m;j++)
        {
            grid_t new_grid=crash_cell(grid,i,j);
            if(is_empty_grid(new_grid))
            {
                found=true;
                value++;
                goto clear;
            }
            grid_linked_list_t *search_result=search_grid_element(mapper.hashmap[new_grid.content%capacity],new_grid);
            if(!search_result)
            {
                result=upsert_grid_element(&mapper.hashmap[new_grid.content%capacity],new_grid,value+1);
                push_element_grid_queue(&Q,new_grid,value+1);
            }
        }
    }
    clear:
    destroy_grid_queue(&Q);
    destroy_grid_map(&mapper);
    if(!found)
        return -1;
    return value;
}

const int prime=1000003;

int main()
{
    char grid_string[N_max][M_max+1];
    int n,m;
    scanf("%d %d",&n,&m);
    for(int i=0;i<n;i++)
    {
        fscanf(stdin,"%s",grid_string[n-1-i]);
    }
    grid_t G;
    set_color_with_fixed_string(&G,grid_string,n,m);
    int moves= minimum_moves(G,prime,prime);
    printf("%d\n",moves);
}
