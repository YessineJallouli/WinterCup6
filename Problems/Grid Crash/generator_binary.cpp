//
// Created by ramizouari on 06/10/23.
//
#include <iostream>
#include <random>
#include <set>

int main(int argc,char**argv)
{
    if(argc<4)
    {
        std::cerr << "Usage: " << argv[0] << " n m C [seed]\n";
        return 1;
    }
    int n,m,C;
    n=std::stoi(argv[1]);
    m=std::stoi(argv[2]);
    C=std::stoi(argv[3]);
    std::uint64_t seed;
    if(argc>=5)
        seed=std::stoull(argv[4]);
    else
        seed=std::random_device{}();
    if(C>2)
    {
        std::cerr << "C must be 1 or 2.\n";
        return 1;
    }
    std::set<char> S;
    std::mt19937_64 g(seed);
    std::uniform_int_distribution<char> d('0','1');
    while(S.size()<C)
        S.insert(d(g));
    std::vector<char> alphabet(S.begin(),S.end());
    std::uniform_int_distribution<int> d2(0,C-1);
    std::cout << n << ' ' << m << '\n';
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
            std::cout << alphabet[d2(g)];
        std::cout << '\n';
    }
}//
// Created by ramizouari on 18/10/23.
//
