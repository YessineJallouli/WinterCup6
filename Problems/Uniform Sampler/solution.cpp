//
// Created by ramizouari on 06/10/23.
//

#include <iostream>
#include <vector>
using real=double;
constexpr int L=1e6;
int main()
{
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    std::vector<real> H(L+1);
    for(int i=1;i<=L;i++)
        H[i]=H[i-1]+1.0/i;
    while(T--)
    {
        int n,m;
        std::cin >> n >> m;
        std::cout << std::fixed;
        std::cout.precision(6);
        std::cout << n*(H[n]-H[n-m]) << '\n';
    }
}