#include <iostream>
#include <vector>
#include <numeric>

int main()
{
    int T;
    std::cin >> T;
    while(T--)
    {
        int n;
        std::cin >> n;
        int sum=0;
        while(n--)
        {
            int a;
            std::cin >> a;
            sum+=1<<a;
        }
        std::cout << sum << '\n';
    }

}