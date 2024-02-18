#include <iostream>

//Use this function for a asking the LCM of P[i] and P[j]
long long query(int i, int j)
{
    long long response;
    std::cout << "? " << i << ' ' << j << std::endl;
    std::cin >> response;
    return response;
}

//Use this function to print the guessed permutation
void answer(const long long sum)
{
    std::cout << "! " << sum << std::endl;
}

int main()
{
    long long sum=0;
    int n;
    std::cin >> n;
    for(int i=0;i<n-1;i++)
        sum += query(i+1,(i+1)%n+1);
    answer(sum>>1);
}