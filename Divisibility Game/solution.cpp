//
// Created by ramizouari on 05/10/23.
//

#include <iostream>
#include <vector>


int main()
{
    int T=1;
//    std::cin >> T;
    while(T--)
    {
        int n,k;
        std::cin >> n >> k;
        std::vector<int> A(n);
        for(auto &a:A) std::cin >> a;
        int r=0;
        for(auto a:A) if(a%k==0)
            r++;
        bool yessine_wins =  r==n-2
                             || (r<n-2 && n%2==0);
        std::cout << (yessine_wins?"Yessine":"Rami") << '\n';
    }
}