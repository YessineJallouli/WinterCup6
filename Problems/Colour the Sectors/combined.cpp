//
// Created by ramizouari on 19/12/23.
//

//
// Created by ASUS on 01/12/2021.
//
#ifndef __NUMBERTHEORY_H__
#define __NUMBERTHEORY_H__
#include <cstdint>
#include <vector>
#include <map>
#include <numeric>
#include <cmath>
#include <stack>
#include <algorithm>
//
// Created by ramizouari on 01/12/2021.
//

#ifndef ACPC_PREPARATION_ABSTRACT_ALGEBRA_H
#define ACPC_PREPARATION_ABSTRACT_ALGEBRA_H
#include <complex>
#include <functional>
#include <cstdint>
#include <concepts>

namespace cp
{
    using natural = std::uint64_t;
    using integer = std::int64_t;
    using real = long double;
    using IR=real;
    using IC= std::complex<IR>;
    constexpr real epsilon=1e-6;

    template<typename R>
    R commutator(R a,R b)
    {
        return a*b-b*a;
    }

    template<typename M,typename G=typename M::base_field>
    M conj(M a)
    {
        if constexpr (std::is_same_v<G, IC>)
        {
            if constexpr (std::is_same_v<G, M>)
                return std::conj(a);
            else for (auto& s : a)
                    s = conj<typename std::remove_reference<decltype(s)>::type, G>(s);
        }
        return a;
    }

    template<typename R,typename ...StructureMetaData>
    R pow(R a, long long n,StructureMetaData ... meta_info)
    {
        if(n==0)
            return R(1,meta_info...);
        else if(n==1)
            return a;
        auto s=pow(a,n/2,meta_info...);
        return n%2?s*s*a:s*s;
    }


    template<typename R,typename F,typename Id>
    R functional_pow(R a,long long n,const F& f,const Id& identity)
    {
        if(n==0)
            return identity;
        else if(n==1)
            return a;
        auto s=functional_pow(a,n/2,f,identity);
        return n%2?f(f(s,s),a):f(s,s);
    }

    template<typename R>
    bool is_zero(const R&a)
    {
        return a==R{};
    }


    inline bool is_zero(const std::complex<long double>&a)
    {
        return std::abs(a) < epsilon;
    }

    inline bool is_zero(const std::complex<double>&a)
    {
        return std::abs(a) < epsilon;
    }

    inline bool is_zero(const real &a)
    {
        return std::abs(a) < epsilon;
    }

    template<typename R>
    R gcd(R a,R b)
    {
        if(a<b)
            std::swap(a,b);
        R q,tmp;
        while(!is_zero(b))
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
        while(!is_zero(b))
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
}


#endif //ACPC_PREPARATION_ABSTRACT_ALGEBRA_H
#include <memory>

namespace cp
{
    using couple =std::pair<integer,integer>;

    class abstract_factoriser
    {
    public:
        virtual ~abstract_factoriser()=default;
        virtual std::vector<integer> prime_factors(integer m) const
        {
            std::vector<integer> P;
            auto p=smallest_divisor(m);
            while(m>1)
            {
                if(P.empty()||P.back()!=p)
                    P.push_back(p);
                m/=p;
            }
            return P;
        }
        virtual integer smallest_divisor(integer m) const=0;
        virtual bool is_prime(integer m) const
        {
            return smallest_divisor(m)==m;
        }
        virtual std::vector<couple> prime_decomposition(integer m) const
        {
            std::vector<couple> P;
            auto p=smallest_divisor(m);
            while(m>1)
            {
                int s=0;
                while(m%p==0)
                {
                    m/=p;
                    s++;
                }
                P.emplace_back(p,s);
            }
            return P;
        }
        virtual std::vector<integer> divisors_list(integer m) const
        {
            std::vector<integer> D;
            divisors_list_rec(m, D, prime_factors(m));
            return D;
        }
    protected:
        void divisors_list_rec(integer n,std::vector<integer> &D,const std::vector<integer> &P, int o=0) const
        {
            auto r=P.size();
            for(int i=o;i<r;i++) if(n%P[i]==0)
                    divisors_list_rec(n/P[i],D,P,i);
            D.push_back(n);

        }

    };

    struct default_factoriser_t
    {
        inline static std::shared_ptr<abstract_factoriser> default_factoriser;
    };


    class factoriser : public abstract_factoriser
    {
        int n;
        std::vector<integer> p_list,smallest_d;
        std::vector<std::vector<integer>> p_factors;


    public:
        factoriser(int _n):n(_n),smallest_d(n+1),p_factors(n+1)
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

        [[nodiscard]] std::vector<integer> prime_factors(integer m) const override
        {
            if(m<=n)
                return p_factors[m];
            std::vector<integer> result={};
            while(m>1)
            {
                auto p= smallest_divisor(m);
                if(result.empty()||result.back()!=p)
                    result.push_back(p);
                m/=p;
            }
            return result;
        }

        std::vector<std::pair<integer,integer>> prime_decomposition(integer m) const override
        {
            integer r=m;
            std::vector<std::pair<integer,integer>> p_dec;
            for(auto p: prime_factors(m))
            {
                int s=0;
                while(r%p==0)
                {
                    r/=p;
                    s++;
                }
                p_dec.emplace_back(p,s);
            }
            return p_dec;
        }

        std::vector<integer> divisors_list_sorted(integer m) const
        {
            auto D=divisors_list(m);
            std::sort(D.begin(),D.end());
            return D;
        }

        [[nodiscard]] integer smallest_divisor(integer m) const override
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

        [[nodiscard]] bool is_prime(integer m) const override
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
            if(s==0)
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

    template<std::integral I>
    inline I chinese_remainder(const std::vector<std::pair<I,I>> &S)
    {
        std::stack<std::pair<I,I>> Q;
        for(auto s:S)
            Q.push(s);
        while(Q.size() > 1)
        {
            auto [a1,p1]=Q.top();
            Q.pop();
            auto [a2,p2]=Q.top();
            Q.pop();
            auto [k1,k2]=bezout(p1,p2);
            auto x=(k1*a2*p1+k2*a1*p2)%(p1*p2);
            if(x<0)
                x+=p1*p2;
            Q.emplace(x,p1*p2);
        }
        return Q.top().first;
    }

    template<std::integral I>
    I chinese_remainder(const std::vector<I>& A,const std::vector<I>& P)
    {
        std::vector<std::pair<I,I>> S;
        int n=A.size(),m=P.size();
        S.reserve(n);
        for(int i=0;i<n;i++)
            S.emplace_back(A[i],P[i]);
        return chinese_remainder(S);
    }
}


#endif
//
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_MODULAR_FUNCTIONS_H
#define CPLIBRARY_MODULAR_FUNCTIONS_H
//
// Created by ASUS on 30/11/2021.
//
#ifndef __MODULAR__ARITHMETIC__
#define __MODULAR__ARITHMETIC__
#include <cstdint>
#include <utility>
//
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_FIXED_H
#define CPLIBRARY_FIXED_H


namespace cp
{
    template<integer mod>
    struct cyclic
    {
        integer n;
        inline static bool assume_prime=true;
        inline static constexpr integer m = mod;
        constexpr cyclic(integer o=0):n((o+m)%m){}
        bool operator==(int O) const
        {
            return n==(m+O)%m;
        }

        bool operator!=(int O) const
        {
            return n!=(m+O)%m;
        }

        bool operator==(cyclic O) const
        {
            return n==O.n;
        }

        bool operator!=(cyclic O) const
        {
            return n!=O.n;
        }

        cyclic operator-() const
        {
            return cyclic(-n);
        }

        auto& operator+=(const cyclic &O)
        {
            n=(n+O.n)%m;
            return *this;
        }
        auto& operator-=(const cyclic &O)
        {
            n=(n+m-O.n)%m;
            return *this;
        }

        auto& operator*=(const cyclic &O)
        {
            n=(n*O.n)%m;
            return *this;
        }

        auto& operator/=(const cyclic &O)
        {
            return (*this)*=O.inv();
        }

        auto operator*(const cyclic &O) const
        {
            auto w=*this;
            return w*=O;
        }

        auto operator+(const cyclic &O) const
        {
            auto w=*this;
            return w+=O;
        }

        auto operator-(const cyclic &O) const
        {
            auto w=*this;
            return w-=O;
        }

        auto operator/(const cyclic &O) const
        {
            return (*this)*O.inv();
        }

        cyclic pinv() const
        {
            return egcd(n,m).a;
        }

        auto inv() const
        {
            if(assume_prime)
                return pow(*this,m-2);
            else return pinv();
        }

        auto& operator++()
        {
            return *this+=1;
        }

        auto& operator--()
        {
            return *this-=1;
        }

        auto operator++(int)
        {
            cyclic r(n);
            *this += 1;
            return r;
        }

        auto operator--(int)
        {
            cyclic r(n);
            *this -= 1;
            return r;
        }

        explicit operator integer&()
        {
            return n;
        }

        explicit operator const integer&() const
        {
            return n;
        }

        static constexpr integer modulus()
        {
            return m;
        }
    };

    template<integer m>
    auto operator*(integer k,cyclic<m> s)
    {
        return s*k;
    }

    template<integer m>
    auto operator+(integer k,cyclic<m> s)
    {
        return s+k;
    }

    template<integer m>
    auto operator-(integer k,cyclic<m> s)
    {
        return (-s)+k;
    }
}

#endif //CPLIBRARY_FIXED_H
//
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_DYNAMIC_H
#define CPLIBRARY_DYNAMIC_H
namespace cp
{
    inline static constexpr integer dynamic_modulus=-2;
    template<>
    struct cyclic<dynamic_modulus>
    {
        integer m,n;
        bool assume_prime=true;
    public:
        cyclic(integer o=0,integer q=0):m(q),n(m?(o+m)%m:o){}
        bool operator==(integer O) const
        {
            if(!m) return n==O;
            else return n==(m+O)%m;
        }

        bool operator==(cyclic O) const
        {
            return n==O.n;
        }

        cyclic& operator+=(const cyclic &O)
        {
            if(!m) m=O.m;
            n+=O.n;
            if(m)
                n%=m;
            return *this;
        }

        cyclic& operator+=(integer O)
        {
            n=n+O;
            if(m) n%=m;
            return *this;
        }

        cyclic& operator-=(const cyclic &O)
        {
            if(!m)
                m=O.m;
            n+=m-O.n;
            if(m)
                n%=m;
            return *this;
        }

        cyclic& operator-=(integer O)
        {
            n+=m-O;
            if(m) n%=m;
            return *this;
        }

        cyclic& operator*=(const cyclic &O)
        {
            if(!m) m=O.m;
            n*=O.n;
            if(m) n%=m;
            return *this;
        }

        cyclic& operator*=(integer O)
        {
            n*=O;
            if(m) n%=m;
            return *this;
        }

        cyclic& operator=(integer O)
        {
            n=O;
            if(m) n%=m;
            return *this;
        }

        cyclic inv() const
        {
            if(m==1)
                return *this;
            else if(assume_prime)
                return pow(*this,m-2,m);
            else return pinv();
        }

        cyclic& operator/=(const cyclic &O)
        {
            return (*this)*=O.inv();
        }

        cyclic& operator/=(integer O)
        {
            return (*this)*=cyclic(O,m).inv();
        }

        cyclic operator*(const cyclic &O) const
        {
            auto w=*this;
            return w*=O;
        }

        cyclic operator+(const cyclic &O) const
        {
            auto w=*this;
            return w+=O;
        }

        cyclic operator+(integer O) const
        {
            auto w=*this;
            return w+=O;
        }

        cyclic operator-() const
        {
            return {m-n,m};
        }

        cyclic operator-(const cyclic &O) const
        {
            auto w=*this;
            return w-=O;
        }

        cyclic operator-(integer O) const
        {
            auto w=*this;
            return w-=O;
        }

        cyclic operator/(const cyclic &O) const
        {
            return (*this)*O.inv();
        }

        cyclic operator/(integer O) const
        {
            return (*this)*cyclic(O,m).inv();
        }

        cyclic pinv() const
        {
            return {egcd(n,m).a,m};
        }

        cyclic& operator++()
        {
            return *this+=1;
        }

        cyclic& operator--()
        {
            return *this-=1;
        }

        cyclic operator++(int)
        {
            cyclic r(n,m);
            *this += 1;
            return r;
        }

        cyclic operator--(int)
        {
            cyclic r(n,m);
            *this -= 1;
            return r;
        }

        explicit operator integer&()
        {
            return n;
        }

        explicit operator const integer&() const
        {
            return n;
        }

        integer modulus() const
        {
            return m;
        }
    };

}

#endif //CPLIBRARY_DYNAMIC_H
//
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_STATIC_H
#define CPLIBRARY_STATIC_H
namespace cp
{
    inline static constexpr integer static_modulus=-1;
    template<>
    struct cyclic<static_modulus>
    {
        integer n;
    public:
        inline static integer m=1;
        inline static bool assume_prime=true;
        cyclic(integer o=0):n((o+m)%m){}
        bool operator==(integer O) const
        {
            return n==(m+O)%m;
        }

        bool operator!=(integer O) const
        {
            return n!=(m+O)%m;
        }

        bool operator==(cyclic O) const
        {
            return n==O.n;
        }

        bool operator!=(cyclic O) const
        {
            return n!=O.n;
        }

        cyclic& operator+=(const cyclic &O)
        {
            n=(n+O.n)%m;
            return *this;
        }
        cyclic& operator-=(const cyclic &O)
        {
            n=(n+m-O.n)%m;
            return *this;
        }

        cyclic& operator*=(const cyclic &O)
        {
            n=(n*O.n)%m;
            return *this;
        }

        cyclic inv() const
        {
            if(assume_prime)
                return pow(*this,m-2);
            else return pinv();
        }

        cyclic& operator/=(const cyclic &O)
        {
            return (*this)*=O.inv();
        }

        cyclic operator*(const cyclic &O) const
        {
            auto w=*this;
            return w*=O;
        }

        cyclic operator+(const cyclic &O) const
        {
            auto w=*this;
            return w+=O;
        }

        cyclic operator-() const
        {
            return cyclic(m-n);
        }

        cyclic operator-(const cyclic &O) const
        {
            auto w=*this;
            return w-=O;
        }

        cyclic operator/(const cyclic &O) const
        {
            return (*this)*O.inv();
        }

        cyclic pinv() const
        {
            return egcd(n,m).a;
        }

        cyclic& operator++()
        {
            return *this+=1;
        }

        cyclic& operator--()
        {
            return *this-=1;
        }

        cyclic operator++(int)
        {
            cyclic r(n);
            *this += 1;
            return r;
        }

        cyclic operator--(int)
        {
            cyclic r(n);
            *this -= 1;
            return r;
        }

        explicit operator integer&()
        {
            return n;
        }

        explicit operator const integer&() const
        {
            return n;
        }

        static integer modulus()
        {
            return m;
        }
        static void set_modulus(integer _m)
        {
            m=_m;
        }
    };

    using d_cyclic=cyclic<static_modulus>;

}

#endif //CPLIBRARY_STATIC_H
#endif
//
// Created by ramizouari on 01/12/2021.
//
#ifndef __RING__EXTENSION__
#define __RING__EXTENSION__
//
// Created by ramizouari on 01/12/2021.
//
#ifndef __POLYNOMIAL__H__
#define __POLYNOMIAL__H__
#include <vector>
#include <map>


#endif

#endif
//
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_FUNCTIONS_H
#define CPLIBRARY_FUNCTIONS_H
namespace cp
{
    inline integer totient(integer n,abstract_factoriser &F)
    {
        integer r=1;
        for(auto [p,k]:F.prime_decomposition(n))
            r*=pow(p,k-1)*(p-1);
        return r;
    }

    inline integer carmichael_totient(integer n,abstract_factoriser &F)
    {
        integer r=1;
        for(auto [p,k]:F.prime_decomposition(n))
        {
            if(p==2&&k>=3)
                r=std::lcm(r,pow(2,k-2));
            else
                r = std::lcm(r, pow(p, k - 1) * (p - 1));
        }
        return r;
    }

    inline integer divisors_count(integer n,abstract_factoriser &F)
    {
        integer r=1;
        for(auto [p,k]:F.prime_decomposition(n))
            r*=k+1;
        return r;
    }

    inline integer divisors_sum(integer n,abstract_factoriser &F)
    {
        integer r=1;
        for(auto [p,k]:F.prime_decomposition(n))
            r*=(pow(p,k+1)-1)/(p-1);
        return r;
    }

    inline integer prime_multiplicity(integer n,integer p,abstract_factoriser &F)
    {
        for(auto [q,k]:F.prime_decomposition(n))
            if(q==p)
                return k;
        return 0;
    }

    inline integer prime_multiplicity(integer n,integer p)
    {
        integer r=0;
        while(n%p==0)
        {
            n/=p;
            r++;
        }
        return r;
    }


}

#endif //CPLIBRARY_FUNCTIONS_H
#include <random>
namespace cp
{
    inline std::vector<integer> inverse_table(int n,int prime)
    {
        std::vector<integer> I(n + 1);
        I[0] = I[1] = 1;
        for (int i = 2; i <= n; i++)
            I[i] = I[prime % i] *
                   (prime - prime / i) % prime;
        return I;
    }
}

#endif //CPLIBRARY_MODULAR_FUNCTIONS_H
#include <iostream>
#include <chrono>
#include <unordered_set>

constexpr cp::integer M=998'244'353;
using IK=cp::cyclic<M>;
constexpr cp::integer L=1e6+1;

IK sector_colours_count_normal(cp::integer m,cp::integer K,cp::abstract_factoriser &F,std::vector<IK> &K_pow,std::vector<cp::integer> &I,std::vector<IK> &cache)
{
    if(cache[m]!=0) return cache[m];
    using namespace cp;
    if(m<=2)
    {
        if(m==1) return K;
        else return K*(K+1)/2;
    }
    auto divisors=F.divisors_list(m);
    IK R;
    for(auto d:divisors)
        R+=totient(m/d,F)*K_pow[d];
    R*=I[m];
    return cache[m]=R;
}

IK sector_colours_count_normal(const std::vector<cp::integer> &A,cp::integer K,cp::abstract_factoriser &F,std::vector<IK> &K_pow,std::vector<cp::integer> &I,std::vector<IK> &cache)
{
    return std::accumulate(A.begin(),A.end(),IK{1},[&](auto a,auto b){return a*sector_colours_count_normal(b,K,F,K_pow,I,cache);});
}

IK sector_colours_count_with_local_reverse(cp::integer m,cp::integer K,cp::abstract_factoriser &F,std::vector<IK> &K_pow,std::vector<cp::integer> &I,std::vector<IK> &cache)
{
    if(cache[m]!=0) return cache[m];
    using namespace cp;
    if(m<=2)
    {
        if(m==1) return K;
        else return K*(K+1)/2;
    }
    auto divisors=F.divisors_list(m);
    IK R1,R2;
    for(auto d:divisors)
        R1+=totient(m/d,F)*K_pow[d];
    R1*=I[m];
    if(m%2==0)
        R2=K_pow[m/2]*(K+1)*I[2];
    else
        R2=K_pow[(m+1)/2];
    return cache[m]=(R1+R2)*I[2];
}
IK sector_colours_count_with_local_reverse(const std::vector<cp::integer>& A,cp::integer K,cp::abstract_factoriser &F,std::vector<IK> &K_pow,std::vector<cp::integer> &I,std::vector<IK> &cache)
{
    return std::accumulate(A.begin(),A.end(),IK{1},[&](auto a,auto b){return a*sector_colours_count_with_local_reverse(b,K,F,K_pow,I,cache);});
}

IK sector_colours_count_with_global_reverse_1(cp::integer m,cp::integer K,cp::abstract_factoriser &F,std::vector<IK> &K_pow,std::vector<cp::integer> &I,std::vector<IK> &cache)
{
    if(cache[m]!=0) return cache[m];
    using namespace cp;
    if(m<=2)
    {
        if(m==1) return K;
        else return K*(K+1)/2;
    }
    auto divisors=F.divisors_list(m);
    IK R;
    for(auto d:divisors)
        R+=totient(m/d,F)*K_pow[d];
    R*=I[m];
    return cache[m]=R;
}

IK sector_colours_count_with_global_reverse_2(cp::integer m,cp::integer K,std::vector<IK> &K_pow,std::vector<cp::integer> &I)
{
    switch (m)
    {
        case 1:
            return K;
        case 2:
            return K*(K+1)/2;
        default:
            if(m%2==0)
                return K_pow[m/2]*(K+1)*I[2];
            else
                return K_pow[(m+1)/2];
    }
}

IK sector_colours_count_with_global_reverse(std::vector<cp::integer> &A,cp::integer K,cp::abstract_factoriser &F,std::vector<IK> &K_pow,std::vector<cp::integer> &I,std::vector<IK> &cache)
{
    auto R1=std::accumulate(A.begin(),A.end(),IK{1},[&](auto a,auto b){return a*sector_colours_count_with_global_reverse_1(b,K,F,K_pow,I,cache);});
    auto R2=std::accumulate(A.begin(),A.end(),IK{1},[&](auto a,auto b){return a*sector_colours_count_with_global_reverse_2(b,K,K_pow,I);});
    return (R1+R2)*I[2];
}


struct rotation_equal
{
    bool operator()(const std::vector<cp::integer> &A,const std::vector<cp::integer> &B) const
    {
        int n=A.size();
        bool any_equal=false;
        for(int j=0;j<n && !any_equal;j++)
        {
            any_equal=true;
            for(int i=0;i<n && any_equal;i++)
                if(A[i]!=B[(i+j)%n])
                    any_equal=false;
        }
        return any_equal;
    }

    bool operator()(const std::vector<std::vector<cp::integer>> &A,const std::vector<std::vector<cp::integer>> &B) const
    {
        bool all_equal=true;
        for(int i=0;i<A.size() && all_equal;i++)
            all_equal=(*this)(A[i],B[i]);
        return all_equal;
    }

    cp::integer operator()(const std::vector<cp::integer> &A) const
    {
        return std::reduce(A.begin(),A.end());
    }

    cp::integer operator()(const std::vector<std::vector<cp::integer>> &A) const
    {
        return std::accumulate(A.begin(),A.end(),cp::integer{0},[&](auto a,auto b){return 10*a+(*this)(b);});
    }

};

struct dihedral_equal
{
    rotation_equal R;
    bool operator()(const std::vector<cp::integer> &A,const std::vector<cp::integer> &B) const
    {
        return R(A,B) || R(A,std::vector<cp::integer>(B.rbegin(),B.rend()));
    }

    bool operator()(const std::vector<std::vector<cp::integer>> &A,const std::vector<std::vector<cp::integer>> &B) const
    {
        bool all_equal=true;
        for(int i=0;i<A.size() && all_equal;i++)
            all_equal=(*this)(A[i],B[i]);
        return all_equal;
    }

    cp::integer operator()(const std::vector<cp::integer> &A) const
    {
        return R(A);
    }

    cp::integer operator()(const std::vector<std::vector<cp::integer>> &A) const
    {
        return R(A);
    }
};

struct rotation_equal_mod2
{
    rotation_equal R;
    bool operator()(const std::vector<cp::integer> &A,const std::vector<cp::integer> &B) const
    {
        return R(A,B) || R(A,std::vector<cp::integer>(B.rbegin(),B.rend()));
    }

    bool operator()(const std::vector<std::vector<cp::integer>> &A,const std::vector<std::vector<cp::integer>> &B) const
    {
        bool all_equal=true;
        for(int i=0;i<A.size() && all_equal;i++)
            all_equal=R(A[i],B[i]);
        if(!all_equal) {
            all_equal = true;
            for (int i = 0; i < A.size() && all_equal; i++)
                all_equal = R(A[i], std::vector<cp::integer>(B[i].rbegin(), B[i].rend()));
        }
        return all_equal;
    }

    cp::integer operator()(const std::vector<cp::integer> &A) const
    {
        return R(A);
    }

    cp::integer operator()(const std::vector<std::vector<cp::integer>> &A) const
    {
        return R(A);
    }
};

template<typename SymmetryGroup>
IK naive_count(const std::vector<cp::integer> &A,cp::integer K)
{
    IK R=1;
    std::vector<std::vector<cp::integer>> B(A.size());
    for(int i=0;i<A.size();i++)
        B[i].resize(A[i]);
    std::unordered_set<std::vector<std::vector<cp::integer>>,SymmetryGroup,SymmetryGroup> S;
    do {
        S.emplace(B);
        for(auto &X:B) for(auto &x:X)
        {
            x++;
            if(x==K)
                x=0;
            else goto skip;
        }
        skip:
        ;
    }while(std::any_of(B.begin(),B.end(),[&](const auto& X){
        return std::any_of(X.begin(),X.end(),[&](auto x){return x!=0;});
    }));
    return S.size();
}


int main()
{
    using namespace cp;
    integer n,K;
    std::cin >> n >> K;
    std::vector<integer> A(n);
    for(auto &a:A) std::cin >> a;
    auto F=std::make_shared<factoriser>(L);
    default_factoriser_t::default_factoriser=F;
    std::vector<integer> I_table(L);
    I_table=cp::inverse_table(L,M);
    std::vector<IK> K_pow(L),cache(L);
    K_pow[0]=1;
    for(int i=1;i<L;i++)
        K_pow[i]=K_pow[i-1]*K;
    IK Y=sector_colours_count_with_global_reverse(A,K,*F,K_pow,I_table,cache);
    std::cout << (integer)Y << std::endl;
}
