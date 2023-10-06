//
// Created by ramizouari on 04/10/23.
//
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include "nt/number_theory.h"


std::ostream& query(std::ostream &H,int a,int i)
{
    return H << "? " << a << ' ' << i << std::endl;

}

std::pair<integer,int> largest_prime(int n,std::mt19937_64  &rng,factoriser &F)
{
    std::uniform_int_distribution<int> d(1,n);
    int a=d(rng);
    integer s;
    std::pair<integer,int> P;
    int o;
    for(int i=1;i<=n;i++) if(i!=a)
    {
        query(std::cout,a,i);
        std::cin >> s;
        P = std::max(P,std::make_pair(s,i));
    }
    P.first =F.prime_factors(P.first).back();
    return P;
}

int main()
{
    using integer=std::int64_t;
    using couple=std::pair<integer,int>;
    std::random_device dev;
    std::mt19937_64 rng(15);
    int n;
    std::cin >> n;
    factoriser F(n);
    auto [p,o] = largest_prime(n,rng,F);
    std::vector<int> P(n);
    P[o-1]=p;
    for(int i=1;i<=n;i++) if(i!=o)
    {
        query(std::cout,o,i);
        std::cin >> P[i-1];
        P[i-1]/=p;
    }
    std::cout << "! ";
    for(auto p:P)
        std::cout << p << ' ';
    std::cout << std::endl;
}