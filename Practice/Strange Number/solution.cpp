#include <iostream>
#include <cmath>


bool is_prime(int n)
{
    int m=std::ceil(std::sqrt(n));
    for(int i=2;i<=m;i++) if(n%i==0)
        return false;
    return true;
}

int main()
{
    int n;
    std::cin >> n;
    n++;
    while(!is_prime(n))
        n++;
    std::cout << n << '\n';
}