//
// Created by ramizouari on 06/10/23.
//

#include <iostream>
#include <vector>
#include <algorithm>
using real=long double;
constexpr int L=1e6+1;
int main()
{
    int T;
    std::cin >> T;
    std::vector<real> H(L);
    for(int i=1;i<=L;i++)
        H[i]=H[i-1]+1.0/i;
    while(T--)
    {
        int n,m;
        std::cin >> n >> m;
        std::cout << std::fixed;
        std::cout.precision(4);
        std::cout << n*(H[n]-H[n-m]) << '\n';
    }
}