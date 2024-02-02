//
// Created by ramizouari on 06/02/23.
//
#include <iostream>

using real=long double;
constexpr real p=0.6;
int main()
{
    std::cout.precision(5);
    char c;
    int L;
    std::cin >> L;
    for(int i=0;i<L;i++)
    {
        std::cin >> c;
        if(c!='G')
            return c;
        real S=0;
        while(S<=p)
        {
            std::cout << 1 << std::endl;
            std::cin >> c;
            if(c!='S')
                break;
            std::cin >> S;
        }
        if(c == 'S') {
            std::cout << 0 << std::endl;
            std::cin >> c;
        }

    }
}
