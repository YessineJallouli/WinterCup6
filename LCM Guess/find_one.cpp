#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <map>


std::ostream& query(std::ostream &H,int a,int i)
{
    return H << "? " << a << ' ' << i << std::endl;

}

using integer=std::int64_t;

std::vector<int> potential_one_position(int n,std::mt19937_64  &rng)
{
    std::uniform_int_distribution<int> d(1,n);
    int a=d(rng);
    std::vector<integer> L(n);
    std::map<integer,std::vector<int>> Q;
    int o;
    for(int i=1;i<=n;i++) if(i!=a)
    {
        query(std::cout,a,i);
        std::cin >> L[i-1];
        if(!Q.count(L[i-1]))
            Q.emplace(L[i-1],std::vector<int>{});
        Q[L[i-1]].push_back(i);
    }
    Q.begin()->second.push_back(a);
    return Q.begin()->second;
}

int main()
{
    using couple=std::pair<integer,int>;
    std::random_device dev;
    std::mt19937_64 rng(15);
    int n;
    std::cin >> n;
    std::vector<int> G;
    do
    {
        if(G.empty())
            G= potential_one_position(n,rng);
        else
        {
            std::vector<int> O=potential_one_position(n,rng);
            auto it = std::set_intersection(G.begin(),G.end(),O.begin(),O.end(),G.begin());
            G.resize(std::distance(G.begin(),it));
        }
    } while (G.size()>1);
    if(G.empty())
        throw std::runtime_error("No one found???");
    auto o=G.front();
    std::vector<int> P(n);
    P[o-1]=1;
    for(int i=1;i<=n;i++) if(i!=o)
    {
        query(std::cout,o,i);
        std::cin >> P[i-1];
    }
    std::cout << "! ";
    for(auto p:P)
        std::cout << p << ' ';
    std::cout << std::endl;
}