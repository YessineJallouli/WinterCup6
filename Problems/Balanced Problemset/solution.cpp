#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>

template<typename T>
std::strong_ordering compare_optional_1(const std::optional<T> &A,const std::optional<T> &B)
{
    if(A.has_value() && B.has_value())
        return *A<=>*B;
    else if(A.has_value())
        return std::strong_ordering::greater;
    else if(B.has_value())
        return std::strong_ordering::less;
    else
        return std::strong_ordering::equal;
}

template<typename T>
std::strong_ordering compare_optional_2(const std::optional<T> &A,const std::optional<T> &B)
{
    if(A.has_value() && B.has_value())
        return *A<=>*B;
    else if(A.has_value())
        return std::strong_ordering::less;
    else if(B.has_value())
        return std::strong_ordering::greater;
    else
        return std::strong_ordering::equal;
}

using integer=long long;
int main()
{
    int T;
    std::cin >> T;
    while(T--)
    {
        integer n,K;
        std::cin >> n >> K;
        std::vector<integer> A(n);
        for(auto &a:A)
            std::cin >> a;
        std::sort(A.begin(),A.end());
        integer delta=std::numeric_limits<integer>::max();
        for(int i=0;i<=n;i++) for(int j=i;j<=n;j++)
        {
            std::optional<integer> a,b,c,d,e,f;
            if(i)
            {
                a=A.front()+K;
                b=A[i-1]+K;
            }
            if(i<j)
            {
                c=A[i];
                d=A[j-1];
            }
            if(j<n)
            {
                e=A[j]-K;
                f=A.back()-K;
            }
            auto I={a,b,c,d,e,f};
            auto U=std::max(I, [](auto x,auto y)->bool
            {
                return compare_optional_1(x,y)==std::strong_ordering::less;
            });
            auto L=std::min(I, [](auto x,auto y)->bool
            {
                return compare_optional_2(x,y)==std::strong_ordering::less;
            });
            if(L.has_value() && U.has_value())
                delta=std::min(delta,*U-*L);
        }
        std::cout << delta << std::endl;
    }
}