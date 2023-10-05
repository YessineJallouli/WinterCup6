//
// Created by ramizouari on 04/10/23.
//
#include <iostream>
#include <queue>
#include <set>
#include <memory>
#include <map>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>

int mex(const std::vector<int> &v)
{
    std::vector<bool> seen(v.size()+1);
    for(int x:v)
        if(x>=0 && x<seen.size())
            seen[x]=true;
    for(int i=0;i<seen.size();i++)
        if(!seen[i])
            return i;
    return seen.size();
}

struct Grundy
{
    std::map<std::vector<int>,int> G;
public:
    int grundy(std::vector<int> &S)
    {
        int k=S.size();
        if(G.count(S))
            return G[S];
        if(std::all_of(S.begin()+1,S.end(),[](int x){return x==0;}))
            return G[S]=0;
        std::vector<int> M;
        for(int i=0;i<k;i++) if(S[i]>0)
        {
            S[i]--;
            for (int j = i; j < k; j++) if (S[j] > 0)
                {
                    S[j]--;
                    S[(i + j) % k]++;
                    M.push_back(grundy(S));
                    S[j]++;
                    S[(i + j) % k]--;
                }
            S[i]++;
        }
        auto m=mex(M);
        G[S]=m;
        /*std::vector<int> H(k);
        for(int i=2;i<=k;i++) if(std::gcd(i,k)==1)
        {
            for (int j = 0; j < k; j++)
                H[(i*j)%k]=S[j];
            G[H]=m;
        }*/
        return m;
    }

    auto operator()(std::vector<int> S)
    {
        return grundy(S);
    }
};


int main()
{
    int n,k,retrial;
    std::random_device dev;
    std::mt19937_64 g(dev());
    std::ofstream file("output2.txt");
    std::cin >> n >> k >> retrial;
    Grundy grundy;
    if(retrial==0)
    {
        std::vector<int> A(n);
        for(auto &a:A)
            std::cin >> a;
        std::vector<int> C(k);
        for(auto a:A)
            C[a%k]++;
        std::cout << grundy(C) << std::endl;
    }
    else while(retrial--) for(int r=2;r<=n;r++) for(int m=k;m>=2 ;m-=2)
    {
        std::vector<int> A(r);
        std::uniform_int_distribution d(0,m-1);
        int S=0;
        for(int i=0;i<r-1;i++)
        {
            A[i]=d(g);
            S+=A[i];
        }
        A[r-1]=m-S%m;
        std::vector<int> C(m);
        for(auto a:A)
            C[a%m]++;
        std::cout << grundy(C) << std::endl;
    }

    file << "Cache: " << grundy.G.size() << std::endl;
    for(auto &[U,c]:grundy.G)
    {

        auto s=std::accumulate(U.begin(),U.end(),0);
        if(s%2==0 && U[0]<s && c==0 || c>0 && U[0]<s && s%2==1)
        {
            std::cout << "Found: " << "G=" << c << ".U=";
            for (auto u: U)
                std::cout << u << ' ';
            std::cout << '\n';
        }
        file << '[';
        for(auto u:U)
            file << u << ",";
        file << "]: " << c << '\n';
    }

}