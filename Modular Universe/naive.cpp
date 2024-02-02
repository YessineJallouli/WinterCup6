#include <iostream>
#include <cstdint>
using integer=std::int64_t;

int main()
{
    std::ios_base::sync_with_stdio(false);
    int Q;
    std::cin >> Q;
    for(int s=0;s<Q;s++)
    {
        integer n,m,x,y,d;
        std::cin >> n >> m  >> x >> y >> d;
        integer R=0;
        for(integer i=0;i<n;i++) for(integer j=0;j<m;j++) if(i*d%n == x && i*d%m == y)
                R++;
        std::cout << R << '\n';
    }
}