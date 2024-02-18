#include <stdio.h>

//Use this function for a asking the LCM of P[i] and P[j]
long long query(int a,int b)
{
    printf("? %d %d\n",a,b);
    fflush(stdout);
    long long response;
    scanf("%lld",&response);
    return response;
}

//Use this function to print the guessed permutation
void answer(long long sum)
{
    printf("! %d\n",sum);
    fflush(stdout);
}

int main()
{
	int n;
	scanf("%d", &n);
	long long sum=0;
		
	/*
		Your code here. Feel free to modify any part of this file, or even change it completely.
	*/
}

