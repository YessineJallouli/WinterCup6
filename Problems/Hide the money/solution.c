#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef long long integer;

typedef struct point
{
    int x,y;
} point;

typedef struct priority_queue
{
    point *data;
    int size,capacity;
} priority_queue;

void swap(point *a,point *b)
{
    point aux=*a;
    *a=*b;
    *b=aux;
}

integer S(integer x,integer n)
{
    return n*(n-1)/2-(n-1-x)*x;
}

integer cost(point p,integer n,integer m)
{
    return m*S(p.x,n)+n*S(p.y,m);;
}

integer priority(point p,integer n,integer m)
{
    return -cost(p,n,m);
}

priority_queue init_priority_queue(int capacity)
{
    priority_queue Q;
    Q.data=(point*)malloc(sizeof(point)*capacity);
    Q.size=0;
    if(!Q.data)
    {
        Q.capacity=0;
        return Q;
    }
    Q.capacity=capacity;
    return Q;
}

void free_priority_queue(priority_queue *Q)
{
    free(Q->data);
    Q->capacity=0;
    Q->size=0;
}

bool push(priority_queue *Q,point p,integer n,integer m)
{
    if(Q->size==Q->capacity)
    {
        Q->capacity*=2;
        if(!Q->capacity)
            Q->capacity=1;
        point* new_data=(point*)realloc(Q->data,Q->capacity*sizeof(point));
        if(!new_data)
            return true;
        Q->data=new_data;
    }
    Q->data[Q->size++]=p;
    int current=Q->size-1;
    while(current)
    {
        int parent=(current-1)/2;
        if(priority(Q->data[current],n,m)<priority(Q->data[parent],n,m))
        {
            swap(&Q->data[current],&Q->data[parent]);
            current=parent;
        }
        else break;
    }
    return false;
}

bool empty(priority_queue *Q)
{
    return Q->size==0;
}

point pop(priority_queue *Q,integer n,integer m)
{
    point result=Q->data[0];
    Q->data[0]=Q->data[--Q->size];
    int current=0;
    while(1)
    {
        int left=2*current+1,right=2*current+2;
        if(left>=Q->size)
            break;
        int next=current;
        if(priority(Q->data[left],n,m)<priority(Q->data[next],n,m))
            next=left;
        if(right<Q->size && priority(Q->data[right],n,m)<priority(Q->data[next],n,m))
            next=right;
        if(next==current)
            break;
        swap(&Q->data[current],&Q->data[next]);
        current=next;
    }
    return result;
}

typedef struct point_linked_list_t
{
    point p;
    struct point_linked_list_t *next;
} point_linked_list_t;


point_linked_list_t* init_point_linked_list()
{
    return NULL;
}

point_linked_list_t* push_point_head(point_linked_list_t *list,point p)
{
    point_linked_list_t *new_node=(point_linked_list_t*)malloc(sizeof(point_linked_list_t));
    new_node->p=p;
    new_node->next=list;
    return new_node;
}

point_linked_list_t* pop_point_head(point_linked_list_t *list,point *p)
{
    if(!list)
        return NULL;
    *p=list->p;
    point_linked_list_t *next=list->next;
    free(list);
    return next;
}

void destroy_point_linked_list(point_linked_list_t *list)
{
    while(list)
    {
        point_linked_list_t *next=list->next;
        free(list);
        list=next;
    }
}

point_linked_list_t* find_point(point_linked_list_t *list,point p)
{
    while(list)
    {
        if(list->p.x==p.x && list->p.y==p.y)
            return list;
        list=list->next;
    }
    return NULL;
}


typedef struct point_hashset
{
    point_linked_list_t **data;
    int capacity;
    integer mod;
} point_hashset;

const integer z=2654435761u;

integer point_hash(point p,integer mod)
{
    return (p.x*z+p.y)%mod;
}

point_hashset init_point_hashset(int capacity)
{
    point_hashset result;
    result.data=(point_linked_list_t**)malloc(sizeof(point_linked_list_t*)*capacity);
    result.capacity=capacity;
    for(int i=0;i<capacity;i++)
        result.data[i]=NULL;
    return result;
}

void insert_point(point_hashset *set,point p)
{
    integer index=point_hash(p,set->capacity);
    set->data[index]=push_point_head(set->data[index],p);
}

bool contains_point(point_hashset *set,point p)
{
    integer index=point_hash(p,set->capacity);
    return find_point(set->data[index],p)!=NULL;
}

void free_point_hashset(point_hashset *set)
{
    for(int i=0;i<set->capacity;i++)
        destroy_point_linked_list(set->data[i]);
    free(set->data);
    set->capacity=0;
}

const int mods[]={7,97,1009,10487,100003,1000003};

int find_mod(integer n, double max_density)
{
    for(int i=0;i<6;i++)
    {
        if(n<max_density*mods[i])
            return mods[i];
    }
    return 1000003;
}

int main()
{
    int T;
    scanf("%d",&T);
    while(T--)
    {
        integer n,m,k;
        {
            int n_,m_,k_;
            scanf("%d %d %d",&n_,&m_,&k_);
            n=n_;
            m=m_;
            k=k_;
        }
        int mod=find_mod(k,2);
        point_hashset visited=init_point_hashset(mod);
        priority_queue Q=init_priority_queue(k+4);
        push(&Q,(point){0,0},n,m);
        push(&Q,(point){n-1,0},n,m);
        push(&Q,(point){0,m-1},n,m);
        push(&Q,(point){n-1,m-1},n,m);
        insert_point(&visited,(point){0,0});
        insert_point(&visited,(point){n-1,0});
        insert_point(&visited,(point){0,m-1});
        insert_point(&visited,(point){n-1,m-1});
        integer total_cost=0;
        integer count=0;
        while(!empty(&Q) && count < k)
        {
            point p=pop(&Q,n,m);
            total_cost+=cost(p,n,m);
            count++;
            for(int i=-1;i<=1;i++) for(int j=-1;j<=1;j++) if(p.x+i>=0 && p.x+i<n && p.y+j>=0 && p.y+j<m)
            {
                point new_point={p.x+i,p.y+j};
                if(!contains_point(&visited,new_point))
                {
                    insert_point(&visited,new_point);
                    push(&Q,new_point,n,m);
                }
            }
        }
        printf("%lld\n",total_cost);
        free_point_hashset(&visited);
        free_priority_queue(&Q);

    }
}