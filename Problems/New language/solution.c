#include <stdio.h>

int main()
{
    int T;
    scanf("%d",&T);
    while(T--)
    {
        int n;
        scanf("%d",&n);
        int sum=0;
        while(n--)
        {
            int a;
            scanf("%d",&a);
            sum+=1<<a;
        }
        printf("%d\n",sum);
    }
}