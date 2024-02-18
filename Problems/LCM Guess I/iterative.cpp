//
// Created by ramizouari on 04/10/23.
//
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
#include <optional>

using natural = std::uint64_t;
using integer = std::int64_t;
using real = long double;
using IR=real;

template<typename R>
R commutator(R a,R b)
{
    return a*b-b*a;
}


template<typename R,typename ...StructureMetaData>
R pow(R a, long long n,StructureMetaData ... meta_info)
{
    if(n==0)
        return R(1,meta_info...);
    else if(n==1)
        return a;
    auto s=pow(a,n/2);
    return n%2?s*s*a:s*s;
}

template<typename R>
R gcd(R a,R b)
{
    if(a<b)
        std::swap(a,b);
    R q,tmp;
    while(b!=0)
    {
        q=a/b;
        tmp=b;
        b=a-b*q;
        a=tmp;
    }
    return a;
}

template<typename R>
R lcm(const R &a,const R &b)
{
    return a*b/gcd(a,b);
}

template<typename R=integer>
struct egcd_t
{
    R a,b,d;
};

template<typename R>
egcd_t<R> egcd(R a,R b)
{
    if(a<b)
    {
        auto e = egcd(b, a);
        std::swap(e.a,e.b);
        return e;
    }
    R q,s1=1,s2=0,t1=0,t2=1,tmp;
    while(b!=0)
    {
        q=a/b;
        tmp=s2;
        s2=s1-q*s2;
        s1=tmp;
        tmp=t2;
        t2=t1-q*t2;
        t1=tmp;
        tmp=b;
        b=a-b*q;
        a=tmp;
    }
    return {s1,t1,a};
}

template<typename R>
std::pair<R,R> bezout(R a, R b)
{
    auto [u,v,_]=egcd(a,b);
    return {u,v};
}

template<typename B>
B next_gray(B n)
{
    return n^(n>>1);
}

template<typename F,typename R>
std::pair<integer,integer> floyd_functional_cycle(F && f,R x0)
{
    /*
     * Find a period
     * */
    R x=x0,y=x;
    integer m=0;
    do
    {
        x=f(x);
        y=f(f(y));
        m++;
    }while(y!=x);
    /*
     * Find offset
     * */
    x=x0,y=x;
    for(int i=0;i<m;i++)
        y=f(y);
    int offset=0;
    while(x!=y)
    {
        x=f(x);
        y=f(y);
        offset++;
    }

    /*
     * Find fundamental period
     * */
    y=f(x);
    integer period=1;
    while(x!=y) {
        y = f(y);
        period++;
    }
    return std::make_pair(period,offset);
}


template<typename F,typename R>
integer functional_period(F &&f, R x)
{
    /*
    * Find a period
    * */
    R y=x;
    integer m=0;
    do
    {
        x=f(x);
        y=f(f(y));
        m++;
    }while(y!=x);
    return m;
}


using couple =std::pair<integer,integer>;

class factoriser
{
    int n;
    std::vector<integer> p_list,smallest_d;
    std::vector<std::vector<integer>> p_factors;
    std::vector<std::vector<integer>> d_list;
    std::vector<std::vector<couple>> p_dec;

    void divisors_list_rec(int n,std::vector<integer> &D,const std::vector<integer> &P, int o=0)
    {
        auto r=P.size();
        for(int i=o;i<r;i++) if(n%P[i]==0)
                divisors_list_rec(n/P[i],D,P,i);
        D.push_back(n);

    }
public:
    factoriser(int _n):n(_n),smallest_d(n+1),p_factors(n+1),d_list(n+1),p_dec(n+1)
    {
        p_list.reserve(n/log(n));
        std::vector<bool> is_prime(n+1,true);
        for(integer i=2;i<=n;i++) if(is_prime[i])
            {
                p_list.push_back(i);
                smallest_d[i]=i;
                p_factors[i]={i};
                for(integer j=2*i;j<=n;j+=i)
                {
                    if(is_prime[j])
                    {
                        smallest_d[j] = i;
                        is_prime[j]=false;
                    }
                    p_factors[j].push_back(i);
                }
            }
    }

    [[nodiscard]] const auto& prime_factors(integer m) const
    {
        static std::vector<integer> holder;
        if(m<=n)
            return p_factors[m];
        auto p=smallest_divisor(m);
        holder={};
        while(m>1)
        {
            auto p= smallest_divisor(m);
            if(holder.empty()||holder.back()!=p)
                holder.push_back(p);
            m/=p;
        }
        return holder;
    }

    const auto& prime_decomposition(integer m)
    {
        static std::vector<std::pair<integer,integer>> holder;
        if(m<=n)
        {
            integer r=m;
            if(p_dec[m].empty())
            {
                for(auto p: prime_factors(m))
                {
                    int s=0;
                    while(r%p==0)
                    {
                        r/=p;
                        s++;
                    }
                    p_dec[m].emplace_back(p,s);
                }
            }
            return p_dec[m];
        }
        else
        {
            holder.clear();
            integer r=m;
            for(auto p: prime_factors(m))
            {
                int s=0;
                while(r%p==0)
                {
                    r/=p;
                    s++;
                }
                holder.emplace_back(p,s);
            }
            return holder;
        }
    }

    const auto& divisors_list(integer m)
    {
        static std::vector<integer> holder;
        static std::map<integer,std::vector<integer>> cache;
        if(m<=n)
        {
            if (d_list[m].empty())
                divisors_list_rec(m, d_list[m], p_factors[m]);
            return d_list[m];
        }
        else
        {
            if(cache.count(m))
                return cache[m];
            divisors_list_rec(m,holder, prime_factors(m));
            cache[m]=holder;
            return cache[m];
        }
    }

    const auto& divisors_list_sorted(integer m)
    {
        static std::vector<integer> holder;
        static std::map<integer,std::vector<integer>> cache;
        if(m<=n)
        {
            if (d_list[m].empty())
            {
                divisors_list_rec(m, d_list[m], p_factors[m]);
                std::sort(d_list[m].begin(),d_list[m].end());
            }
            return d_list[m];
        }
        else
        {
            if(cache.count(m))
                return cache[m];
            divisors_list_rec(m,holder, prime_factors(m));
            std::sort(holder.begin(),holder.end());
            cache[m]=holder;
            return cache[m];
        }
    }

    [[nodiscard]] integer smallest_divisor(integer m) const
    {
        if(m<=n)
            return smallest_d[m];
        integer L=std::ceil(std::sqrt(m));
        for(auto p:p_list)
        {
            if(p>L)
                break;
            else if(m%p==0)
                return p;
        }
        return m;
    }

    [[nodiscard]] bool is_prime(int m) const
    {
        if(m<n)
            return m>1 && smallest_d[m]==m;
        else
        {
            integer L=std::ceil(std::sqrt(m));
            for(auto p:p_list)
                if(m%p==0)
                    return false;
                else if(p>L)
                    break;
            return true;
        }
    }

    integer totient_rec(integer n,const std::vector<integer> &P, integer o=0)
    {
        if(n==0)
            return 0;
        integer S=n;
        for(int i=o;i<P.size();i++)
            S-= totient_rec(n/P[i],P,i+1);
        return S;
    }

    integer totient(integer n)
    {
        integer R=1;
        for(auto [p,m]: prime_decomposition(n))
            R*=pow(p,m-1)*(p-1);
        return R;
    }

    integer totient(integer n,integer m)
    {
        if(n==0)
            return 0;
        auto r=m%n;
        auto P= prime_factors(n);
        return (m/n)*totient(n)+totient_rec(r,P);
    }

    integer carmichael_totient(integer n)
    {
        integer R=1;
        for(auto [p,m]: prime_decomposition(n))
        {
            if(p==2 && m>2)
                R=std::lcm(R,pow(p,m-2));
            else R=std::lcm(R,pow(p,m-1)*(p-1));
        }
        return R;
    }

    integer divisors_count(integer n)
    {
        integer R=1;
        for(auto [_,m]: prime_decomposition(n))
            R*=(m+1);
        return R;
    }

    integer divisor_function(integer n,integer s)
    {
        if(n==0)
            return divisors_count(n);
        integer R=1;
        for(auto [p,m]: prime_decomposition(n))
            R*=(pow(p,(m+1)*s)-1)/(pow(p,s)-1);
        return R;
    }

    integer divisors_sum(integer n)
    {
        return divisor_function(n,1);
    }

    [[nodiscard]] integer count_primes() const
    {
        return p_list.size();
    }

    [[nodiscard]] const auto& prime_list() const
    {
        return p_list;
    }

    void generate_radicals_rec(std::vector<integer> &R,integer a,integer L,int o=0)
    {
        for(int s=o;s<p_list.size() && a*p_list[s] <= L;s++)
        {
            R.push_back(a*p_list[s]);
            generate_radicals_rec(R,a*p_list[s],L,s+1);
        }
    }

    std::vector<integer> genereate_radicals(integer L)
    {
        std::vector<integer> radicals;
        generate_radicals_rec(radicals,1,L);
        return radicals;
    }

};



integer query(std::ostream &O,std::istream& I,int a,int b, std::map<std::pair<int,int>,integer> &C)
{
    if(C.count({a,b}))
        return C.at({a,b});
    O << "? " << a << ' ' << b << std::endl;
    integer s;
    I >> s;
    C[{a,b}]=s;
    C[{b,a}]=s;
    return s;
}



std::pair<integer,int> large_prime(int n,std::mt19937_64  &rng,factoriser &F, std::map<std::pair<int,int>,integer> &M)
{
    std::uniform_int_distribution<int> d(1,n);
    integer s;
    std::pair<integer,int> P;
    int a,b;
    std::vector<bool> used(n);
    do
    {
        do
        {
            a=d(rng),b=d(rng);
            while(b==a)
                b=d(rng);
            if(a>b)
                std::swap(a,b);
        }while(used[a] || used[b]);
        s=query(std::cout,std::cin,a,b,M);
        used[a]=used[b]=true;
        P.first = F.prime_factors(s).back();
    }while(P.first <= (n+1)/2);
    int c = d(rng);
    while(a==c || b==c)
        c=d(rng);
    s=query(std::cout,std::cin,a,c,M);
    auto factors=F.prime_factors(s);
    if(std::binary_search(factors.begin(),factors.end(),P.first))
        P.second = a;
    else P.second=b;
    return P;
}

std::pair<integer,int> largest_prime(int n,std::mt19937_64  &rng,factoriser &F, std::map<std::pair<int,int>,integer> &M)
{
    std::uniform_int_distribution<int> d(1,n);
    int a=d(rng);
    integer s;
    std::pair<integer,int> P;
    int o;
    for(int i=1;i<=n;i++) if(i!=a)
        {
            auto [p,q]=std::minmax(i,a);
            s=query(std::cout,std::cin,p,q,M);
            P = std::max(P,std::make_pair(s,i));
        }
    P.first =F.prime_factors(P.first).back();
    return P;
}

void contract(std::set<int> & J,int i,std::map<std::pair<int,int>,integer> &M)
{
    std::vector<std::pair<int,integer>> C;
    for(auto j:J) if(j+1!=i)
        C.emplace_back(j,query(std::cout,std::cin,i,j+1,M));
    auto minValue=std::min_element(C.begin(),C.end(),[](auto a,auto b)->bool
    {
        return a.second<b.second;
    })->second;
    for(auto [j,s]:C) if(s>minValue)
        J.erase(j);

}

int main()
{
    using integer=std::int64_t;
    using couple=std::pair<integer,int>;
    std::random_device dev;
    std::map<std::pair<int,int>,integer> C;
    std::mt19937_64 rng(15);
    int n;
    std::cin >> n;
    factoriser F(n);
    std::vector<int> I(n),P(n);
    std::set<int> J;
    std::iota(I.begin(),I.end(),1);
    for(int i=0;i<n;i++)
        J.insert(i);
    std::shuffle(I.begin(),I.end(),rng);
    for(int i=0;i<n && J.size() > 1;i++)
        contract(J,I[i],C);
    auto j=*J.begin();
    P[j] = 1;
    for(int i=0;i<n;i++) if(i!=j)
        P[i]=query(std::cout,std::cin,i+1,j+1,C);
    std::cout << "! ";
    for(auto p:P)
        std::cout << p << ' ';
    std::cout << std::endl;
}