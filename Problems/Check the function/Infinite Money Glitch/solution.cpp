#include <iostream>
#include "graph/graph.h"
#include <random>
#include <chrono>

using real=double;

struct Conversion
{
    real rate, fee;
};

using GraphType=cp::graph::WeightedGraph<Conversion>;


real bellman_ford(GraphType &G, int s, real x)
{
    std::vector<real> D(G.size(), 0);
    D[s] = x;
    for (int i = 0; i < G.size(); i++) for (int u = 0; u < G.size(); u++) for (auto [v, conversion] : G.adjacencyList[u])
        D[v] = std::max(D[v], (D[u] - conversion.fee) * conversion.rate);
    return D[s];
}

int minimal_investment(GraphType &G, int s,int x)
{
    int a=0,b=x+1,c;
    while(a<b)
    {
        c=(a+b)/2;
        if(bellman_ford(G,s,c)<=c)
            a=c+1;
        else
            b=c;
    }
    return a;
}

real bellman_ford_growth_upper_bound(GraphType &G, int s)
{
    std::vector<double> D1(G.size(), 0),D2(G.size(),0);
    D1[s] = 1;
    for (int i = 0; i < G.size(); i++)
    {
        std::copy(D1.begin(),D1.end(),D2.begin());
        for (int u = 0; u < G.size(); u++)
            for (auto [v, conversion]: G.adjacencyList[u])
                D2[v] = std::max(D2[v], D1[u] * conversion.rate);
        std::swap(D1, D2);
    }
    return D1[s];
}

real bellman_ford_growth_lower_bound(GraphType &G, int s)
{
    std::vector<double> D1(G.size(), std::numeric_limits<double>::infinity()),D2(G.size(), std::numeric_limits<double>::infinity());
    D1[s] = 1;
    for (int i = 0; i < G.size(); i++)
    {
        std::copy(D1.begin(),D1.end(),D2.begin());
        for (int u = 0; u < G.size(); u++)
            for (auto [v, conversion]: G.adjacencyList[u])
                D2[v] = std::min(D2[v], D1[u] * conversion.rate);
        std::swap(D1, D2);
    }
    return D1[s];
}

int main(int argc,char **argv)
{
    int n,m,x;
    std::cin >> n >> m >> x;
    GraphType G(n);
    if(argc < 2) for(int i=0;i<m;i++)
    {
        int a,b;
        Conversion conversion;
        std::cin >> a >> b >> conversion.rate >> conversion.fee;
        a--;
        b--;
        G.connect(a,b,conversion);
    }
    else
    {
        std::uint64_t seed;
        if(std::string_view(argv[1])=="random")
        {
            std::random_device rd;
            seed=rd();
        }
        else
            seed=std::stoull(argv[1]);
        std::mt19937_64 rng(seed);
        std::uniform_real_distribution<real> rate(0.95,1.05);
        std::uniform_int_distribution<int> node(0,n-1);
        std::uniform_real_distribution<real> fee(0,1);
        for(int i=0;i<m;i++)
        {
            int a,b;
            Conversion conversion;
            a=node(rng);
            b=node(rng);
            conversion.rate=rate(rng);
            conversion.fee=fee(rng);
            G.connect(a,b,conversion);
        }

    }
    auto t1=std::chrono::high_resolution_clock::now();
    auto z=minimal_investment(G,0,x);
    auto t2=std::chrono::high_resolution_clock::now();
    std::cout << (z>x?-1:z) << std::endl;
    std::cerr << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "ms" << std::endl;
    std::cout << "Growth upper bound: " << std::scientific << bellman_ford_growth_upper_bound(G,0) << std::endl;
    std::cout << "Growth lower bound: " << std::scientific << bellman_ford_growth_lower_bound(G,0) << std::endl;
}