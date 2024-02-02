#include <testlib.h>
#include <queue>
#include "graph/graph.h"

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


constexpr int max_graph_size=500;
constexpr int max_edge_count=4000;
constexpr int max_allowed_money=1'000'000'000;
constexpr real min_rate=0.1,max_rate=10;
constexpr real min_fee=0,max_fee=100;
constexpr real max_growth_rate=1e6;

bool is_simple_graph(const GraphType &G)
{
    std::vector<std::vector<bool>> visited(G.size(),std::vector<bool>(G.size()));
    for(int i=0;i<G.size();i++)
        visited[i][i]=true;
    for(int i=0;i<G.size();i++) for(auto [j,_]:G.adjacencyList[i])
    {
        if(visited[i][j])
            return false;
        visited[i][j]=true;
    }
    return true;
}

bool is_simply_connected_graph(const GraphType &G)
{
    std::vector<bool> visited(G.size());
    std::queue<int> Q;
    Q.push(0);
    visited[0]=true;
    while(!Q.empty())
    {
        int u=Q.front();
        Q.pop();
        for(const auto &L:{G.adjacencyList[u],G.reverseList[u]}) for(auto [v,_]:L) if(!visited[v])
        {
            visited[v]=true;
            Q.push(v);
        }
    }
    return std::all_of(visited.begin(),visited.end(),[](bool b){return b;});
}

int main(int argc,char **argv)
{
    registerValidation(argc,argv);
    int n=inf.readInt(2,max_graph_size,"n");
    inf.readSpace();
    int m=inf.readInt(1,max_edge_count,"m");
    inf.readSpace();
    int x=inf.readInt(1,max_allowed_money,"x");
    cp::graph::WeightedGraph<Conversion> G(n);
    inf.readEoln();
    for(int i=0;i<m;i++)
    {
        int a=inf.readInt(1,n,"a");
        inf.readSpace();
        int b=inf.readInt(1,n,"b");
        inf.readSpace();
        real rate=inf.readDouble(min_rate,max_rate,"rate");
        inf.readSpace();
        real fee=inf.readDouble(min_fee,max_fee,"fee");
        ensuref(a!=b,"No self loops");
        inf.readEoln();
        G.connect(a-1,b-1,{rate,fee});
    }
    inf.readEof();
    ensuref(bellman_ford_growth_upper_bound(G,0)<=max_growth_rate,"Conversion rate is too high");
    ensuref(is_simple_graph(G),"Graph is not simple");
    ensuref(is_simply_connected_graph(G),"Graph is not simply connected");
}