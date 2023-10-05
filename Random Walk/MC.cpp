//
// Created by ramizouari on 05/10/23.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <cmath>
using real=long double;

struct Graph
{
    int n;
    std::vector<std::vector<std::pair<int,double>>> adjacencyList,reverseList;
protected:

public:
    Graph(int _n):n(_n),adjacencyList(n),reverseList(n){}
    void connect(int a,int b, real weight)
    {
        adjacencyList[a].emplace_back(b,weight);
        reverseList[b].emplace_back(a,weight);
    }


};

double simulation(const Graph& G, int a,int b, int limit,std::mt19937_64  &rng)
{
    int i=0;
    while(a!=b && i < limit)
    {
        std::vector<double> A,B;
        for(auto [c,w]:G.adjacencyList[a])
            A.push_back(w);
        for(auto [c,w]:G.adjacencyList[b])
            B.push_back(w);
        std::discrete_distribution d1(A.begin(),A.end()),d2(B.begin(),B.end());
        a=G.adjacencyList[a][d1(rng)].first;
        b=G.adjacencyList[b][d2(rng)].first;
        i++;
    }
    if(i==limit)
        return std::numeric_limits<double>::infinity();
    else
        return i;
}


constexpr int L=100;

int main()
{
    int n, m;
    int s, t;
    std::cin >> n >> m >> s >> t;
    Graph G(n);
    real inv100=1./100;
    //std::cout << inv100;
    std::random_device dev;
    std::mt19937_64 g(dev());
    for (int i = 0; i < m; i++)
    {
        int a, b;
        real p;
        std::cin >> a >> b >> p;
        if(p!=0)
            G.connect(a,b,p);
    }
    std::vector<double> E(L);
    for(int i=0;i<L;i++)
    {
        E[i] = simulation(G, s, t, 100000, g);
        if(E[i]==std::numeric_limits<double>::infinity())
            break;
    }
    auto expected_value=std::accumulate(E.begin(),E.end(),0.0)/L;
    std::cout << expected_value << std::endl;
}