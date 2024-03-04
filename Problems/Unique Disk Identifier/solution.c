#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>

typedef struct sieve_t
{
    int n;
    int *smallest_divisor;
    bool *is_prime;
    int *primes;
    int nb_primes;
}sieve_t;

typedef long long integer;

sieve_t generate_sieve(int n)
{
    sieve_t sieve;
    sieve.n=n;
    sieve.smallest_divisor=(int*)malloc(sizeof(int)*(n+1));
    memset(sieve.smallest_divisor,0,sizeof(int)*(n+1));
    sieve.is_prime=(bool*)malloc(sizeof(bool)*(n+1));
    memset(sieve.is_prime,true,sizeof(bool)*(n+1));
    sieve.primes=(int*)malloc(sizeof(int)*(n+1));
    sieve.nb_primes=0;
    sieve.is_prime[0]=sieve.is_prime[1]=false;
    for(int i=2;i<=n;i++) if(sieve.is_prime[i])
    {
        sieve.smallest_divisor[i]=i;
        sieve.primes[sieve.nb_primes]=i;
        sieve.nb_primes++;
        for(int j=i*2;j<=n;j+=i)
        {
            sieve.is_prime[j]=false;
            if(sieve.smallest_divisor[j]==0)
                sieve.smallest_divisor[j]=i;
        }
    }
    return sieve;
}

int smallest_divisor(int n,sieve_t *sieve)
{
    if(n<=sieve->n)
        return sieve->smallest_divisor[n];
    int L=(int)ceil(sqrt(n));
    for(int i=0;i<sieve->nb_primes;i++)
    {
        if(sieve->primes[i]>L)
            break;
        else if(n%sieve->primes[i]==0)
            return sieve->primes[i];
    }
    return n;
}

typedef struct int_array_t
{
    int* data;
    int capacity;
} int_array_t;

typedef struct long_array_t
{
    integer* data;
    int capacity;
} long_array_t;

typedef struct prime_dec_t
{
    int* primes;
    int* exponents;
    int size;
} prime_dec_t;

prime_dec_t prime_decomposition(int n,sieve_t *sieve)
{
    prime_dec_t result;
    result.primes=(int*)malloc(sizeof(int)*(int)ceil(log2(n)));
    result.exponents=(int*)malloc(sizeof(int)*(int)ceil(log2(n)));
    result.size=0;
    while(n>1)
    {
        int p=smallest_divisor(n,sieve);
        if(result.size==0||result.primes[result.size-1]!=p)
        {
            result.primes[result.size]=p;
            result.exponents[result.size]=1;
            result.size++;
        }
        else
            result.exponents[result.size-1]++;
        n/=p;
    }
    return result;
}

void destroy_prime_dec(prime_dec_t *prime_dec)
{
    free(prime_dec->primes);
    free(prime_dec->exponents);
}

void generate_divisors_rec(int n, prime_dec_t* primeDec,int_array_t *divisors,int * size, int index)
{
    divisors->data[*size]=n;
    (*size)++;
    for(int i=index;i<primeDec->size;i++) if(n%primeDec->primes[i] == 0)
        generate_divisors_rec(n/primeDec->primes[i],primeDec,divisors,size,i);
}

int count_divisors(int n, sieve_t* sieve)
{
    prime_dec_t primeDec=prime_decomposition(n,sieve);
    int result=1;
    for(int i=0;i<primeDec.size;i++)
        result*=(primeDec.exponents[i]+1);
    destroy_prime_dec(&primeDec);
    return result;
}

int_array_t generate_divisors(int n,sieve_t * sieve)
{
    int_array_t divisors;
    int d=count_divisors(n,sieve);
    divisors.data=(int*)malloc(sizeof(int)* d);
    divisors.capacity=d;
    int size=0;
    prime_dec_t primeDec=prime_decomposition(n,sieve);
    generate_divisors_rec(n,&primeDec,&divisors,&size,0);
    destroy_prime_dec(&primeDec);
    return divisors;
}

int_array_t init_int_array(int capacity)
{
    int_array_t int_array;
    int_array.data=(int*)malloc(sizeof(int)*capacity);
    int_array.capacity=capacity;
    return int_array;
}

long_array_t init_long_array(int capacity)
{
    long_array_t long_array;
    long_array.data=(integer*)malloc(sizeof(integer)*capacity);
    long_array.capacity=capacity;
    return long_array;

}

void destroy_long_array(long_array_t* long_array)
{
    free(long_array->data);
}

void destroy_int_array(int_array_t* int_array)
{
    free(int_array->data);
}

void destroy_sieve(sieve_t* sieve)
{
    free(sieve->smallest_divisor);
    free(sieve->is_prime);
    free(sieve->primes);
}

const int L=1e6;

const int64_t M=998244353;

int64_t pow_mod(int64_t a,int64_t b,int64_t m)
{
    int64_t result=1;
    while(b)
    {
        if(b%2)
            result=(result*a)%m;
        a=(a*a)%m;
        b/=2;
    }
    return result;
}

int64_t inv_mod_prime(int64_t a,int64_t m)
{
    return pow_mod(a,m-2,m);
}

int totient(int n,sieve_t *sieve)
{
    int result=n;
    prime_dec_t primeDec=prime_decomposition(n,sieve);
    for(int i=0;i<primeDec.size;i++)
        result-=result/primeDec.primes[i];
    destroy_prime_dec(&primeDec);
    return result;
}

int64_t inv_mod(int64_t a,int64_t m,sieve_t *sieve)
{
    return pow_mod(a,totient(m,sieve)-1,m);
}

long_array_t inverse_table(int n,int64_t m)
{
    long_array_t result=init_long_array(n+1);
    result.data[1]=1;
    for(int i=2;i<=n;i++)
        result.data[i]=(m-m/i)*result.data[m%i]%m;
    return result;
}

int64_t sector_colours_count_with_global_reverse_1(int64_t m,int64_t K,sieve_t *sieve, long_array_t *K_pow,long_array_t *I, long_array_t *cache)
{
    if(cache->data[m]!=0)
        return cache->data[m];
    if(m<=2)
    {
        if(m==1) return K;
        else return (K*(K+1)/2)%M;
    }
    int_array_t divisors= generate_divisors(m,sieve);
    int64_t R=0;
    for(int i=0;i<divisors.capacity;i++)
    {
        int d=divisors.data[i];
        R=(R+totient(m/d,sieve)*K_pow->data[d])%M;
     }
    R=(R*I->data[m])%M;
    destroy_int_array(&divisors);
    return cache->data[m]=R;
}

int64_t sector_colours_count_with_global_reverse_2(int64_t m,int64_t K,long_array_t *K_pow,long_array_t *I)
{
    switch (m)
    {
        case 1:
            return K;
        case 2:
            return (K*(K+1)/2)%M;
        default:
            if(m%2==0)
                return ((K_pow->data[m/2]*(K+1))%M*I->data[2])%M;
            else
                return K_pow->data[(m+1)/2];
    }
}


int64_t sector_colours_count_with_global_reverse(struct long_array_t *A,int64_t K,sieve_t *sieve,long_array_t *K_pow,long_array_t *I,long_array_t *cache)
{
    int64_t R1=1,R2=1;
    for(int i=0;i<A->capacity;i++)
    {
        R1=(R1*sector_colours_count_with_global_reverse_1(A->data[i],K,sieve,K_pow,I,cache))%M;
        R2=(R2*sector_colours_count_with_global_reverse_2(A->data[i],K,K_pow,I))%M;
    }
return ((R1+R2)%M*I->data[2])%M;
}



int main()
{
    sieve_t sieve=generate_sieve(L);
    integer n, K;
    {
        int n_,K_;
        scanf("%d %d", &n_, &K_);
        n=n_;
        K=K_;
    }
    long_array_t S=init_long_array(n);
    long_array_t K_pow= init_long_array(L+1),I= inverse_table(L,M),cache=init_long_array(L+1);
    memset(cache.data,0,sizeof(int64_t)*cache.capacity);
    for(int i=0;i<n;i++)
    {
        int in;
        scanf("%d", &in);
        S.data[i]=in;
    }
    K_pow.data[0]=1;
    K_pow.data[1]=K;
    for(int i=2;i<=L;i++)
        K_pow.data[i]=(K_pow.data[i-1]*K)%M;
    printf("%ld\n",sector_colours_count_with_global_reverse(&S,K,&sieve,&K_pow,&I,&cache));
    destroy_long_array(&K_pow);
    destroy_long_array(&I);
    destroy_long_array(&cache);
    destroy_long_array(&S);
    destroy_sieve(&sieve);
}