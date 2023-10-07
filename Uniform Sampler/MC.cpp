//
// Created by ramizouari on 07/10/23.
//
#include <iostream>
#include <unordered_set>
#include <random>
#include <algorithm>

int sampler_simulation(int n,int m,std::mt19937_64 &rng)
{
    std::uniform_int_distribution d(0,n-1);
    std::unordered_set<int> S;
    int k=0;
    while(S.size()<m)
    {
        k++;
        S.insert(d(rng));
    }
    return k;
}
constexpr int L=1000;
int main()
{
    int T;
    std::cin >> T;
    while(T--)
    {
        int n,m;
        std::cin >> n >> m;
        std::random_device dev;
        std::mt19937_64 rng(dev());
        std::vector<int> S(L);
        for(auto &s:S)
            s= sampler_simulation(n,m,rng);
        auto E=std::accumulate(S.begin(),S.end(),0.L)/S.size();
        std::cout << E << '\n';
    }
}