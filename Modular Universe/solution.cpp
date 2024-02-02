#include <iostream>
#include <cstdint>
#include <numeric>

using integer=std::int64_t;
int main()
{
    std::ios_base::sync_with_stdio(false);
    int Q;
    std::cin >> Q;
    for(int s=0;s<Q;s++)
    {
        int n,m,x,y,d;
        std::cin >> n >> m  >> x >> y >> d;
        auto a=std::gcd(n,d),b=std::gcd(m,d);
        if(x%a || y%b)
            std::cout << 0 << '\n';
        else std::cout << a*b << '\n';
    }
}