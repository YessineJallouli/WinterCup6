#include <stdio.h>
typedef long long integer;

integer gcd(integer a,integer b)
{
    while(b)
    {
        integer r=a%b;
        a=b;
        b=r;
    }
    return a;
}

integer count_solutions(integer x,integer n,integer d)
{
    integer a = gcd(n,d);
    if(x%a)
        return 0;
    return a;
}
int main()
{
    int Q;
    scanf("%d",&Q);
    for(int s=0;s<Q;s++)
    {
        int n,m,x,y,d;
        scanf("%d %d %d %d %d",&n,&m,&x,&y,&d);
        integer a=count_solutions(x,n,d),b=count_solutions(y,m,d);
        printf("%lld\n",a*b);
    }
}