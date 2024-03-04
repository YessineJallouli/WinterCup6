#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

//This does not use qsort, just an exercise in implementing a sorting algorithm
typedef long long integer;

bool merge_array(integer *data, integer offset, integer size)
{
    integer *tmp = (integer*)malloc(sizeof(integer)*size);
    if(!tmp)
        return true;
    memcpy(tmp,data,sizeof(integer)*size);
    integer *A=tmp,*B=tmp+offset,*endA=tmp+offset,*endB=tmp+size;
    while(A!=endA && B!=endB)
    {
        if(*A<*B)
        {
            *data=*A;
            ++A;
        }
        else
        {
            *data=*B;
            ++B;
        }
        ++data;
    }
    while(A!=endA)
    {
        *data=*A;
        ++A;
        ++data;
    }
    while(B!=endB)
    {
        *data=*B;
        ++B;
        ++data;
    }
    free(tmp);
    return false;
}

bool merge_sort(integer *data, int size)
{
    if(size <= 1)
        return false;
    if(merge_sort(data,size/2))
        return true;
    if(merge_sort(data+size/2,size-size/2))
        return true;
    if(merge_array(data,size/2,size))
        return true;
    return false;
}
integer op(integer id,integer K)
{
    switch(id)
    {
        case 0:
        case 1:
            return K;
        case 2:
        case 3:
            return 0;
        case 4:
        case 5:
            return -K;
    }
}
integer maximal_difference(integer **content, int size,integer K)
{
    integer max=INT64_MIN,min=INT64_MAX;
    for(int i=0;i<size;i++) if(content[i])
    {
        if(*content[i] + op(i,K) > max)
            max=*content[i] + op(i,K);
        if(*content[i] + op(i,K) <min)
            min=*content[i] + op(i,K);
    }
    return max-min;
}

integer min(integer a,integer b)
{
    return a<b?a:b;
}

int main()
{
    int T;
    scanf("%d",&T);
    while(T--)
    {
        int n,K;
        scanf("%d %d",&n,&K);
        integer *data=(integer*)malloc(sizeof(integer)*n);
        for(int i=0;i<n;i++)
            scanf("%lld",data+i);
        merge_sort(data,n);
        integer delta=INT64_MAX;
        for(int i=0;i<=n;i++) for(int j=0;j<=n;j++)
        {
            integer* content[6];
            for(int k=0;k<6;k++)
                content[k]=NULL;
            if(i)
            {
                content[0]=data;
                content[1]=data+i-1;
            }
            if(i<j)
            {
                content[2]=data+i;
                content[3]=data+j-1;
            }
            if(j<n)
            {
                content[4]=data+j;
                content[5]=data+n-1;
            }
            delta=min(delta,maximal_difference(content,6,K));
        }
        printf("%lld\n",delta);
        free(data);
    }

    return 0;
}