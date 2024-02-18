#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

bool is_prime(int n)
{
    if(n<2)
        return false;
    int L=(int)ceil(sqrt(n));
    for(int i=2;i<=L;i++) if(n%i==0)
        return false;
    return true;
}

void query(int a,int b)
{
    printf("? %d %d\n",a,b);
    fflush(stdout);
}

typedef long long integer;

typedef enum state_t
{
    unknown,
    other,
    self
} state_t;

int main()
{
    int n;
    scanf("%d",&n);
    integer* answers= (integer*)malloc(n*sizeof(integer));
    int maximal_prime=n;
    while(!is_prime(maximal_prime))
        maximal_prime--;
    state_t state=unknown;
    int prime_pos;
    for(int i=2;i<=n && state!=self;i++)
    {
        query(1, i);
        scanf("%lld",answers+i-1);
        switch (state)
        {
            case unknown:
                if(answers[i-1]%maximal_prime == 0)
                {
                    state = other;
                    prime_pos = i;
                }
                break;
            case other:
                if(answers[i-1]%maximal_prime == 0)
                {
                    state = self;
                    prime_pos = 1;
                }
                break;
        }
    }
    answers[prime_pos-1]=maximal_prime;
    for(int i=1;i<=n;i++) if(i!=prime_pos)
    {
        query(prime_pos,i);
        scanf("%lld",answers+i-1);
        answers[i-1] /= maximal_prime;
    }
    printf("! ");
    for(int i=0;i<n;i++)
        printf("%lld ",answers[i]);
    printf("\n");
    free(answers);
}