#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#define FIRST "Rami"
#define SECOND "Yessine"
int main()
{
    int n,k;
    scanf("%d %d",&n,&k);
    int *A=(int*)malloc(sizeof(int)*n);
    if(!A) return EXIT_FAILURE;
    for(int i=0;i<n;i++) scanf("%d",A+i);
    for(int i=0;i<n;i++) A[i]%=k;
    int nb_zeros=0;
    for(int i=0;i<n;i++) if(A[i]==0) nb_zeros++;
    if(nb_zeros==n)
        printf(SECOND);
    else if(nb_zeros==n-2)
        printf(FIRST);
    else
        printf(n%2?FIRST:SECOND);
    free(A);
}