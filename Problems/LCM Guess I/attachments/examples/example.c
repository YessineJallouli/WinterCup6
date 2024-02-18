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
void answer(int *permutation, int n)
{
    printf("! ");
    for(int i=0;i<n;i++)
        printf("%d ",permutation[i]);
    printf("\n");   
    fflush(stdout);
}

int main()
{
	int n;
	scanf("%d", &n);
	int permutation[n];
	//Permutation is unknown, initialize it to zero. You have to find the permutation!
	for(int i=0;i<n;i++)
		permutation[i]=0;
		
	/*
		Your code here. Feel free to modify any part of this file, or even change it completely.
	*/
}

