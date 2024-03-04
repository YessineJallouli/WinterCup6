#include <stdio.h>

const int N_max=4000;
typedef long long integer;
const integer mod=998244353;
int main()
{
    integer nCr[N_max+1][N_max+1];
    nCr[0][0]=1;
    for(int i=0;i<=N_max;i++) for(int j=0;j<=i;j++)
    {
        nCr[i][j]=0;
        if(i) nCr[i][j]+=nCr[i-1][j];
        if(i && j) nCr[i][j]+=nCr[i-1][j-1];
        nCr[i][j]%=mod;
    }
    int n;
    scanf("%d",&n);
    printf("%lld\n",nCr[n][n/2]);
}