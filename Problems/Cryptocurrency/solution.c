#include <stdio.h>
#include <stdlib.h>
int max(int a,int b)
{
    return a>b?a:b;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--)
    {
        int n;
        scanf("%d",&n);
        int* A=(int*)malloc(n*sizeof(int));
        if(!A)
            return EXIT_FAILURE;
        for(int i=0;i<n;i++)
            scanf("%d",A+i);
        int i=0;
        int max_inc=0;
        while(i<n)
        {
            int j=i;
            while(j+1<n && A[j+1]>=A[j])
                j++;
            max_inc=max(max_inc,A[j]-A[i]);
            i=j+1;
        }
        printf("%d\n",max_inc);
        free(A);
    }
}