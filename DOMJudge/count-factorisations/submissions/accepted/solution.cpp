#include <iostream>
#include <numeric>
#include <map>
#include <fstream>
#include <chrono>
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_FIXED_SPARSE_ARRAY_H
#define CPLIBRARY_FIXED_SPARSE_ARRAY_H
#include <vector>
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_BITS_H
#define CPLIBRARY_BITS_H
namespace cp
{
    inline unsigned int bit_log(unsigned int n)
    {
        unsigned char a=0,b=30,r=0;
        while(a<=b)
        {
            auto c=(a+b)/2;
            if(n>>c)
                a=c+1;
            else
            {
                b=c-1;
                r=c-1;
            }
        }
        if(r && (1<<(r-1))==n)
            return r-1;
        return r;
    }

    inline unsigned int bit_floor(unsigned int n)
    {
        return 1<<bit_log(n);
    }

    inline unsigned int bit_ceil(unsigned int n)
    {
        unsigned r=1;
        while(r<n)
            r<<=1;
        return r;
    }
}

#endif //CPLIBRARY_BITS_H
namespace cp::data_structures::fixed
{
    template<typename O>
    struct sparse_array
    {
        using T=typename O::type;
        using type = T;
        inline static O F=O();
        int n,h;
        std::vector<std::vector<T>> S;
    public:
        sparse_array(const std::vector<T>&A):n(bit_ceil(A.size())),h(bit_log(n)),S(h+1)
        {
            int r=1;
            for(int i=h;i>=0;i--,r*=2)
                S[i].resize(n-r+1,O::neutral);
            for(int i=0;i<A.size();i++)
                S[h][i]=A[i];
            r=1;
            for(int i=h-1;i>=0;i--,r*=2) for(int j=0;j<=n-2*r;j++)
                    S[i][j]=F(S[i+1][j],S[i+1][j+r]);
        }

        T query(int l,int r) const
        {
            if(l>=r)
                return O::neutral;
            auto d=r-l;
            auto s=bit_floor(d);
            auto b=bit_log(s);
            return F(S[h-b][l],S[h-b][r-s]);
        }
    };


}
#endif //CPLIBRARY_SPARSE_ARRAY_H
//
// Created by ASUS on 01/12/2021.
//
#ifndef __OPERATION_H__
#define __OPERATION_H__
#include <numeric>
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
    template<typename T>
    struct binary_operation : public std::enable_shared_from_this<binary_operation<T>>
    {
        using type=T;
        template<typename H0,typename ...H>
        T operator()(const H0&a,const H&... b) const
        {
            if constexpr (sizeof...(b) == 0)
                return a;
            else return reduce(a,this->operator()(b...));
        }
        virtual T reduce(const T& a, const T& b) const = 0;
        virtual T neutral_element() const
        {
            return T{};
        }
    };

    template<typename T>
    struct invertible_operation : public std::enable_shared_from_this<invertible_operation<T>>
    {
        virtual T inv(const T& a) const = 0;
    };

    template<typename T>
    struct involution_inverse_t : public invertible_operation<T>
    {
        virtual T inv(const T& a) const
        {
            return a;
        }
    };


    template<typename T>
    struct multiplicative_inverse_t : public invertible_operation<T>
    {
        T inv(const T& a) const override
        {
            return a.inv();
        }
    };

    template<std::floating_point T>
    struct multiplicative_inverse_t<T> : public invertible_operation<T>
    {
        T inv(const T& a) const override
        {
            return 1/a;
        }
    };

    template<typename T>
    struct multiplicative_inverse_t<std::complex<T>> : public invertible_operation<std::complex<T>>
    {
        std::complex<T> inv(const std::complex<T>& a) const override
        {
            return std::complex<T>(1.)/a;
        }
    };

    template<typename T>
    struct additive_inverse_t : public invertible_operation<T>
    {
        T inv(const T& a) const override
        {
            return -a;
        }
    };

    template<typename T>
    struct bitwise_inverse_t : public invertible_operation<T>
    {
        T inv(const T& a) const override
        {
            return ~a;
        }
    };

    template<typename T>
    struct monoid_plus_t:public binary_operation<T> {
        T reduce(const T &a, const T &b) const override {
            return a + b;
        }
        inline static T neutral{};
    };

    template<typename T>
    struct plus_t:public monoid_plus_t<T>,public additive_inverse_t<T>
    {
    };

    template<typename T>
    struct multiplies_t:public binary_operation<T>
    {
        T reduce(const T&a,const T&b) const override
        {
            return a*b;
        }

        inline static T neutral=T(1);
        T neutral_element() const override
        {
            return neutral;
        }
    };

    template<typename T>
    struct field_multiplies_t:public multiplies_t<T>,public invertible_operation<T>
    {
    };

    template<typename T>
    struct max_t:public binary_operation<T>
    {
        T e;
        explicit max_t(T _e):e(_e){}
        max_t(): max_t(T{}){}
        T reduce(const T&a,const T&b) const override
        {
            return std::max(a,b);
        }

        inline static T neutral{0};
        T neutral_element() const override
        {
            return e;
        }
    };

    template<typename T>
    struct min_t:public binary_operation<T>
    {
        T e;
        explicit min_t(T _e):e(_e){}
        min_t(): min_t(T{}){}

        T reduce(const T&a,const T&b) const override
        {
            return std::min(a,b);
        }

        inline static T neutral{};

        T neutral_element() const override
        {
            return e;
        }
    };

    template<typename T>
    struct gcd_t:public binary_operation<T>
    {
        T reduce(const T&a,const T&b) const override
        {
            return gcd(a,b);
        }

        inline static T neutral{0};
    };

    template<typename T>
    struct lcm_t:public binary_operation<T>
    {
        T reduce(const T&a,const T&b) const override
        {
            return lcm(a,b);
        }

        inline static T neutral{1};
        T neutral_element() const override
        {
            return neutral;
        }
    };

    template<typename T>
    struct xor_t:public binary_operation<T>,public invertible_operation<T>
    {
        T reduce(const T&a,const T&b) const
        {
            return a^b;
        }

        T inv(const T&a) const
        {
            return a;
        }

        inline static T neutral{};
    };

    template<typename T>
    struct and_t:public binary_operation<T>
    {
        T reduce(const T&a,const T&b) const override
        {
            return a&b;
        }

        inline static T neutral=static_cast<T>(-1);
        T neutral_element() const override
        {
            return neutral;
        }
    };

    template<typename T>
    struct or_t:public binary_operation<T>
    {
        T reduce(const T&a,const T&b) const override
        {
            return a|b;
        }

        inline static T neutral{};
    };

    template<typename T>
    struct logical_and_t :public binary_operation<T>
    {
        T reduce(const T& a, const T& b) const override
        {
            return a && b;
        }

        inline static T neutral{true};
        T neutral_element() const override
        {
            return neutral;
        }
    };

    template<typename T>
    struct logical_or_t :public binary_operation<T>
    {
        T reduce(const T& a, const T& b) const override
        {
            return a || b;
        }

        inline static T neutral{false};
        T neutral_element() const override
        {
            return neutral;
        }
    };

    template<typename T>
    struct logical_xor_t :public binary_operation<T>,public invertible_operation<T>
    {
        T reduce(const T& a, const T& b) const override
        {
            return !a && b || a && !b;
        }
        T inv(const T&a) const
        {
            return !a;
        }
        inline static T neutral{false};
        T neutral_element() const override
        {
            return neutral;
        }
    };

    template<typename T>
    class binary_operation_ptr
    {
        std::shared_ptr<binary_operation<T>> op;
    public:
        binary_operation_ptr(std::shared_ptr<binary_operation<T>> value): op(value){}
        template<typename ...H>
        auto operator()(const H&... h) const
        {
            return op->operator()(h...);
        }

        auto reduce(const T& a,const T& b) const
        {
            return op->reduce(a,b);
        }

        auto neutral_element() const
        {
            return op->neutral_element();
        }
    };

    template<typename T>
    class invertible_binary_operation_ptr : public binary_operation_ptr<T>
    {
        std::shared_ptr<invertible_operation<T>> inverter;
    public:
        invertible_binary_operation_ptr(std::shared_ptr<binary_operation<T>> b,
                                        std::shared_ptr<invertible_operation<T>> I): binary_operation_ptr<T>(b),inverter(I){}
        using binary_operation_ptr<T>::operator();
        using binary_operation_ptr<T>::neutral_element;
        auto inv(const T& a) const
        {
            return inverter->inv(a);
        }
    };
}

#endif
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
            while(m>1)
            {
                auto p=smallest_divisor(m);
                P.push_back(p);
                m/=p;
                while(m%p==0)
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
            while(m>1)
            {
                auto p=smallest_divisor(m);
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

        std::vector<integer> divisors_list_sorted(integer m) const
        {
            auto D=divisors_list(m);
            std::sort(D.begin(),D.end());
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

    class light_factoriser : public abstract_factoriser
    {
        int n;
        std::vector<integer> p_list,smallest_d;
    public:
        light_factoriser(int _n):n(_n),smallest_d(n+1)
        {
            p_list.reserve(n/log(n));
            std::vector<bool> is_prime(n+1,true);
            for(integer i=2;i<=n;i++) if(is_prime[i])
            {
                p_list.push_back(i);
                smallest_d[i]=i;
                for(integer j=2*i;j<=n;j+=i) if(is_prime[j])
                {
                    smallest_d[j] = i;
                    is_prime[j]=false;
                }
            }
        }

        [[nodiscard]] std::vector<integer> prime_factors(integer m) const override
        {
            std::vector<integer> result;
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

        [[nodiscard]] integer count_primes() const
        {
            return p_list.size();
        }

        [[nodiscard]] const auto& prime_list() const
        {
            return p_list;
        }
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

        [[nodiscard]] integer count_primes() const
        {
            return p_list.size();
        }

        [[nodiscard]] const auto& prime_list() const
        {
            return p_list;
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

#ifndef CPLIBRARY_DYNAMIC_H
#define CPLIBRARY_DYNAMIC_H
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
// Created by ASUS on 01/12/2021.
//

#ifndef __OPTIMISATION__
#define __OPTIMISATION__
//
// Created by ramizouari on 30/11/2021.
//
#ifndef __LINEAR__ALGEBRA__
#define __LINEAR__ALGEBRA__
#include <vector>
#include <array>
//
// Created by ramizouari on 01/12/2021.
//
#ifndef __POLYNOMIAL__H__
#define __POLYNOMIAL__H__
#include <vector>
#include <map>

namespace cp
{
    /**
* @brief Polynomial class
* @details This is the class of polynomials over a commutative ring R
* @tparam R the type of the coefficients
* @Requirements
* <strong>R</strong> is a commutative ring
* @Notes
* Formally this class is simply R[x]
*/
    template<typename R>
    struct polynomial
    {
        std::vector<R> p;

    public:

        const std::vector<R>& data() const
        {
            return p;
        }
        std::vector<R>& data()
        {
            return p;
        }

        bool operator==(const R& a) const
        {
            if (is_zero(a))
                return p.empty();
            else return degree() == 0 && p.front() == a;
        }

        bool operator==(const polynomial<R> &) const = default;

        polynomial& reduce()
        {
            while(!p.empty() && is_zero(p.back()))
                p.pop_back();
            return *this;
        }
        polynomial(R k):p(1,k)
        {
            reduce();
        }

        template<std::integral T>
        polynomial(T k):p(1,k)
        {
            reduce();
        }

        polynomial()=default;

        polynomial(std::vector<R> _p):p(std::move(_p))
        {
            reduce();
        }

        int degree() const
        {
            return p.size()-1;
        }

        int dim() const
        {
            return p.size();
        }

        auto& operator+=(const polynomial &O)
        {
            int r=std::max(p.size(),O.p.size());
            p.resize(r);
            for(int i=0;i<O.p.size();i++)
                p[i]+=O.p[i];
            reduce();
            return *this;
        }

        auto& operator-=(const polynomial &O)
        {
            int r=std::max(p.size(),O.p.size());
            p.resize(r);
            for(int i=0;i<O.p.size();i++)
                p[i]-=O.p[i];
            reduce();
            return *this;
        }

        polynomial operator*(const polynomial &O) const
        {
            if(O.p.empty() || p.empty())
                return polynomial(0);
            int n=degree(),m=O.degree();
            polynomial q;
            q.p.resize(n+m+1);
            for(int i=0;i<=n;i++) for(int j=0;j<=m;j++)
                    q.p[i+j]+=p[i]*O.p[j];
            q.reduce();
            return q;
        }

        auto& operator*=(const polynomial &O)
        {
            auto r=(*this)*O;
            p.swap(r.p);
            return *this;
        }

        auto operator+(const polynomial &O) const
        {
            auto r=*this;
            return r+=O;
        }

        auto operator-(const polynomial &O) const
        {
            auto r=*this;
            return r-=O;
        }

        auto operator-() const
        {
            auto r=*this;
            for(auto &s:r.p)
                s=-s;
            return r;
        }

        auto operator*=(R a)
        {
            if(is_zero(a))
                p.clear();
            else for(auto& s:p)
                    s*=a;
            reduce();
            return *this;
        }

        bool operator!=(R a) const
        {
            return !(*this==a);
        }

        auto& operator+=(R a)
        {
            return *this+=polynomial({a});
        }

        auto& operator-=(R a)
        {
            return *this-=polynomial({a});
        }

        auto operator+(R a) const
        {
            auto q=*this;
            return q+=a;
        }

        auto operator-(R a) const
        {
            auto q=*this;
            return q-=a;
        }

        /**
        * @details creates a preorder between polynomials based on the degree
        * @Requirements:
        * None
        * @Notes
        * This function is essential for the euclidean algorithm to work
        */
        bool operator<(const polynomial &O) const
        {
            return degree() < O.degree();
        }

        /**
         * @brief Polynomial self division
        * @details Divides the polynomial by a constant and stores the result in itself
        * @Requirements
        * One of the following:
         * <ul>
        * <li> R is an integral ring [2]
        * <li> k is invertible
         * </ul>
        */

        auto& operator/=(R k)
        {
            for(auto &s:p)
                s/=k;
            return *this;
        }

        auto operator/(R k) const
        {
            auto q=*this;
            return q/=k;
        }

        /**
         * @brief Euclidean division
        * @details Applies euclidean division between two polynomials
        * @Requirements
        * One of the following
        * <ul>
        * <li> R is a field [1]
        * <li> R is an euclidean domain [2]
        * <li> R is a commutative ring, and the dominant coefficient of <strong>O</strong> is inversible
        * <ul/>
        * @Notes
        * Even that condition [1] is a special case of [2], given that some properties of euclidean division are
        * guaranteed only if <strong>R</strong> is a field, We will seperate the two cases
        */
        std::pair<polynomial,polynomial> euclidean_division(const polynomial &O) const
        {
            if(degree() < O.degree())
                return {R(0),*this};
            polynomial q,r=*this;
            int n=degree(),m=O.degree();
            q.p.resize(n-m+1);
            for(int i=n;i>=m;i--)
            {
                q.p[i-m]=r[i]/O.p[m];
                for(int j=0;j<=m;j++)
                    r.p[i+j-m]-=q.p[i-m]*O.p[j];
            }
            r.reduce();
            return {q,r};
        }

        //Get the quotient of the euclidean division
        polynomial operator/(const polynomial &O) const
        {
            return euclidean_division(O).first;
        }

        //Get the remainder of the euclidean division
        polynomial operator%(const polynomial &O) const
        {
            return euclidean_division(O).second;
        }

        auto& operator/=(polynomial &O)
        {
            p.swap(((*this)/O).p);
            return *this;
        }

        auto &operator[](int k)
        {
            return p[k];
        }

        const auto& operator[](int k) const
        {
            return p[k];
        }

        polynomial derivative() const
        {
            if (p.empty())
                return {};
            polynomial D;
            D.p.resize(degree());
            for(int i=1;i<=degree();i++)
                D.p[i-1]=p[i]*R(i);
            return D;
        }

        /**
        * @brief Polynomial Evaluation
        * @details Evaluates the polynomial at a given point
        * Evaluates the polynomial over an associative R-algebra H
        * @Requirements
        * <strong>H</strong> is an associative algebra over <strong>R</strong>
        */
        template<typename H>
        std::common_type<H,R>::type operator()(H a) const
        {
            typename std::common_type<H,R>::type r{};
            for(int i=degree();i>=0;i--)
                r=r*a+p[i];
            return r;
        }

        auto begin()
        {
            return p.begin();
        }

        auto end()
        {
            return p.end();
        }

        auto begin() const
        {
            return p.begin();
        }

        auto end() const
        {
            return p.end();
        }

        explicit operator std::vector<R>&()
        {
            return p;
        }

        explicit operator const std::vector<R>&() const
        {
            return p;
        }
    };

    template<typename R>
    polynomial<R> operator*(R a,const polynomial<R> &p)
    {
        auto q=p;
        return q*=a;
    }

    template<typename R>
    polynomial<R> operator-(R a,const polynomial<R> &p)
    {
        auto q=-p;
        return q+=a;
    }

/**
* @brief The functional identity polynomial
* @details This constant is the generator of all polynomials over R.
* @Notes
* Formally, it is simply the polynomial X:x->x
*/
    template<typename R>
    const polynomial<R> X=polynomial<R>(std::vector<R>{0,1});

/**
* @brief Newton Interpolation
* @details Applies Lagrange Interpolation over points (x,y) using Newton's method
* @Requirements
 * <ol>
* <li> x does not have a duplicate element
* <li> One of the following
 * <ul>
*   <li> R is a field
*   <li> (s-t) is invertible for all elements s,t in x
 *  </ul>
 * </ol>
*/
    template<typename R>
    polynomial<R> newton_interpolation(const std::vector<R> &x,const std::vector<R> &y)
    {
        int n=x.size()-1;
        std::vector<std::vector<R>> d(n+1,std::vector<R>(n+1));
        for(int i=0;i<=n;i++)
            d[i][i]=y[i];
        for(int r=1;r<=n;r++) for(int i=0;i+r<=n;i++)
                d[i][i+r]=(d[i+1][i+r]-d[i][i+r-1])/(x[i+r]-x[i]);
        polynomial<R> p,u=R(1);
        for(int i=0;i<=n;i++) {
            p +=d[0][i]*u;
            u*=(X<R>-x[i]);
        }
        return p;
    }
}

#endif
//
// Created by ramizouari on 09/12/22.
//

#ifndef CPLIBRARY_VECTOR_H
#define CPLIBRARY_VECTOR_H

#include <vector>
#include <cstddef>
#include <array>

namespace cp::linalg
{
    struct v_shape
    {
        int n;
    };

/**
 * @brief Dynamic Vector
 * @detail Dynamic Vector is a vector in the mathematical sense,
 * @formal It is the union of R^k for all k, where k is the dimension of the vector.
 * <ul>
 * <li> Addition between 2 vectors are defined with respect to the first vector's shape <br>
 * <li> for all k, the set of all vectors of shape k is a vector space
 * </ul>
 * @Requirements
 * R is a commutative ring
 * */
    template<typename R>
    class d_vector
    {
        std::vector<R> u;
    public:
        using base_field=R;
        using base_ring=R;
        inline static int n=0;
        d_vector():u(n){}
        d_vector(std::vector<R> _u):u(std::move(_u)){}
        d_vector(v_shape shape):u(shape.n){}

        bool operator==(const d_vector<R>& other) const
        {
            return u==other.u;
        }
        auto dim() const
        {
            return u.size();
        }

        auto& operator[](int k)
        {
            return u[k];
        }

        const auto& operator[](int k) const
        {
            return u[k];
        }

        auto& operator+=(const d_vector &o)
        {
            for(int i=0;i<dim();i++)
                u[i]+=o.u[i];
            return *this;
        }

        auto& operator-=(const d_vector &o)
        {
            for(int i=0;i<dim();i++)
                u[i]-=o.u[i];
            return *this;
        }

        auto& operator*=(R k)
        {
            for(auto &s:u)
                s*=k;
            return *this;
        }

        auto operator+(const d_vector &o) const
        {
            auto v=*this;
            return v+=o;
        }

        auto operator-(const d_vector &o) const
        {
            auto v=*this;
            return v-=o;
        }

        auto operator-() const
        {
            auto v=*this;
            for(auto &s:v.u)
                s=-s;
            return v;
        }

        auto& operator/=(R k)
        {
            for(auto &s:u)
                s/=k;
            return *this;
        }

        auto operator/(R k) const
        {
            auto v=*this;
            return v/=k;
        }

        auto begin() {
            return u.begin();
        }

        auto begin() const
        {
            return u.cbegin();
        }

        auto end()
        {
            return u.end();
        }

        auto end() const
        {
            return u.cend();
        }
    };

    template<typename R>
    auto operator*(const R&k,const d_vector<R>& u)
    {
        auto v=u;
        return v*=k;
    }

/**
 * @brief Static Vector:
 * @tparam R is the base field
 * @tparam n is the dimension of the vector space
 * @details It is a member of an R-vector space E where dim(E)= n
 * @Requirements
 * <strong>R</strong> is a commutative ring. <br>
 * @Formal <strong>E</strong> is an <strong>R</strong>-module, and it is a vector space only if <strong>R</strong> is a field. <br>
 * In fact, the name s_vector is used for consistency with the computer science's name.
 */

    template<typename R,int n>
    class s_vector
    {
        std::array<R,n> u;
    public:
        using base_field=R;
        using base_ring=R;
        inline static constexpr int dim()
        {
            return n;
        }

        s_vector()
        {
            for(int i=0;i<n;i++)
                u[i]=0;
        }

        s_vector(std::array<R,n>_u):u(std::move(_u)){}

        bool operator==(const s_vector&) const = default;

        auto& operator[](int k)
        {
            return u[k];
        }

        const auto& operator[](int k) const
        {
            return u[k];
        }

        auto& operator+=(const s_vector &o)
        {
            auto r=std::min(dim(),o.dim());
            for(int i=0;i<r;i++)
                u[i]+=o.u[i];
            return *this;
        }

        auto& operator-=(const s_vector &o)
        {
            auto r=std::min(dim(),o.dim());
            for(int i=0;i<r;i++)
                u[i]-=o.u[i];
            return *this;
        }

        auto& operator*=(R k)
        {
            for(auto &s:u)
                s*=k;
            return *this;
        }

        auto operator+(const s_vector &o) const
        {
            auto v=*this;
            return v+=o;
        }

        auto operator-(const s_vector &o) const
        {
            auto v=*this;
            return v-=o;
        }

        auto operator-() const
        {
            auto v=*this;
            for(auto &s:v.u)
                s=-s;
            return v;
        }

        auto& operator/=(R k)
        {
            for(auto &s:u)
                s/=k;
            return *this;
        }

        auto operator/(R k) const
        {
            auto v=*this;
            return v/=k;
        }

        auto begin()
        {
            return u.begin();
        }

        auto begin() const
        {
            return u.cbegin();
        }

        auto end()
        {
            return u.end();
        }

        auto end() const
        {
            return u.cend();
        }

        template <size_t k>
        auto& get()& {
            return u[k];
        }

        template <size_t k>
        const auto& get() const& {
            return u[k];
        }

        template <size_t k>
        auto&& get() const&& {
            return u[k];
        }

        template <size_t k>
        auto&& get() && {
            return u[k];
        }
    };

    template<typename R,int n>
    auto operator*(const R&k,const s_vector<R,n>& u)
    {
        auto v=u;
        return v*=k;
    }
}

namespace std
{
    template<typename R,int n>
    struct tuple_size<cp::linalg::s_vector<R, n>> : std::integral_constant<size_t, n>{};
    template<size_t k,typename R,int n>
    struct tuple_element<k, cp::linalg::s_vector<R, n>>
    {
        using type = R;
    };
}

#endif //CPLIBRARY_VECTOR_H


namespace cp::linalg
{
    struct m_shape
    {
        int n,m;
    };

/**
 * @brief Matrix:
* @details This is the union of R^(n*m) for all n and m
* @Requirements
* <strong>R</strong> is a commutative ring.
* @formal it is the set of matrices over the commutative ring <strong>R</strong> <br>
 * In fact, It is the union of L(R^a,R^b) over all a and b where L(E,F) is the set of matrices acting on E with values over F
*/
    template<typename R>
    struct d_matrix
    {
        std::vector<std::vector<R>> M;

    public:
        using base_field=R;
        using base_ring=R;
        inline static int n=0,m=0;
        const auto& data() const
        {
            return M;
        }
        auto &data()
        {
            return M;
        }
        d_matrix(R k=0,m_shape shape ={n,m}):M(shape.n,std::vector<R>(shape.m))
        {
            for(int i=0;i<std::min(shape.n,shape.m);i++)
                M[i][i]=k;
        }
        d_matrix(std::vector<std::vector<R>> &&_M):M(std::move(_M)){}
        d_matrix(const std::vector<std::vector<R>> &_M) :M(_M) {}
        auto row_dim() const
        {
            return M.size();
        }

        auto col_dim() const
        {
            return M.empty()?0:M[0].size();
        };

        auto& operator[](int k)
        {
            return M[k];
        }

        R tr() const
        {
            int m=col_dim(),n=row_dim();
            R r=0;
            for(int i=0;i<std::min(n,m);i++)
                r+=M[i][i];
            return r;
        }

        d_matrix T() const
        {
            int m=col_dim(),n=row_dim();
            d_matrix P(0,m_shape{m,n});
            for(int i=0;i<n;i++) for(int j=0;j<m;j++)
                    P.M[j][i]=M[i][j];
            return P;
        }

        d_matrix H() const
        {
            int m = col_dim(), n = row_dim();
            d_matrix P(0, m_shape{ m,n });
            for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
                    P.M[j][i] = conj<R,R>(M[i][j]);
            return P;
        }

        const auto& operator[](int k) const
        {
            return M[k];
        }

        auto &operator+=(const d_matrix &O)
        {
            int r1=std::min(row_dim(),O.row_dim()),r2=std::min(col_dim(),O.col_dim());
            for(int i=0;i<r1;i++) for(int j=0;j<r2;j++)
                    M[i][j]+=O.M[i][j];
            return *this;
        }

        auto &operator-=(const R &O)
        {
            int r=std::min(row_dim(),col_dim());
            for(int i=0;i<r;i++)
                M[i][i]-=O;
            return *this;
        }

        auto &operator-=(const d_matrix &O)
        {
            int r1=std::min(row_dim(),O.row_dim()),r2=std::min(col_dim(),O.col_dim());
            for(int i=0;i<r1;i++) for(int j=0;j<r2;j++)
                    M[i][j]-=O.M[i][j];
            return *this;
        }

        auto operator+(const d_matrix &O) const
        {
            auto N=*this;
            return N+=O;
        }

        auto operator-(const d_matrix &O) const
        {
            auto N=*this;
            return N-=O;
        }

        auto operator-(const R &O) const
        {
            auto N=*this;
            return N-=O;
        }

        auto operator-() const
        {
            auto N=*this;
            for(auto &row:N.M) for(auto &s:row)
                    s=-s;
            return N;
        }

        auto operator*(const d_matrix &O) const
        {
            int n=row_dim(),m=col_dim(),p=O.col_dim();
            d_matrix N(0,m_shape{n,p});
            for(int i=0;i<n;i++) for(int k=0;k<m;k++) for(int j=0;j<p;j++)
                        N.M[i][j]+=M[i][k]*O.M[k][j];
            return N;
        }

        auto &operator*=(const d_matrix &O)
        {
            auto N=(*this)*O;
            M.swap(N.M);
            return *this;
        }

        auto & operator*=(R k)
        {
            for(auto &row:M) for(auto &u:row)
                    u*=k;
            return *this;
        }

        d_vector<R>operator*(const d_vector<R> &u) const
        {
            int n=row_dim(),m=col_dim();
            d_vector<R> v(v_shape{n});
            for(int j=0;j<m;j++) for(int i=0;i<n;i++)
                    v[i]+=M[i][j]*u[j];
            return v;
        }

        auto &operator/=(R k)
        {
            for(auto &row:M) for(auto &u:row)
                    u/=k;
            return *this;
        }

        auto operator/(R k) const
        {
            auto N=*this;
            return N/=k;
        }

        auto& operator/=(const d_matrix& O)
        {
            return *this *= O.inv();
        }

        auto operator/(const d_matrix &O) const
        {
            return (*this) * O.inv();
        }

        auto begin()
        {
            return M.begin();
        }

        auto begin() const
        {
            return M.cbegin();
        }

        auto end()
        {
            return M.end();
        }

        auto end() const
        {
            return M.cend();
        }

        auto row_echelon_form() const
        {
            int n=row_dim(),m=col_dim();
            auto P=*this;
            int s=0;
            for(int i=0;i<n;i++)
            {
                int p=s;
                while(p<n && is_zero(P.M[p][i]))
                    p++;
                if(p==n)
                    continue;
                std::swap(P.M[p],P.M[s]);
                R w=P.M[s][i];
                for(int j=s+1;j<n;j++)
                {
                    R r=P.M[j][i]/w;
                    for (int k = i; k < m; k++)
                        P.M[j][k]-=r*P.M[i][k];
                }
                s++;
            }
            return P;
        }

        int rank() const
        {
            auto E=row_echelon_form();
            int r=0;
            int n=row_dim(),m=col_dim();
            for(int i=0,j=0;i<n&&j<m;j++)
                if(!is_zero(E.M[i][j]))
                {
                    r++;
                    i++;
                }
            return r;
        }

        int nullity() const
        {
            return row_dim()-rank();
        }

        R det() const
        {
            int n=row_dim(),m=col_dim();
            auto P=*this;
            bool invert=false;
            for(int i=0;i<n;i++)
            {
                int p=i;
                while(p<n && is_zero(P.M[p][i]))
                    p++;
                if(p==n)
                    return 0;
                if(p!=i)
                {
                    std::swap(P.M[p], P.M[i]);
                    invert=!invert;
                }
                R w=P.M[i][i];
                for(int j=i+1;j<n;j++)
                {
                    R r=P.M[j][i]/w;
                    for (int k = i; k < m; k++)
                        P.M[j][k]-=r*P.M[i][k];
                }
            }
            R d=1;
            for(int i=0;i<n;i++)
                d*=P.M[i][i];
            return invert?-d:d;
        }


        d_matrix inv() const
        {
            int n=row_dim(),m=col_dim();
            d_matrix P=*this,Q(1,m_shape{n,m});
            for(int i=0;i<n;i++)
            {
                int p=i;
                while(p<n && is_zero(P.M[p][i]))
                    p++;
                if(p==n)
                    continue;
                std::swap(P.M[p], P.M[i]);
                std::swap(Q.M[p],Q.M[i]);
                R w=P.M[i][i];
                for(int j=i+1;j<n;j++)
                {
                    R r=P.M[j][i]/w;
                    for (int k = 0; k < m; k++)
                    {
                        P.M[j][k] -= r*P.M[i][k];
                        Q.M[j][k] -= r*Q.M[i][k];
                    }
                }
            }
            for(int i=n-1;i>=0;i--)
            {
                R w=P.M[i][i];
                for(int j=0;j<n;j++)
                    Q.M[i][j]/=w;
                for(int k=i-1;k>=0;k--)
                {
                    R r=P.M[k][i];
                    for (int j = 0; j < n; j++)
                        Q.M[k][j] -= r*Q.M[i][j];
                }
            }
            return Q;
        }

        d_vector<R> solve(d_vector<R> A)
        {
            int n=col_dim(),m=row_dim();
            d_matrix P=*this;
            for(int i=0;i<n;i++)
            {
                int p=i;
                while(p<n && is_zero(P.M[p][i]))
                    p++;
                if(p==n)
                    continue;
                std::swap(P.M[p], P.M[i]);
                std::swap(A[p],A[i]);
                R w=P.M[i][i];
                for(int j=i+1;j<n;j++)
                {
                    if(is_zero(w))
                        continue;
                    R r=P.M[j][i]/w;
                    for (int k = 0; k < m; k++)
                        P.M[j][k] -= r*P.M[i][k];
                    A[j]-=r*A[i];
                }
            }
            for(int i=n-1;i>=0;i--)
            {
                R w=P.M[i][i];
                if(is_zero(w))
                    continue;
                A[i]/=w;
                for(int k=i-1;k>=0;k--)
                {
                    R r=P.M[k][i];
                    A[k] -= r*A[i];
                }
            }
            return A;
        }
    };

    template<typename R>
    d_matrix<R> operator*(const R&a,const d_matrix<R> &M)
    {
        auto N=M;
        return N*=a;
    }

/**
 * @details Static Matrix
 * It is an element of the vector space L(R^n,R^m)
 * @tparam R the base commutative ring
 * @tparam n the number of rows
 * @tparam m the number of columns
 * @Requirements R is a commutative ring
 * @Requirements n>=0
 * @Requirements m>=0
 * @Formal
 * <ul>
 * <li> Multiplication between 2 matrices is defined if the shapes are compatible
 * <li> Multiplication between a matrix and a vector is defined if the shapes are compatible
 * <li> It is an associative algebra if n=m
 * </ul>
 * */

    template<typename R,int n,int m>
    class s_matrix
    {
        std::array<std::array<R,m>,n> M;
    public:
        using base_field=R;
        using base_ring=R;
        bool operator==(const s_matrix&) const = default;
        s_matrix(R k=0)
        {
            for(int i=0;i<n;i++) for(int j=0;j<m;j++)
                    M[i][j]=i==j?k:0;
        }
        s_matrix(std::array<std::array<R,m>,n> _M):M(std::move(_M)){}

        inline static constexpr int row_dim()
        {
            return n;
        }

        s_matrix(const std::vector<std::array<R,m>> &_M)
        {
            int counter=0;
            for(int i=0;i<n;i++) for(int j=0;j<m;j++)
                    M[i][j]=_M[i][j];
        }

        inline static constexpr int col_dim()
        {
            return m;
        };

        auto& operator[](int k)
        {
            return M[k];
        }

        const auto& operator[](int k) const
        {
            return M[k];
        }

        R tr() const
        {
            R r=0;
            for(int i=0;i<std::min(n,m);i++)
                r+=M[i][i];
            return r;
        }

        s_matrix<R,m,n> T() const
        {
            s_matrix<R,m,n> P;
            for(int i=0;i<n;i++) for(int j=0;j<m;j++)
                    P.M[j][i]=M[i][j];
            return P;
        }

        s_matrix<R, m, n> H() const
        {
            s_matrix<R, m, n> P;
            for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
                    P.M[j][i] = conj(M[i][j]);
            return P;
        }

        auto &operator+=(const s_matrix &O)
        {
            for(int i=0;i<n;i++) for(int j=0;j<m;j++)
                    M[i][j]+=O.M[i][j];
            return *this;
        }

        auto &operator-=(const s_matrix &O)
        {
            for(int i=0;i<n;i++) for(int j=0;j<m;j++)
                    M[i][j]-=O.M[i][j];
            return *this;
        }

        auto operator+(const s_matrix &O) const
        {
            auto N=*this;
            return N+=O;
        }

        auto operator-(const s_matrix &O) const
        {
            auto N=*this;
            return N-=O;
        }

        auto operator-() const
        {
            auto N=*this;
            for(auto &row:N.M) for(auto &s:row)
                    s=-s;
            return N;
        }

        template<int p>
        s_matrix<R,n,p> operator*(const s_matrix<R,m,p> &O) const
        {
            s_matrix<R,n,p> N(0);
            for(int i=0;i<n;i++) for(int k=0;k<m;k++) for(int j=0;j<p;j++)
                        N[i][j]+=M[i][k]*O[k][j];
            return N;
        }

        auto &operator*=(const s_matrix &O)
        {
            static_assert(n==m);
            auto N=(*this)*O;
            M.swap(N.M);
            return *this;
        }

        auto & operator*=(R k) {
            for (auto &row: M)
                for (auto &u: row)
                    u *= k;
            return *this;
        }
        s_vector<R,n> operator*(const s_vector<R,m> &u) const
        {
            s_vector<R,n> v;
            for(int j=0;j<m;j++) for(int i=0;i<n;i++)
                    v[i]+=M[i][j]*u[j];
            return v;
        }

        auto &operator/=(R k)
        {
            for(auto &row:M) for(auto &u:row)
                    u/=k;
            return *this;
        }

        auto operator/(R k) const
        {
            auto N=*this;
            return N/=k;
        }

        auto& operator/=(const s_matrix &O)
        {
            return *this*=O.inv();
        }

        auto operator/(const s_matrix &O) const
        {
            return (*this) * O.inv();
        }

        auto begin()
        {
            return M.begin();
        }

        auto begin() const
        {
            return M.cbegin();
        }

        auto end()
        {
            return M.end();
        }

        auto end() const
        {
            return M.cend();
        }

        auto row_echelon_form() const
        {
            auto P=*this;
            int s=0;
            for(int i=0;i<n;i++)
            {
                int p=s;
                while(p<n && is_zero(P.M[p][i]))
                    p++;
                if(p==n)
                    continue;
                P.M[p].swap(P.M[s]);
                R w=P.M[s][i];
                for(int j=s+1;j<n;j++)
                {
                    R r=P.M[j][i]/w;
                    for (int k = i; k < m; k++)
                        P.M[j][k]-=r*P.M[i][k];
                }
                s++;
            }
            return P;
        }

        int rank() const
        {
            auto E=row_echelon_form();
            int r=0;
            for(int i=0,j=0;i<n&&j<m;j++)
                if(! is_zero(E.M[i][j]))
                {
                    r++;
                    i++;
                }
            return r;
        }

        int nullity() const
        {
            return row_dim()-rank();
        }

        R det() const
        {
            static_assert(n==m);
            auto P=*this;
            bool invert=false;
            for(int i=0;i<n;i++)
            {
                int p=i;
                while(p<n && is_zero(P.M[p][i]))
                    p++;
                if(p==n)
                    continue;
                if(p!=i)
                {
                    std::swap(P.M[p], P.M[i]);
                    invert=!invert;
                }
                R w=P.M[i][i];
                for(int j=i+1;j<n;j++)
                {
                    R r=P.M[j][i]/w;
                    for (int k = i; k < m; k++)
                        P.M[j][k]-=r*P.M[i][k];
                }
            }
            R d=1;
            for(int i=0;i<n;i++)
                d*=P.M[i][i];
            return invert?-d:d;
        }

        s_matrix inv() const
        {
            static_assert(n==m);
            s_matrix P=*this,Q(1);
            for(int i=0;i<n;i++)
            {
                int p=i;
                while(p<n && is_zero(P.M[p][i]))
                    p++;
                if(p==n)
                    continue;
                std::swap(P.M[p], P.M[i]);
                std::swap(Q.M[p],Q.M[i]);
                R w=P.M[i][i];
                for(int j=i+1;j<n;j++)
                {
                    R r=P.M[j][i]/w;
                    for (int k = 0; k < m; k++)
                    {
                        P.M[j][k] -= r*P.M[i][k];
                        Q.M[j][k] -= r*Q.M[i][k];
                    }
                }
            }
            for(int i=n-1;i>=0;i--)
            {
                R w=P.M[i][i];
                for(int j=0;j<n;j++)
                    Q.M[i][j]/=w;
                for(int k=i-1;k>=0;k--)
                {
                    R r=P.M[k][i];
                    for (int j = 0; j < n; j++)
                        Q.M[k][j] -= r*Q.M[i][j];
                }
            }
            return Q;
        }

        s_vector<R,m> solve(s_vector<R,n> A) const
        {
            static_assert(n==m);
            s_matrix P=*this;
            for(int i=0;i<n;i++)
            {
                int p=i;
                while(p<n && P.M[p][i]==R(0))
                    p++;
                if(p==n)
                    continue;
                std::swap(P.M[p], P.M[i]);
                std::swap(A[p],A[i]);
                R w=P.M[i][i];
                for(int j=i+1;j<n;j++)
                {
                    if(is_zero(w))
                        continue;
                    R r=P.M[j][i]/w;
                    for (int k = 0; k < m; k++)
                        P.M[j][k] -= r*P.M[i][k];
                    A[j]-=r*A[i];
                }
            }
            for(int i=n-1;i>=0;i--)
            {
                R w=P.M[i][i];
                if(w==R(0))
                    continue;
                A[i]/=w;
                for(int k=i-1;k>=0;k--)
                {
                    R r=P.M[k][i];
                    A[k] -= r*A[i];
                }
            }
            return A;
        }

    };

    template<typename R,int n,int m>
    s_matrix<R,n,m> operator*(const R&a,const s_matrix<R,n,m> &M)
    {
        auto N=M;
        return N*=a;
    }

    template<int n=-1>
    using IE = std::conditional_t<n >= 0, s_vector<real, n>, d_vector<real>>;

    template<int n = -1>
    using IH = std::conditional_t<n >= 0, s_vector<IC, n>, d_vector<IC>>;

    template<int n = -1,int m=n>
    using IM_IR = std::conditional_t<n >= 0 && m>=0, s_matrix<real, n,m>, d_matrix<real>>;

    template<int n = -1, int m = n>
    using IM_IC = std::conditional_t<n >= 0 && m >= 0, s_matrix<IC, n, m>, d_matrix<IC>>;
}
#endif // __LINEAR__ALGEBRA__
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
#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__
#include <functional>

namespace cp::topology
{
    template<typename R>
    concept complex_type= requires(const R&r) {
        {r.real()} -> std::convertible_to<double>;
        {r.imag()} -> std::convertible_to<double>;
    };

    template<typename R>
    concept real_type= !complex_type<R>;

/*
* This class is the class of all metrics over E
* @Requirements
* None
*/
    template<typename E>
    struct metric_t
    {
        virtual real metric(const E&u,const E&v)const =0;
        real distance(const E& u, const E& v) const
        {
            return metric(u, v);
        }
    };


/*
* This class is the class of all norms over E
* @Requirements
* E is a vector space over a field
* @Notes
* Every specialisation of this class must define the norm consistently with the field K
*/
    template<typename E>
    struct norm_t :public metric_t<E>
    {
        virtual real norm(const E&u)const =0;
        real metric(const E&u,const E&v)const  override
        {
            return norm(v-u);
        }
    };

/*
* This class is the class of all inner products over E
* @Requirements
* 1. K is IC or IR
* 2. E is a vector space over E
* @Notes
* Every specialisation of this class must define the inner product consistently with the field K
*/

    template<typename K,typename E>
    struct inner_product_t:public norm_t<E>
    {
        real norm(const E&u) const override
        {
            return std::sqrt(std::abs(inner_product(u,u)));
        }

        virtual K inner_product(const E&u,const E&v)const  =0;
    };

/*
* This class defines the natural inner product over E
* @Requirements
* 1. K is IC or IR
* 2. E is a vector space over E
*/
    template<typename K,typename E>
    struct L2_inner_product;

    template<typename E>
    struct L2_inner_product<real,E>:public inner_product_t<real,E>
    {
        real inner_product(const E&u,const E&v) const
        {
            auto m=std::min(u.dim(),v.dim());
            real R=0;
            for(int i=0;i<m;i++)
                R+=u[i]*v[i];
            return R;
        }
    };

    template<typename E>
    struct L2_inner_product<IC,E>:public inner_product_t<IC,E>
    {
        IC inner_product(const E&u,const E&v) const
        {
            auto m=std::min(u.dim(),v.dim());
            IC R=0;
            for(int i=0;i<m;i++)
                R+=std::conj(u[i])*v[i];
            return R;
        }
    };

    template<typename E>
    struct L1_norm :public norm_t<E>
    {
        real norm(const E&u) const
        {
            real R=0;
            for(auto s:u)
                R+=std::abs(s);
            return R;
        }
    };

    template<typename E>
    struct L_inf_norm :public norm_t<E>
    {
        real norm(const E&u) const
        {
            real R=0;
            for(auto s:u)
                R=std::max(R,std::abs(s));
            return R;
        }
    };

    struct real_inner_product :public inner_product_t<real,real>
    {
        real inner_product(const real &u,const real &v) const override
        {
            return u*v;
        }
    };

    struct complex_inner_product :public inner_product_t<IC,IC>
    {
        IC inner_product(const IC &u,const IC &v) const override
        {
            return std::conj(u)*v;
        }
    };

    template<>
    struct L2_inner_product<real,real>:public real_inner_product{};
    template<>
    struct L_inf_norm<real>:public real_inner_product{};
    template<>
    struct L1_norm<real>:public real_inner_product{};

    template<>
    struct L2_inner_product<IC,IC>:public complex_inner_product{};

    template<typename E,typename F,typename R>
    class derivator
    {
        real eps;
    public:
        derivator(real _eps=1e-7):eps(_eps){}
        template<typename ...StructureMetaData>
        R jacobian(const std::function<F(E)>&f,E a,StructureMetaData ... meta_info) const
        {
            R J(0,meta_info...);
            for(int i=0;i<J.col_dim();i++)
            {
                a[i]+=eps;
                auto z2=f(a);
                a[i]-=2*eps;
                auto z1=f(a);
                auto du=(z2-z1)/(2*eps);
                for(int j=0;j<J.row_dim();j++)
                    J[j][i]=du[j];
                a[i]+=eps;
            }
            return J;
        }
    };

    template<typename E,real_type IK>
    class derivator<E,IK,E>
    {
        IK eps;
    public:
        derivator(IK _eps=1e-7):eps(_eps){}

        E gradient(const std::function<IK(E)>&f,E a) const
        {
            E grad(a);
            for(int i=0;i<grad.dim();i++)
            {
                a[i]+=eps;
                auto z2=f(a);
                a[i]-=2*eps;
                auto z1=f(a);
                a[i]+=eps;
                grad[i]=(z2-z1)/(2*eps);
            }
            return grad;
        }
    };

    template<real_type IK>
    class derivator<IK,IK,IK>
    {
        IK eps;
    public:
        derivator(IK _eps=1e-7):eps(_eps){}
        real derivative(const std::function<IK(IK)>&f,IK a) const
        {
            return (f(a+eps)-f(a-eps))/(2*eps);
        }

        real gradient(const std::function<IK(IK)>&f,IK a) const
        {
            return derivative(f,a);
        }
    };

    template<typename E,complex_type IK>
    class derivator<E,IK,E>
    {
        using real_type =typename IK::value_type;
        real eps;
    public:
        derivator(real_type _eps=1e-7):eps(_eps){}

        E gradient(const std::function<IK(E)>&f,E a) const
        {
            E grad(a);
            for(int i=0;i<grad.dim();i++)
            {
                a[i]+=eps;
                auto z2=f(a);
                a[i]-=2*eps;
                auto z1=f(a);
                a[i]+=eps;
                grad[i]=(z2-z1)/(2*eps);
            }
            return grad;
        }
    };

    template<complex_type IK>
    class derivator<IK,IK ,IK>
    {
        using real_type = typename IK::value_type;
        real_type eps;
    public:
        derivator(real_type _eps=1e-7):eps(_eps){}
        IC derivative(const std::function<IK(IK)>&f,IK a) const
        {
            return (f(a+eps)-f(a-eps))/(2*eps);
        }

        IC gradient(const std::function<IK(IK)>&f,IK a) const
        {
            return derivative(f,a);
        }
    };

    template<typename K,typename R,typename M,typename Norm=L2_inner_product<K,R>>
    class newton_raphson;

    template<typename Norm,real_type IK>
    class newton_raphson<IK,IK,IK,Norm>
    {
        inline static Norm N=Norm();
        derivator<IK, IK,IK> &D;
        real x0;
        real eps=1e-5;
    public:
        newton_raphson(real _x0, derivator<IK, IK, IK>& d) :D(d),x0(_x0) {}
        real root(const std::function<IK(IK)>& f) const
        {
            real x = x0;
            while (N.norm(f(x)) > eps)
                x = x - f(x) / D.derivative(f, x);
            return x;
        }
    };

    template<typename Norm,complex_type IK>
    class newton_raphson<IK,IK,IK,Norm>
    {
        inline static Norm N=Norm();
        derivator<IK, IK,IK> &D;
        IC x0;
        real eps=1e-5;
    public:
        newton_raphson(real _x0, derivator<IK, IK, IK>& d) :D(d),x0(_x0) {}
        IC root(const std::function<IK(IK)>& f) const
        {
            IC x = x0;
            while (N.norm(f(x)) > eps)
                x = x - f(x) / D.derivative(f, x);
            return x;
        }
    };

    template<real_type IK,typename E,typename M,typename Norm>
    class newton_raphson<IK,E,M,Norm>
    {
        inline static Norm N=Norm();
        derivator<E, E,M> &D;
        E x0;
        IK eps=1e-5;
    public:
        newton_raphson(E _x0, derivator<E, E, M>& d) :D(d),x0(_x0) {}
        template<typename ...StructureMetaData>
        E root(const std::function<E(E)>& f,StructureMetaData ... meta_info) const
        {
            E x = x0;
            while (N.norm(f(x)) > eps) {
                auto u=D.jacobian(f, x,meta_info...).solve(f(x));
                x-=u;
            }
            return x;
        }
    };


    template<complex_type IK,typename E,typename M,typename Norm>
    class newton_raphson<IK,E,M,Norm>
    {
        inline static Norm N=Norm();
        derivator<E, E,M> &D;
        E x0;
        real eps=1e-5;
    public:
        newton_raphson(E _x0, derivator<E, E, M>& d) :D(d),x0(_x0) {}
        template<typename ...StructureMetaData>
        E root(const std::function<E(E)>& f,StructureMetaData ... meta_info) const
        {
            E x = x0;
            while (N.norm(f(x)) > eps) {
                auto u=D.jacobian(f, x,meta_info...).solve(f(x));
                x-=u;
            }
            return x;
        }
    };

    template<typename Norm>
    class newton_raphson<IC,Norm,IC>
    {
        inline static constexpr Norm N=Norm();
        derivator<IC, IC,IC> &D;
        IC x0;
        real eps=1e-5;
    public:
        newton_raphson(IC _x0, derivator<IC, IC, IC>& d) :D(d),x0(_x0) {}
        IC root(const std::function<IC(IC)>& f) const
        {
            IC x = x0;
            while (N.norm(f(x)) > eps)
                x = x - f(x) / D.derivative(f, x);
            return x;
        }
    };
}

#endif
#include <functional>
namespace cp::topology
{
    using namespace linalg;
    template<int n>
    s_vector<real,n> point_wise_divide(const s_vector<real,n> &x,const s_vector<real,n> &y)
    {
        s_vector<real,n> z;
        for(int i=0;i<n;i++)
            z[i]=x[i]/y[i];
        return z;
    }

    template<int n,int m>
    s_vector<real,n> get_column(const s_matrix<real,n,m> &A,int k)
    {
        s_vector<real,n> C;
        for(int i=0;i<n;i++)
            C[i]=A[i][k];
        return C;
    }

    template<int n>
    real max(const s_vector<real,n> &x)
    {
        real r=x[0];
        for(auto s:x)
            r=std::max(r,s);
        return r;
    }

    template<int n>
    int argmax(const s_vector<real,n> &x)
    {
        int k=0;
        for(int i=1;i<n;i++) if(x[i]>x[k])
                k=i;
        return k;
    }

    template<int n, int m>
    s_vector<real,m> simplex(
            const s_vector<real,m>& _Z,
            const s_matrix<real,n, m>& _A,
            s_vector<real,n> b)
    {
        s_vector<real, n + m > Z;
        for (int i = 0; i < m; i++)
            Z[i] = _Z[i];
        s_matrix<real,n, n + m>A;
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
                A[i][j] = _A[i][j];
        for (int i = m; i < n + m; i++)
            A[i - m][i] = 1;
        while (max(Z) > 0)
        {
            auto q = argmax(Z);
            int p = -1;
            auto c = point_wise_divide(b,get_column(A,q));
            for (int k = 0; k < n; k++)
                if (A[k][q] > 0 && c[k] >= 0 && (p == -1 || c[k] < c[p]))
                    p = k;
            if (p == -1)
                break;
            for (int i = 0; i < n; i++) if (i != p)
                {
                    auto k=A[i][q];
                    b[i] -= (k / A[p][q]) * b[p];
                    for(int j=0;j<n+m;j++)
                        A[i][j] -= (k / A[p][q]) * A[p][j];
                    A[i][q] = 0;

                }
            auto k=Z[q];
            for(int j=0;j<n+m;j++)
                Z[j] -= (k / A[p][q]) * A[p][j];
            Z[q] = 0;
        }
        s_vector<real,n + m> h;
        for (int i = 0; i < n; i++)
            h[i] = b[i];
        s_matrix<real,n + m, n + m> Q;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n + m; j++)
                Q[i][j] = A[i][j];
        int r = 0;
        for (int i = 0; i < n + m ; i++)
            if (Z[i] < 0)
            {
                Q[n + r++][i] = 1;
                for (int j = 0; j < n; j++)
                    A[j][i] = 0;
            }
        s_vector<real,m> d;
        auto g = Q.solve(h);
        for (int i = 0; i < m; i++)
            d[i] = g[i];
        return d;
    }


    d_vector<real> point_wise_divide(const d_vector<real> &x,const d_vector<real> &y)
    {
        int n=x.dim();
        d_vector<real> z(v_shape{n});
        for(int i=0;i<n;i++)
            z[i]=x[i]/y[i];
        return z;
    }

    d_vector<real> get_column(const d_matrix<real> &A,int k)
    {
        int n=A.row_dim(),m=A.col_dim();
        d_vector<real> C(v_shape{m});
        for(int i=0;i<n;i++)
            C[i]=A[i][k];
        return C;
    }

    real max(const d_vector<real> &x)
    {
        real r=x[0];
        for(auto s:x)
            r=std::max(r,s);
        return r;
    }

    int argmax(const d_vector<real> &x)
    {
        int k=0,n=x.dim();
        for(int i=1;i<n;i++) if(x[i]>x[k])
                k=i;
        return k;
    }

    d_vector<real> simplex(
            const d_vector<real>& _Z,
            const d_matrix<real>& _A,
            d_vector<real> b)
    {
        int n=b.dim(),m=_Z.dim();
        d_vector<real> Z(v_shape{n+m});
        for (int i = 0; i < m; i++)
            Z[i] = _Z[i];
        d_matrix<real>A(0,m_shape{n,n+m});
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
                A[i][j] = _A[i][j];
        for (int i = m; i < n + m; i++)
            A[i - m][i] = 1;
        while (max(Z) > 0)
        {
            auto q = argmax(Z);
            int p = -1;
            auto c = point_wise_divide(b,get_column(A,q));
            for (int k = 0; k < n; k++)
                if (A[k][q] > 0 && c[k] >= 0 && (p == -1 || c[k] < c[p]))
                    p = k;
            if (p == -1)
                break;
            for (int i = 0; i < n; i++) if (i != p)
                {
                    auto k=A[i][q];
                    b[i] -= (k / A[p][q]) * b[p];
                    for(int j=0;j<n+m;j++)
                        A[i][j] -= (k / A[p][q]) * A[p][j];
                    A[i][q] = 0;

                }
            auto k=Z[q];
            for(int j=0;j<n+m;j++)
                Z[j] -= (k / A[p][q]) * A[p][j];
            Z[q] = 0;
        }
        d_vector<real> h(v_shape{n+m});
        for (int i = 0; i < n; i++)
            h[i] = b[i];
        d_matrix<real> Q(0,m_shape{n+m,n+m});
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n + m; j++)
                Q[i][j] = A[i][j];
        int r = 0;
        for (int i = 0; i < n + m ; i++)
            if (Z[i] < 0)
            {
                Q[n + r++][i] = 1;
                for (int j = 0; j < n; j++)
                    A[j][i] = 0;
            }
        d_vector<real> d(v_shape{m});
        auto g = Q.solve(h);
        for (int i = 0; i < m; i++)
            d[i] = g[i];
        return d;
    }

    template<typename E,real_type IK=IR,typename Norm=L2_inner_product<real,E>>
    class gradient_descent
    {
        inline static constexpr Norm N{};
    protected:
        IK p=.05;
        IK eps;
        derivator<E,real,E>& D;
    public:
        gradient_descent(derivator<E,IK,E> &d,IK _eps=1e-3):D(d),eps(_eps) {}
        E argmin(const std::function<IK(E)>& f,E x) const
        {
            for (; N.norm(D.gradient(f, x)) > eps; x -= p * D.gradient(f, x));
            return x;
        }
        E argmin(const std::function<IK(E)>& f, E x,int L) const
        {
            for (; N.norm(D.gradient(f, x)) > eps && L--; x -= p * D.gradient(f, x));
            return x;
        }
        E argmin(const std::function<std::pair<IK,E>(E)>& f, E x) const
        {
            auto P = f(x);
            while (N.norm(P.second) > eps)
            {
                x -= p * P.second;
                P = f(x);
            }
            return x;
        }

        E argmin(const std::function<std::pair<real, E>(E)>& f, E x,int L) const
        {
            auto P = f(x);
            while (N.norm(P.second) > eps && L--)
            {
                x -= p * P.second;
                P = f(x);
            }
            return x;
        }

    };

    template<typename E,real_type IK=IR,typename InnerProduct=L2_inner_product<IK,E>>
    class barzilai_borwein_gradient_descent
    {
        IK p=.1;
        IK eps=1e-8;
        derivator<E,real,E>& D;
        inline static constexpr InnerProduct B{};
    public:
        barzilai_borwein_gradient_descent(derivator<E, IK,E>& d, real _p):D(d),p(_p){}

        E argmin(const std::function<IK(E)>& f, E s,int L)
        {
            this->p = 0.1;
            E x = s- this->p*this->D.gradient(f, s);
            for (; B.norm(this->D.gradient(f, x)) > this->eps && L; x -= this->p * this->D.gradient(f, x))
            {
                update_rate(f, x,s);
                s = x;
                L--;
            }
            return x;
        }

        E argmin(const std::function<std::pair<IK, E>(E)>& f, E s, int L)
        {
            auto P = f(s);
            E x = s - this->p * f(s).second;
            auto Q = f(x);
            while (B.norm(P.second) > eps && L--)
            {
                update_rate(x, s,Q.second,P.second);
                s = x;
                x -= this->p * Q.second;
                P = std::move(Q);
                Q = f(x);
            }
            return x;
        }


        virtual void update_rate(const std::function<IK(E)>& f, const E& x,const E& s)
        {
            auto L = this->D.gradient(f, x) - this->D.gradient(f, s);
            this->p = B.inner_product(L,x - s) / B.inner_product(L,L);
        }

        virtual void update_rate(const E& x, const E& s,const E& dx,const E& ds)
        {
            auto L = dx - ds;
            this->p = B.inner_product(L, x - s) / B.inner_product(L, L);
        }
    };
}
#endif
//
// Created by ramizouari on 12/01/24.
//

#ifndef CPLIBRARY_FORMAL_SERIES_H
#define CPLIBRARY_FORMAL_SERIES_H
//
// Created by ASUS on 01/12/2021.
//

#ifndef ACPC_PREPARATION_FAST_POLY_H
#define ACPC_PREPARATION_FAST_POLY_H
//
// Created by ASUS on 01/12/2021.
//

#ifndef ACPC_PREPARATION_FFT_H
#define ACPC_PREPARATION_FFT_H
#include <numbers>
#include <algorithm>
#include <optional>
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
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_MODULAR_FUNCTIONS_H
#define CPLIBRARY_MODULAR_FUNCTIONS_H
//
// Created by ramizouari on 01/12/2021.
//
#ifndef __RING__EXTENSION__
#define __RING__EXTENSION__

namespace cp
{
    /*
 * This header contains a list of usual ring extensions:
 * 1. field of fractions for an integral ring R
 * 2. nilpotent extension of a ring R: formally it is isomorphic to R[x]/x^k where k is the nilpotence index
 * 3. idempotent extention of a ring R: formally it is isomorphic to R[x]/(x^k-x^(k-1)) where k is the idempotence index
 * 4. ring extension of a ring R with a polynomial q: formally it is isomorphic to R[x]/q(x)
 * 5. quadratic extension of a ring R with a polynomial x²-ax-b: formally it is isomorphic to R[x]/(x²-ax-b)
 */

    template<typename R>
    struct rational_t
    {
        R p;
        R q;
    };

/**
* @brief Rational Extension
* @details This is the field of fractions over R.
* @Requirements
* <ol>
 * <li>R is an integral domain. In particular, R should be an euclidean domain</li>
* <li>  R is not a field</li>
 * </ol>
* @Notes
* We excluded the case R is a field, because
* <ol>
 * <li> Otherwise, Frac(R) will be isomorphic to R</li>
* <li> Euclidean division is frequently not implemented when R is a field</li>
 * </ol>
*/
    template<typename R>
    class rational_extension
    {
        R p,q;
        void reduce()
        {
            auto d=gcd(p,q);
            p/=d;
            q/=d;
        }
    public:
        /**
        * Initialize a rational element
        * @Requirements
        * <strong>q</strong> is not zero
        */
        rational_extension(R _p=0,R _q=1):p(_p),q(_q)
        {
            reduce();
        }

        bool operator==(const rational_extension &O) const
        {
            return p*O.q==O.p*q;
        }

        bool operator!=(const rational_extension &O) const
        {
            return p*O.q!=O.p*q;
        }

        bool operator==(R a) const
        {
            return p==a*q;
        }
        bool operator!=(R a) const
        {
            return p!=a*q;
        }
        auto& operator+=(const rational_extension &o)
        {
            p=p*o.q+o.p*q;
            q*=o.q;
            reduce();
            return *this;
        }

        auto& operator-=(const rational_extension &o)
        {
            p=p*o.q-o.p*q;
            q*=o.q;
            reduce();
            return *this;
        }

        auto& operator*=(const rational_extension &o)
        {
            p*=o.p;
            q*=o.q;
            reduce();
            return *this;
        }

        auto operator+(const rational_extension &o) const
        {
            auto r=*this;
            return r+=o;
        }

        auto operator-(const rational_extension &o) const
        {
            auto r=*this;
            return r-=o;
        }

        auto operator*(const rational_extension &o) const
        {
            auto r=*this;
            return r*=o;
        }

        auto inv() const
        {
            return rational_extension(q,p);
        }
        auto& operator/=(const rational_extension &O)
        {
            return *this*=O.inv();
        }

        auto operator/(const rational_extension &O) const
        {
            auto r=*this;
            return r/=O;
        }

        auto operator-() const
        {
            return rational_extension(-p,q);
        }

        operator rational_t<R>() const
        {
            return rational_t<R>{p,q};
        }

        template <size_t k>
        auto& get()& {
            if constexpr (k == 0)
                return p;
            else return q;
        }

        template <size_t k>
        const auto& get() const& {
            if constexpr (k == 0)
                return p;
            else return q;
        }

        template <size_t k>
        auto&& get() const&& {
            if constexpr (k == 0)
                return p;
            else return q;
        }

        template <size_t k>
        auto&& get() && {
            if constexpr (k == 0)
                return p;
            else return q;
        }
    };

    /**
    * Extension of R into R[x]/x^n. with n the nilpotence index
    * @Requirements:
    * <strong>R</strong> is a commutative ring
    */
    template<typename R,int nilpotence>
    struct nilpotent_extension
    {
        std::vector<R> p;
        void reduce()
        {
            while(!p.empty() && is_zero(p.back()))
                p.pop_back();
        }
    public:
        nilpotent_extension(R k=0):p(1,k)
        {
            reduce();
        }
        nilpotent_extension(const std::vector<R> &_p)
        {
            int n=_p.size();
            p.resize(std::min(n,nilpotence));
            for(int i=0;i<std::min(n,nilpotence);i++)
                p[i]=_p[i];
            reduce();
        }
        auto& operator+=(const nilpotent_extension &O)
        {
            p.resize(std::max(p.size()),O.p.size());
            for(int i=0;i<O.size();i++)
                p[i]+=O.p[i];
            reduce();
            return *this;
        }

        auto& operator-=(const nilpotent_extension &O)
        {
            p.resize(std::max(p.size()),O.p.size());
            for(int i=0;i<O.size();i++)
                p[i]-=O.p[i];
            reduce();
            return *this;
        }

        nilpotent_extension operator*(const nilpotent_extension &O) const
        {
            if(p.empty() || O.p.empty())
                return 0;
            nilpotent_extension q;
            int n=p.size()-1,m=O.p.size()-1;
            q.p.resize(std::min(n+m+1,nilpotence));
            for(int i=0;i<=n;i++) for(int j=0;j<=m && (i+j) < nilpotence;j++)
                    q.p[i+j]+=p[i]*O.p[j];
            q.reduce();
            return q;
        }

        auto& operator*=(const nilpotent_extension &O)
        {
            auto q=(*this)*O;
            p.swap(q.p);
            return *this;
        }

        auto operator+(const nilpotent_extension &O) const
        {
            auto q=*this;
            return q+=O;
        }

        auto operator-(const nilpotent_extension &O) const
        {
            auto q=*this;
            return q-=O;
        }

        auto& operator[](int k)
        {
            return p[k];
        }

        const auto& operator[](int k) const
        {
            return p[k];
        }

        auto begin()
        {
            return p.begin();
        }

        auto end()
        {
            return p.end();
        }

        auto begin() const
        {
            return p.begin();
        }

        auto end() const
        {
            return p.end();
        }
    };

    struct nilpotence_t
    {
        int n;
    };

    /**
    * This is simply the union of R[x]/x^n over all n
    * @Requirements
    * <strong> R </strong> is a commutative ring
    * @Notes
    * If we don't specify the index manually. It will be equal as default to the public static member nilpotence
    * @Warning
    * the value of nilpotence should be initialized.
    */
    template<typename R>
    struct d_nilpotent_extension
    {
        std::vector<R> p;
        int index;
        void reduce()
        {
            while(!p.empty() && (is_zero(p.back())||p.size()>index))
                p.pop_back();
        }

    public:
        inline static int nilpotence;
        d_nilpotent_extension(R k=0,nilpotence_t nil_index={nilpotence}):p(1,k),index(nil_index.n)
        {
            reduce();
        }
        explicit d_nilpotent_extension(nilpotence_t index): d_nilpotent_extension(0,index){}
        d_nilpotent_extension(const std::vector<R> &_p,nilpotence_t nil_index={nilpotence}):index(nil_index.n)
        {
            int n=_p.size();
            p.resize(std::min(n,index));
            for(int i=0;i<std::min(n,index);i++)
                p[i]=_p[i];
            reduce();
        }
        auto& operator+=(const d_nilpotent_extension &O)
        {
            p.resize(std::max(p.size()),O.p.size());
            for(int i=0;i<O.size();i++)
                p[i]+=O.p[i];
            reduce();
            return *this;
        }

        auto& operator-=(const d_nilpotent_extension &O)
        {
            p.resize(std::max(p.size()),O.p.size());
            for(int i=0;i<O.size();i++)
                p[i]-=O.p[i];
            reduce();
            return *this;
        }

        d_nilpotent_extension operator*(const d_nilpotent_extension &O) const
        {
            if(p.empty() || O.p.empty())
                return 0;
            d_nilpotent_extension q(nilpotence_t{index});
            int n=p.size()-1,m=O.p.size()-1;
            q.p.resize(std::min(n+m+1,index));
            for(int i=0;i<=n;i++) for(int j=0;j<=m && (i+j) < index;j++)
                    q.p[i+j]+=p[i]*O.p[j];
            q.reduce();
            return q;
        }

        auto& operator*=(const d_nilpotent_extension &O)
        {
            auto q=(*this)*O;
            p.swap(q.p);
            return *this;
        }

        auto operator+(const d_nilpotent_extension &O) const
        {
            auto q=*this;
            return q+=O;
        }

        auto operator-(const d_nilpotent_extension &O) const
        {
            auto q=*this;
            return q-=O;
        }

        auto& operator[](int k)
        {
            return p[k];
        }

        const auto& operator[](int k) const
        {
            return p[k];
        }

        auto begin()
        {
            return p.begin();
        }

        auto end()
        {
            return p.end();
        }

        auto begin() const
        {
            return p.begin();
        }

        auto end() const
        {
            return p.end();
        }

        operator std::vector<R>&()
        {
            return p;
        }
    };

    /*
    * Extension of R into R[x]/(x^(n+1)-x^n). with n the idempotence index
    * @Requirements:
    * None
    */
    template<typename R,int idempotence>
    class idempotent_extension
    {
        std::vector<R> p;
        void reduce()
        {
            while(!p.empty() && is_zero(p.back()))
                p.pop_back();
        }
    public:
        idempotent_extension(R k=0):p(1,k)
        {
            reduce();
        }
        idempotent_extension(const std::vector<R> &_p)
        {
            int n=_p.size();
            p.resize(std::min(n,idempotence));
            for(int i=0;i<n;i++)
                p[i]=_p[std::min(i,idempotence-1)];
            reduce();
        }
        auto& operator+=(const idempotent_extension &O)
        {
            p.resize(std::max(p.size()),O.p.size());
            for(int i=0;i<O.size();i++)
                p[i]+=O.p[i];
            reduce();
            return *this;
        }

        auto& operator-=(const idempotent_extension &O)
        {
            p.resize(std::max(p.size()),O.p.size());
            for(int i=0;i<O.size();i++)
                p[i]-=O.p[i];
            reduce();
            return *this;
        }

        idempotent_extension operator*(const idempotent_extension &O) const
        {
            if(p.empty() ||O.p.empty())
                return 0;
            idempotent_extension q;
            int n=p.size()-1,m=O.p.size()-1;
            q.p.resize(std::min(n+m+1,idempotence));
            for(int i=0;i<=n;i++) for(int j=0;j<=m;j++)
                    q.p[std::min(i+j,idempotence-1)]+=p[i]*O.p[j];
            q.reduce();
            return q;
        }

        auto& operator*=(const idempotent_extension &O)
        {
            auto q=(*this)*O;
            p.swap(q.p);
            return *this;
        }

        auto operator+(const idempotent_extension &O) const
        {
            auto q=*this;
            return q+=O;
        }

        auto operator-(const idempotent_extension &O) const
        {
            auto q=*this;
            return q-=O;
        }

        auto begin()
        {
            return p.begin();
        }

        auto end()
        {
            return p.end();
        }

        auto begin() const
        {
            return p.begin();
        }

        auto end() const
        {
            return p.end();
        }

        auto& operator[](int k)
        {
            return p[k];
        }

        const auto& operator[](int k) const
        {
            return p[k];
        }
    };

    /**
     * Extension of the ring R to R[x]/q(x) where q is a given poylnomial
     * @Requirements
     * One of the following:
     *  - R is a commutative ring and q is a monic polynomial
     *  - R is a field and q is not zero
     * @Warning
     * the value of q should be initialized.
     */
    template<typename R>
    class ring_extension
    {
        polynomial<R> p;
    public:
        void reduce()
        {
            if(p.degree()>=q.degree())
                p=p%q;
            p.reduce();
        }

        inline static polynomial<R> q;
        ring_extension(R k=0):p(k)
        {
            reduce();
        }
        ring_extension(const std::vector<R> &_p):p(_p)
        {
            reduce();
        }
        auto& operator+=(const ring_extension &O)
        {
            p+=O.p;
            return *this;
        }

        auto& operator-=(const ring_extension &O)
        {
            p-=O.p();
            return *this;
        }

        auto operator*(const ring_extension &O) const
        {
            auto q=*this;
            return q*=O;
        }

        auto& operator*=(const ring_extension &O)
        {
            p*=O.p;
            reduce();
            return *this;
        }

        auto operator+(const ring_extension &O) const
        {
            auto q=*this;
            return q+=O;
        }

        auto operator-(const ring_extension &O) const
        {
            auto q=*this;
            return q-=O;
        }

        /*
         * Calculates the multiplicative inverse of an element p
         * @Requirements
         * 1. d=gcd(p,q) is a non-zero constant polynomial
         * 2. One of the following:
         *  2.1 R is a commutative ring and d is invertible
         *  2.2 R is an euclidean domain and d | a
         * @Notes
         * For (2.2),The divisibility relation is defined as:
         * d | a iff d | s for all s in a
         */
        auto inv() const
        {
            auto [a,b,d]= egcd(p,q);
            return a/d[0];
        }

        /*
         * Calculates a multiplicative pseudo-inverse of an element p
         * @Requirements
         * None
         */
        auto pinv() const
        {
            return egcd(p,q).a;
        }

        auto& operator/=(const ring_extension &O)
        {
            return (*this)*=O.inv();
        }

        auto operator/(const ring_extension &O) const
        {
            return (*this)*O.inv();
        }

        auto& operator/=(R k)
        {
            for(auto &s:p)
                s/=k;
            return *this;
        }

        auto& operator*=(R k)
        {
            for(auto &s:p)
                s*=k;
            return *this;
        }

        auto operator/(R k) const
        {
            auto q=*this;
            return q/=k;
        }

        auto operator*(R k) const
        {
            auto q=*this;
            return q*=k;
        }

        auto begin()
        {
            return p.begin();
        }

        auto end()
        {
            return p.end();
        }

        auto begin() const
        {
            return p.begin();
        }

        auto end() const
        {
            return p.end();
        }

        auto& operator[](int k)
        {
            return p[k];
        }

        const auto& operator[](int k) const
        {
            return p[k];
        }
    };


    /*
     * Extension of the commutative ring R into R[x]/(x²-ax-b)
     * @Requirements:
     * None
     * */
    template<typename R,integer a,integer b>
    class quadratic_extension
    {
        std::array<R,2> p;
    public:

        quadratic_extension(R k=0,R s=0):p({k,s})
        {
        }
        quadratic_extension(const std::array<R,2> &_p):p(_p)
        {
        }

        auto conj() const
        {
            return quadratic_extension(p[0]+a*p[1],-p[1]);
        }

        auto& operator+=(const quadratic_extension &O)
        {
            p[0]+=O.p[0];
            p[1]+=O.p[1];
            return *this;
        }

        auto& operator-=(const quadratic_extension &O)
        {
            p[0]-=O.p[0];
            p[1]-=O.p[1];
            return *this;
        }

        auto operator*(const quadratic_extension &O) const
        {
            quadratic_extension q;
            auto s=p[1]*O.p[1];
            q.p[0]=p[0]*O.p[0]+R(b)*s;
            q.p[1]=p[0]*O.p[1]+p[1]*O.p[0]+R(a)*s;
            return q;
        }

        auto& operator*=(const quadratic_extension &O)
        {
            p=((*this)*O).p;
            return *this;
        }

        auto operator+(const quadratic_extension &O) const
        {
            auto q=*this;
            return q+=O;
        }

        auto operator-(const quadratic_extension &O) const
        {
            auto q=*this;
            return q-=O;
        }


        /*
         * Calculate the multiplicative inverse of p
         * @Requirements:
         * One of the following:
         * - p0²+a*p0*p1-b*p1² is inversible
         * - R is an euclidean domain and p0²+a*p0*p1-b*p1² | conj(p)
         * @Notes
         * For (2.2),The divisibility relation is defined as:
         * d | a iff d | s for all s in a
         * */
        auto inv() const
        {
            return conj()/(p[0]*p[0]+R(a)*p[0]*p[1]-R(b)*p[1]*p[1]);
        }

        /*
         * Divides p by q
         * @Requirements:
         * One of the following:
         * - p0²+a*p0*p1-b*p1² is inversible
         * - R is an euclidean domain and p0²+a*p0*p1-b*p1² | p*conj(q)
         * @Notes
         * 1. Guaranteed to work in an integral domain even if the element O is not inversible,
         * but there still exists Q such that P=Q*O => P/O=Q
         * 2. The divisibility relation is defined as: d | a iff d | s for all s in a
         * */
        auto& operator/=(const quadratic_extension &q)
        {
            *this*=q.conj();
            return *this/=p[0]*p[0]+R(a)*p[0]*p[1]-R(b)*p[1]*p[1];
        }
        /*
         * Divides p by q and store it to p itself
         * @Requirements:
         * One of the following:
         * - p0²+a*p0*p1-b*p1² is inversible
         * - R is an euclidean domain and p0²+a*p0*p1-b*p1² | p*conj(q)
         * @Notes
         * 1. Guaranteed to work in an integral domain even if the element O is not inversible,
         * but there still exists Q such that P=Q*O => P/O=Q
         * 2. The divisibility relation is defined as: d | a iff d | s for all s in a
         * */
        auto operator/(const quadratic_extension &O) const
        {
            return ((*this)*O.conj())/(O.p[0]*O.p[0]+R(a)*O.p[0]*O.p[1]-R(b)*O.p[1]*O.p[1]);
        }

        /*
         * Divides p by k and store it to p itself
         * @Requirements:
         * One of the following:
         * - k is inversible
         * - R is an euclidean domain and k | p
         * @Notes
         * The divisibility relation is defined as: d | a iff d | s for all s in a
         * */
        auto& operator/=(R k)
        {
            p[0]/=k;
            p[1]/=k;
            return *this;
        }

        auto& operator*=(R k)
        {
            p[0]*=k;
            p[1]*=k;
            return *this;
        }

        auto& operator-=(R k)
        {
            p[0]-=k;
            return *this;
        }

        auto& operator+=(R k)
        {
            p[0]+=k;
            return *this;
        }

        auto operator+(R k) const
        {
            auto q=*this;
            return q+=k;
        }

        auto operator-(R k) const
        {
            auto q=*this;
            return q-=k;
        }

        auto operator/(R k) const
        {
            auto q=*this;
            return q/=k;
        }

        auto operator*(R k) const
        {
            auto q=*this;
            return q*=k;
        }

        auto begin()
        {
            return p.begin();
        }

        auto end()
        {
            return p.end();
        }

        auto begin() const
        {
            return p.begin();
        }

        auto end() const
        {
            return p.end();
        }

        auto& operator[](int k)
        {
            return p[k];
        }

        const auto& operator[](int k) const
        {
            return p[k];
        }

        template <size_t k>
        auto& get()&
        {
            return p[k];
        }

        template <size_t k>
        const auto& get() const&
        {
            return p[k];
        }

        template <size_t k>
        auto&& get() const &&
        {
            return p[k];
        }

        template <size_t k>
        auto&& get() &&
        {
            return p[k];
        }
    };

    template<typename R,integer a,integer b>
    auto operator+(R k,const quadratic_extension<R,a,b> &O)
    {
        quadratic_extension<R,a,b> q=O;
        return q+=k;
    }

    template<typename R,integer a,integer b>
    auto operator*(R k,const quadratic_extension<R,a,b> &O)
    {
        quadratic_extension<R,a,b> q=O;
        return q*=k;
    }

    template<typename R,integer a,integer b>
    auto operator-(R k,const quadratic_extension<R,a,b> &O)
    {
        quadratic_extension<R,a,b> q=O;
        return q-=k;
    }

    /*
     * Dynamic ring extension
     * For a ring R, It is the union of R[x]/q(x) over all polynomials q
     * */

    template<typename R>
    struct extension_polynomial_t
    {
        polynomial<R> p;
    };

    /**
     * @brief Dynamic Ring Extension
     * @details It is simply the union of R[x]/q over all polynomials q
     * @Requirements
     * One of the following:
     *  - R is a commutative ring and q is a monic polynomial
     *  - R is a field and q is not zero
     * @Notes
     * If the polynomial is not specified, its value will be by default that of the public static member extension_polynomial
     * @Warning
     * the value of extension_polynomial should be initialized.
     */
    template<typename R>
    class d_ring_extension
    {
        polynomial<R> p;
        polynomial<R> q;
    public:
        void reduce()
        {
            if(p.degree()>=q.degree())
                p=p%q;
            p.reduce();
        }

        inline static polynomial<R> extension_polynomial;
        d_ring_extension(R k=0,extension_polynomial_t<R> ext={extension_polynomial}):p(k),q(ext.p)
        {
            reduce();
        }
        d_ring_extension(const std::vector<R> &_p,extension_polynomial_t<R> ext={extension_polynomial}):p(_p),q(ext.p)
        {
            reduce();
        }

        d_ring_extension(extension_polynomial_t<R> ext):q(ext.p){}

        auto& operator+=(const d_ring_extension &O)
        {
            p+=O.p;
            return *this;
        }

        auto& operator-=(const d_ring_extension &O)
        {
            p-=O.p();
            return *this;
        }

        auto operator*(const d_ring_extension &O) const
        {
            auto q=*this;
            return q*=O;
        }

        auto& operator*=(const d_ring_extension &O)
        {
            p*=O.p;
            reduce();
            return *this;
        }

        auto operator+(const d_ring_extension &O) const
        {
            auto q=*this;
            return q+=O;
        }

        auto operator-(const d_ring_extension &O) const
        {
            auto q=*this;
            return q-=O;
        }

        auto inv() const
        {
            auto [a,b,d]= egcd(p,q);
            return a/d[0];
        }

        auto pinv() const
        {
            return egcd(p,q).a;
        }

        auto& operator/=(const d_ring_extension &O)
        {
            return (*this)*=O.inv();
        }

        auto operator/(const d_ring_extension &O) const
        {
            return (*this)*O.inv();
        }

        auto begin()
        {
            return p.begin();
        }

        auto end()
        {
            return p.end();
        }

        auto begin() const
        {
            return p.begin();
        }

        auto end() const
        {
            return p.end();
        }

        auto& operator[](int k)
        {
            return p[k];
        }

        const auto& operator[](int k) const
        {
            return p[k];
        }
    };
}

namespace std
{
    template<typename R,int a,int b>
    struct tuple_size<cp::quadratic_extension<R,a,b>> : integral_constant<size_t, 2> {};
    template<size_t k, typename R,int a,int b>
    struct tuple_element<k, cp::quadratic_extension<R,a,b>>
    {
        using type = R;
    };
    template<typename R>
    struct tuple_size<cp::rational_extension<R>> : integral_constant<size_t, 2> {};
    template<size_t k, typename R>
    struct tuple_element<k, cp::rational_extension<R>>
    {
        using type = R;
    };
}
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

    inline integer totient_rec(integer n,const std::vector<integer> &P, abstract_factoriser &F, integer o=0)
    {
        if(n==0)
            return 0;
        integer S=n;
        for(int i=o;i<P.size();i++)
            S-= totient_rec(n/P[i],P,F,i+1);
        return S;
    }

    inline integer totient(integer n,integer m, abstract_factoriser &F)
    {
        if(n==0)
            return 0;
        auto r=m%n;
        auto P= F.prime_factors(n);
        return (m/n)*totient(n,F)+totient_rec(r,P,F);
    }
}

#endif //CPLIBRARY_FUNCTIONS_H
#include <random>

template<cp::integer m>
struct std::hash<cp::cyclic<m>>
{
    inline static std::random_device dev=std::random_device();
    inline static std::mt19937 g=std::mt19937(dev());
    inline static constexpr cp::integer M=1e9+7;
    std::uniform_int_distribution<cp::integer> d=std::uniform_int_distribution<cp::integer>(1,M);
    cp::integer a=d(g),b=d(g);
    public:
    size_t operator()(const cp::cyclic<m> &x) const noexcept
    {
        return (a*static_cast<cp::integer>(x)+b)%M;
    }
};

namespace cp
{
    template<typename cyclic_ring>
    integer discrete_log(cyclic_ring a, cyclic_ring r)
    {
        integer s=std::ceil(std::sqrt(cyclic_ring::m));
        cyclic_ring u=pow(a,s),w=1;
        std::unordered_map<cyclic_ring,integer> mapper;
        for(integer i=0;i<=s;i++,w*=a)
            mapper[r*w]=i;
        w=u;
        for(integer i=1;i<=s;i++,w*=u)
            if(mapper.count(w))
                return i*s-mapper[w];
        return -1;
    }

    inline std::vector<integer> inverse_table(int n,int prime)
    {
        std::vector<integer> I(n + 1);
        I[0] = I[1] = 1;
        for (int i = 2; i <= n; i++)
            I[i] = I[prime % i] *
                   (prime - prime / i) % prime;
        return I;
    }

    inline integer primitive_root_of_unity(integer p,abstract_factoriser &F)
    {
        auto phi=carmichael_totient(p,F);
        auto D=F.divisors_list(phi);
        for(integer k=2;k<p-1;k++) if(std::gcd(k,p)==1)
        {
            bool is_primitive=true;
            for (auto d: D)
                if(d< phi && pow(cyclic<dynamic_modulus>(k,p),d,p)==1)
                {
                    is_primitive=false;
                    break;
                }
            if(is_primitive)
                return k;
        }
        return 0;
    }

    template <integer p>
    integer primitive_root_of_unity(abstract_factoriser& F)
    {
        static auto phi = totient(p,F);
        static auto D = F.divisors_list(phi);
        for (integer k = 2; k < p - 1; k++)
        {
            bool is_primitive = true;
            for (auto d : D)
                if (d < phi && pow<d_cyclic>(k, d) == 1)
                {
                    is_primitive = false;
                    break;
                }
            if (is_primitive)
                return k;
        }
        return 0;
    }

    template<integer m>
    integer legendre_symbol(cyclic<m> a)
    {
        integer r;
        if constexpr (m==dynamic_modulus)
            r= (integer) pow(a, (a.modulus() - 1) / 2,a.modulus());
        else
            r= (integer)pow(a, (a.modulus() - 1) / 2);
        if (r > a.modulus() / 2)
            r -= a.modulus();
        return r;
    }

    template<integer m>
    cyclic<m> sqrt(cyclic<m> n)
    {
        using cyclic_field = cyclic<m>;
        if (n.modulus() == 2)
            return n;
        cyclic_field a = 2;
        while (legendre_symbol(a*a-n) != -1)
            ++a;
        extension_polynomial_t<cyclic_field> q = { polynomial<cyclic_field>({n-a * a,0,1}) };
        d_ring_extension<cyclic_field> phi(std::vector<cyclic_field>{ a,1 }, q);
        return pow(phi, (cyclic_field::m+1)/2,q)[0];
    }
}

#endif //CPLIBRARY_MODULAR_FUNCTIONS_H


#endif //ACPC_PREPARATION_FFT_H
//
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_SPARSE_POLYNOMIAL_H
#define CPLIBRARY_SPARSE_POLYNOMIAL_H
#include <map>
#include <vector>
#include <cstdint>

namespace cp
{
    using integer=std::int64_t;
/**
 * @brief Sparse Polynomial
* @details This is the class of sparse polynomials over commutative ring R
* @Requirements
* <strong>R</strong> is a commutative ring
* @Recommendation
* <ol> <li> The coefficients are sparse. Formally a k-sparse polynomial p of degree n is a polynomial where:
* (card supp {p_1,..,p_n}) / n <= k
* <li> It is recommended that k<=0.01 </ol>
* @Notes
* Formally this class is simply R[x]
*/
    template<typename R>
    class sparse_polynomial
    {
        std::map<cp::integer,R> p;
        void reduce()
        {
            std::vector<cp::integer> to_del;
            for(auto [k,x]:p)
                if(is_zero(x))
                    to_del.push_back(k);
            for(auto k:to_del)
                p.erase(k);
        }
    public:
        sparse_polynomial(R k=R{})
        {
            p[0]=k;
            reduce();
        }

        template<std::integral I>
        sparse_polynomial(I k)
        {
            p[0]=k;
            reduce();
        }


        sparse_polynomial(const std::vector<R> &_p)
        {
            for(int i=0;i<_p.size();i++)
                p[i]=_p[i];
            reduce();
        }

        int degree() const
        {
            return p.empty()?-1:p.rbegin()->first;
        }

        sparse_polynomial& operator+=(const sparse_polynomial& O)
        {
            for(const auto& [k,s]:O.p)
            {
                p[k] += s;
                if(is_zero(p[k]))
                    p.erase(k);
            }
            return *this;
        }

        sparse_polynomial& operator-=(const sparse_polynomial& O)
        {
            for(const auto& [k,s]:O.p)
            {
                p[k] -= s;
                if(is_zero(p[k]))
                    p.erase(k);
            }
            return *this;
        }

        sparse_polynomial operator*(const sparse_polynomial &O) const
        {

            sparse_polynomial q;
            for(auto [i,u]:p) for(auto [j,v]:O.p)
                {
                    q.p[i+j]+=u*v;
                    if(is_zero(q.p[i+j]))
                        q.p.erase(i+j);
                }
            return q;
        }

        sparse_polynomial& operator*=(const sparse_polynomial &O)
        {
            auto r=(*this)*O;
            p.swap(r.p);
            return *this;
        }

        sparse_polynomial operator+(const sparse_polynomial &O) const
        {
            auto r=*this;
            return r+=O;
        }

        sparse_polynomial operator-(const sparse_polynomial &O) const
        {
            auto r=*this;
            return r-=O;
        }

        sparse_polynomial operator-() const
        {
            auto r=*this;
            for(auto &[_,s]:r.p)
                s=-s;
            return r;
        }

        sparse_polynomial operator*=(R a)
        {
            if(is_zero(a))
                p.clear();
            else for(auto& [_,s]:p)
                    s*=a;
            reduce();
            return *this;
        }

        sparse_polynomial& operator/=(R k)
        {
            for(auto &s:p)
                s/=k;
            return *this;
        }

        sparse_polynomial operator/(R k) const
        {
            auto q=*this;
            return q/=k;
        }

        R &operator[](cp::integer k)
        {
            return p[k];
        }

        const R& operator[](cp::integer k) const
        {
            return p.at(k);
        }

        /**
         * @brief Polynomial evaluation
        * @details Evaluates the polynomial at a point a.
        * @Requirements:
        * H is an associative algebra over R
        */
        template<typename H>
        std::common_type<H,R>::type operator()(H a) const
        {
            typename std::common_type<H,R>::type r=0,u=1;
            cp::integer i=0;
            for(auto [k,x]:p)
            {
                u*=pow(a,k-i);
                r+=u*x;
                i=k;
            }
            return r;
        }

        operator std::map<cp::integer, R>& ()
        {
            return p;
        }

        operator const std::map<cp::integer, R>& () const
        {
            return p;
        }

        auto size() const
        {
            return p.size();
        }

        std::map<cp::integer,R>& data()
        {
            return p;
        }

        const std::map<cp::integer,R>& data() const
        {
            return p;
        }

        sparse_polynomial derivative() const
        {
            sparse_polynomial q;
            for(auto [k,x]:p)
                q.p.emplace_hint(q.p.end(),k-1,k*x);
            return q;
        }
    };

    template<typename R>
    sparse_polynomial<R> Z=sparse_polynomial<R>(std::vector<R>{0,1});

    template<typename R>
    sparse_polynomial<R> operator*(R a,const sparse_polynomial<R> &p)
    {
        auto q=p;
        return q*=a;
    }

    template<typename R>
    sparse_polynomial<R> operator-(R a,const sparse_polynomial<R> &p)
    {
        auto q=-p;
        return q+=a;
    }

    template<typename R>
    sparse_polynomial<R> operator+(R a,const sparse_polynomial<R> &p)
    {
        auto q=p;
        return q+=a;
    }

}

#endif //CPLIBRARY_SPARSE_POLYNOMIAL_H
//
// Created by ASUS on 01/12/2021.
//
#ifndef __DATA_RANGE_QUERIES_STRUCTURES_H__
#define __DATA_RANGE_QUERIES_STRUCTURES_H__
#include <vector>
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_FIXED_PREFIX_ARRAY_H
#define CPLIBRARY_FIXED_PREFIX_ARRAY_H
#include <vector>
namespace cp::data_structures::fixed
{
    template<typename O>
    struct prefix_array
    {
        using R=typename O::type;
        using type=typename O::type;
        std::vector<R> A;
        std::vector<R> P;
        inline static O F=O();
        prefix_array(const std::vector<R> &_A):A(_A),P(_A.size()+1)
        {
            P[0]=O::neutral;
            for(int i=0;i<A.size();i++)
                P[i+1]=F(P[i],A[i]);
        }

        R query(int l,int r)
        {
            return F(F.inv(P[l]),P[r]);
        }

        void update(int i,R u)
        {
            A[i]=u;
            for(int j=i+1;j<P.size();j++)
                P[j]=F(P[j-1],A[j-1]);
        }
    };

}

#endif //CPLIBRARY_PREFIX_ARRAY_H
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_FIXED_SEGMENT_TREE_H
#define CPLIBRARY_FIXED_SEGMENT_TREE_H
#include <vector>
namespace cp::data_structures::fixed
{
    template<typename O>
    struct segment_tree
    {
        using R=typename O::type;
        using type=R;
        std::vector<std::vector<R>> S;
        std::vector<R> A;
        int n,h;
        segment_tree(const std::vector<R> &_A):A(_A)
        {
            n=bit_ceil(A.size());
            A.resize(n,O::neutral);
            int m=n;
            h=0;
            while(m)
            {
                m/=2;
                h++;
            }
            S.resize(h);
            for(int i=0;i<h;i++)
                S[i].resize(1<<i);
            build();
        }

        void update(int i,R u)
        {
            A[i]=u;
            S[h-1][i]=u;
            int m=h-2;
            i/=2;
            while(m>=0)
            {
                S[m][i]=F(S[m+1][2*i],S[m+1][2*i+1]);
                m--;
                i/=2;
            }
        }

        R query(int l,int r)
        {
            return query(std::max(l,0),std::min(r,n),0,n,0);
        }
    private:
        inline static O F=O();
        void build()
        {
            for(int i=0;i<n;i++)
                S.back()[i]=A[i];
            for(int i=h-2;i>=0;i--) for(int k=0;k<(1<<i);k++)
                    S[i][k]=F(S[i+1][2*k],S[i+1][2*k+1]);
        }
        R query(int l,int r,int a,int b,int depth)
        {
            if(l>=r)
                return O::neutral;
            if(l==a && r==b)
                return S[depth][l>>(h-1-depth)];
            int mid=(a+b)/2;
            if(mid>r)
                return query(l,r,a,mid,depth+1);
            else if(mid<l)
                return query(l,r,mid,b,depth+1);
            else
                return F(query(l,mid,a,mid,depth+1),query(mid,r,mid,b,depth+1));
        }
    };
}

#endif //CPLIBRARY_SEGMENT_TREE_H
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_FIXED_FENWICK_TREE_H
#define CPLIBRARY_FIXED_FENWICK_TREE_H
#include <vector>
namespace cp::data_structures::fixed
{
    template<typename O>
    struct fenwick_tree {
        int n;
        using T=typename O::type;
        std::vector<T> bit;
        inline static O F = O();

        fenwick_tree(int _n):n(_n),bit(n,O::neutral){}
        fenwick_tree(const std::vector<T> &X) : fenwick_tree(X.size())
        {
            for(int i=0;i<n;i++)
                update(i,X[i]);
        }
        T sum(int x) {
            if(x<0)
                return O::neutral;
            T ret = O::neutral;
            for (int i = x; i >= 0; i = (i & (i + 1)) - 1)
                ret = F(ret,bit[i]);
            return ret;
        }

        T query(int a,int b)
        {
            return F(F.inv(sum(a-1)),sum(b));
        }

        T sum(int a,int b)
        {
            return query(a,b);
        }

        void add(int x, T delta) {
            for (int i = x; i < n; i = i | (i + 1))
                bit[i] = F(bit[i], delta);
        }

        void update(int x, T delta) {
            add(x,F(F.inv(sum(x,x)),delta));
        }
    };
}

#endif //CPLIBRARY_FENWICK_TREE_H


#endif
//
// Created by ramizouari on 27/11/23.
//

#ifndef CPLIBRARY_FFT_H
#define CPLIBRARY_FFT_H
//
// Created by ramizouari on 27/11/23.
//

#ifndef CPLIBRARY_VIEW_H
#define CPLIBRARY_VIEW_H
#include <array>
#include <utility>
#include <vector>
#include <numeric>
namespace cp::linalg
{
    inline constexpr std::size_t dynamic_extent = -1;
    template<typename R,std::size_t Rank>
    struct tensor_subview;
    template<typename R,std::size_t Rank>
    struct tensor_view
    {
        virtual R& at(std::array<std::size_t,Rank> indexes) = 0;
        virtual const R& at(std::array<std::size_t,Rank> indexes) const = 0;
        virtual ~tensor_view()= default;
        template<typename ...Args>
        R& at(Args... args)
        {
            return at(std::array<std::size_t,Rank>{args...});
        }
        template<typename ...Args>
        const R& at(Args... args) const
        {
            return at(std::array<std::size_t,Rank>{args...});
        }

        template<typename ...Args>
        R& operator()(Args... args)
        {
            return at(std::array<std::size_t,Rank>{static_cast<std::size_t>(args)...});
        }

        template<typename ...Args>
        const R& operator()(Args... args) const
        {
            return at(std::array<std::size_t,Rank>{static_cast<std::size_t>(args)...});
        }


        const R& operator()(std::array<std::size_t,Rank> args) const
        {
            return at(std::move(args));
        }

        R& operator()(std::array<std::size_t,Rank> args)
        {
            return at(std::move(args));
        }

        virtual std::array<std::size_t,Rank> shape() const = 0;
        static constexpr std::size_t rank()
        {
            return Rank;
        }
        virtual std::size_t size() const
        {
            auto s=shape();
            return std::accumulate(s.begin(),s.end(),1,std::multiplies<>());
        }

        struct iterator
        {
            tensor_view<R,Rank> &src;
            std::array<std::size_t,Rank> indexes;
            bool is_end=false;
            iterator(tensor_view<R,Rank> &src,std::array<std::size_t,Rank> indexes,bool is_end=false):src(src),indexes(indexes),is_end(is_end){}
            iterator& operator++()
            {
                auto shape=src.shape();
                for(int i=Rank-1;i>=0;i--)
                {
                    if(indexes[i]+1<shape[i])
                    {
                        indexes[i]++;
                        return *this;
                    }
                    else
                        indexes[i]=0;
                }
                is_end=true;
                return *this;
            }
            iterator operator++(int)
            {
                iterator tmp=*this;
                ++(*this);
                return tmp;
            }
            bool operator==(const iterator& rhs) const
            {
                return is_end==rhs.is_end && indexes==rhs.indexes;
            }

            bool operator!=(const iterator& rhs) const
            {
                return !(*this==rhs);
            }

            R& operator*()
            {
                return src.at(indexes);
            }

            R* operator->()
            {
                return &src.at(indexes);
            }
        };
        iterator begin()
        {
            return iterator(*this,std::array<std::size_t,Rank>{},false);
        }
        iterator end()
        {
            auto shape=this->shape();
            return iterator(*this,std::array<std::size_t,Rank>{},true);
        }
        virtual tensor_subview<R,Rank> slice(std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end);
        virtual tensor_subview<R,Rank> slice(std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end,std::array<std::size_t,Rank> step);
    };


    template <typename R,std::size_t Rank>
    struct tensor_subview: public tensor_view<R,Rank>
    {
        tensor_view<R,Rank> &src;
        std::array<std::size_t,Rank> m_start,m_end,m_step;
        tensor_subview(tensor_view<R,Rank> &src,std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end):src(src),m_start(start),m_end(end)
        {
            std::fill(m_step.begin(),m_step.end(),1);
        }
        tensor_subview(tensor_view<R,Rank> &src,std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end,
                       std::array<std::size_t,Rank> step):src(src),m_start(start),m_end(end),m_step(step){}

        R& at(std::array<std::size_t,Rank> indexes) override
        {
            std::array<std::size_t,Rank> new_indexes;
            for(int i=0;i<Rank;i++)
                new_indexes[i]=indexes[i]*m_step[i] + m_start[i];
            return src.at(new_indexes);
        }
        const R& at(std::array<std::size_t,Rank> indexes) const override
        {
            std::array<std::size_t,Rank> new_indexes;
            for(int i=0;i<Rank;i++)
                new_indexes[i]=indexes[i]*m_step[i] + m_start[i];
            return src.at(new_indexes);
        }
        std::array<std::size_t,Rank> shape() const override
        {
            std::array<std::size_t,Rank> new_shape;
            for(int i=0;i<Rank;i++)
                new_shape[i]=(m_end[i]-m_start[i]+m_step[i]-1)/m_step[i];
            return new_shape;
        }
        tensor_subview<R,Rank> slice(std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end) override
        {
            std::array<std::size_t,Rank> new_start,new_end;
            for(int i=0;i<Rank;i++)
            {
                new_start[i]=this->m_start[i]+start[i]*m_step[i];
                new_end[i]=this->m_start[i]+end[i]*m_step[i];
            }
            return tensor_subview<R,Rank>(src,new_start,new_end,m_step);
        }
        tensor_subview<R,Rank> slice(std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end,std::array<std::size_t,Rank> step) override
        {
            std::array<std::size_t,Rank> new_start,new_end,new_step;
            for(int i=0;i<Rank;i++)
            {
                new_start[i]=this->m_start[i]+start[i]*m_step[i];
                new_end[i]=this->m_start[i]+end[i]*m_step[i];
                new_step[i]=this->m_step[i]*step[i];
            }
            return tensor_subview<R,Rank>(src,new_start,new_end,new_step);
        }
    };

    template<typename R,std::size_t Rank>
    tensor_subview<R,Rank> tensor_view<R,Rank>::slice(std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end)
    {
        return tensor_subview<R,Rank>(*this,start,end);
    }

    template<typename R,std::size_t Rank>
    tensor_subview<R,Rank> tensor_view<R,Rank>::slice(std::array<std::size_t,Rank> start,std::array<std::size_t,Rank> end,std::array<std::size_t,Rank> step)
    {
        return tensor_subview<R,Rank>(*this,start,end,step);
    }

    template<typename R>
    struct tensor_view<R,dynamic_extent>
    {
        virtual ~tensor_view()= default;

        template<typename ...Args>
        R& at(Args... args)
        {
            return at(std::vector<std::size_t>{args...});
        }
        template<typename ...Args>
        const R& at(Args... args) const
        {
            return at(std::vector<std::size_t>{args...});
        }

        template<typename ...Args>
        R& operator()(Args... args)
        {
            return at(args...);
        }

        template<typename ...Args>
        const R& operator()(Args... args) const
        {
            return at(args...);
        }
        virtual R& at(std::vector<std::size_t> indexes) = 0;
        virtual const R& at(std::vector<std::size_t> indexes) const = 0;

        [[nodiscard]] virtual std::vector<std::size_t> shape() const = 0;
        virtual std::size_t rank() const
        {
            return shape().size();
        }
        virtual std::size_t size() const
        {
            auto s=shape();
            return std::accumulate(s.begin(),s.end(),1,std::multiplies<>());
        }

        struct iterator
        {
            tensor_view<R,dynamic_extent> &src;
            std::vector<std::size_t> indexes;
            bool is_end=false;
            iterator(tensor_view<R,dynamic_extent> &src,std::vector<std::size_t> indexes,bool is_end=false):src(src),indexes(std::move(indexes)),is_end(is_end){}
            iterator& operator++()
            {
                for(int i=src.rank()-1;i>=0;i--)
                {
                    if(indexes[i]+1<src.shape()[i])
                    {
                        indexes[i]++;
                        return *this;
                    }
                    else
                        indexes[i]=0;
                }
                is_end=true;
                return *this;
            }
            iterator operator++(int)
            {
                iterator tmp=*this;
                ++(*this);
                return tmp;
            }
            bool operator==(const iterator& rhs) const
            {
                return is_end==rhs.is_end && indexes==rhs.indexes;
            }

            bool operator!=(const iterator& rhs) const
            {
                return !(*this==rhs);
            }

            R& operator*()
            {
                return src.at(indexes);
            }
        };
        iterator begin()
        {
            auto zeros=shape();
            for(auto &x:zeros)
                x=0;
            return iterator(*this,zeros,false);
        }
        iterator end()
        {
            auto zeros=shape();
            for(auto &x:zeros)
                x=0;
            return iterator(*this,zeros,true);
        }
        virtual tensor_subview<R,dynamic_extent> slice(std::vector<std::size_t> start,std::vector<std::size_t> end);
        virtual tensor_subview<R,dynamic_extent> slice(std::vector<std::size_t> start,std::vector<std::size_t> end,std::vector<std::size_t> step);
    };

    template<typename R>
    struct vector_view : public tensor_view<R,1>
    {
        R* m_data;
        std::size_t m_size;
        vector_view(R* _data,std::size_t _size):m_data(_data),m_size(_size){}
        vector_view(std::vector<R> &v):m_data(v.data()),m_size(v.size()){}
        template<std::size_t N>
        vector_view(std::array<R,N> &v):m_data(v.data()),m_size(v.size()){}
        std::size_t size() const override
        {
            return m_size;
        }
        virtual R& at(std::size_t i)
        {
            return m_data[i];
        }
        virtual const R& at(std::size_t i) const
        {
            return m_data[i];
        }
        virtual ~vector_view()= default;
        R& at(std::array<std::size_t,1> indexes) override
        {
            return at(indexes[0]);
        }
        const R& at(std::array<std::size_t,1> indexes) const override
        {
            return at(indexes[0]);
        }
        std::array<std::size_t,1> shape() const override
        {
            return {size()};
        }
        tensor_subview<R,1> slice(std::array<std::size_t,1> start,std::array<std::size_t,1> end) override
        {
            return tensor_subview<R,1>(*this,start,end);
        }

        tensor_subview<R,1> slice(std::array<std::size_t,1> start,std::array<std::size_t,1> end, std::array<std::size_t,1> step) override
        {
            return tensor_subview<R,1>(*this,start,end,step);
        }

        vector_view& operator=(const std::vector<R>& O)
        {
            for(int i=0;i<size();i++)
                at(i)=O.at(i);
            return *this;
        }
    };

    template<typename R>
    struct tensor_subview<R,dynamic_extent>:tensor_view<R,dynamic_extent>
    {
        tensor_view<R,dynamic_extent> &src;
        std::vector<std::size_t> m_start,m_end,m_step;
        tensor_subview(tensor_view<R,dynamic_extent> &src,std::vector<std::size_t> start,std::vector<std::size_t> end):src(src),m_start(start),m_end(end),m_step(src.rank())
        {
            std::fill(m_step.begin(),m_step.end(),1);
        }
        tensor_subview(tensor_view<R,dynamic_extent> &src,std::vector<std::size_t> start,std::vector<std::size_t> end,
                       std::vector<std::size_t> step):src(src),m_start(start),m_end(end),m_step(step){}

        R& at(std::vector<std::size_t> indexes) override
        {
            std::vector<std::size_t> new_indexes(src.rank());
            for(int i=0;i<src.rank();i++)
                new_indexes[i]=indexes[i] + m_start[i];
            return src.at(new_indexes);
        }
        const R& at(std::vector<std::size_t> indexes) const override
        {
            std::vector<std::size_t> new_indexes(src.rank());
            for(int i=0;i<src.rank();i++)
                new_indexes[i]=indexes[i] + m_start[i];
            return src.at(new_indexes);
        }
        std::vector<std::size_t> shape() const override
        {
            std::vector<std::size_t> new_shape(src.rank());
            for(int i=0;i<src.rank();i++)
                new_shape[i]=m_end[i]-m_start[i];
            return new_shape;
        }
        tensor_subview<R,dynamic_extent> slice(std::vector<std::size_t> start,std::vector<std::size_t> end) override
        {
            std::vector<std::size_t> new_start(src.rank()),new_end(src.rank());
            for(int i=0;i<src.rank();i++)
            {
                new_start[i]=this->m_start[i]+start[i];
                new_end[i]=this->m_start[i]+end[i];
            }
            return tensor_subview<R,dynamic_extent>(src,new_start,new_end);
        }

        tensor_subview<R,dynamic_extent> slice(std::vector<std::size_t> start,std::vector<std::size_t> end,std::vector<std::size_t> step) override
        {
            auto Rank=src.rank();
            std::vector<std::size_t> new_start(Rank),new_end(Rank),new_step(Rank);
            for(int i=0;i<Rank;i++)
            {
                new_start[i]=this->m_start[i]+start[i]*m_step[i];
                new_end[i]=this->m_start[i]+end[i]*m_step[i];
                new_step[i]=this->m_step[i]*step[i];
            }
            return tensor_subview<R,dynamic_extent>(src,new_start,new_end,new_step);
        }

    };

    template<typename R,std::size_t Rank>
    struct to_dynamic_view_t : public tensor_view<R,dynamic_extent>
    {
        tensor_view<R,Rank> &src;
        to_dynamic_view_t(tensor_view<R,Rank> &src):src(src){}
        R& at(std::vector<std::size_t> indexes) override
        {
            std::array<std::size_t,Rank> new_indexes;
            for(int i=0;i<Rank;i++)
                new_indexes[i]=indexes[i];
            return src.at(new_indexes);
        }
        const R& at(std::vector<std::size_t> indexes) const override
        {
            std::array<std::size_t,Rank> new_indexes;
            for(int i=0;i<Rank;i++)
                new_indexes[i]=indexes[i];
            return src.at(new_indexes);
        }
        std::vector<std::size_t> shape() const override
        {
            return std::vector<std::size_t>(src.shape().begin(),src.shape().end());
        }
        tensor_subview<R,dynamic_extent> slice(std::vector<std::size_t> start,std::vector<std::size_t> end) override
        {
            std::vector<std::size_t> new_start(Rank),new_end(Rank);
            for(int i=0;i<Rank;i++)
            {
                new_start[i]=start[i];
                new_end[i]=end[i];
            }
            return tensor_subview<R,dynamic_extent>(*this,new_start,new_end);
        }

        tensor_subview<R,dynamic_extent> slice(std::vector<std::size_t> start,std::vector<std::size_t> end, std::vector<std::size_t> step) override
        {
            std::vector<std::size_t> new_start(Rank),new_end(Rank),new_step(Rank);
            for(int i=0;i<Rank;i++)
            {
                new_start[i]=start[i];
                new_end[i]=end[i];
                new_step[i]=step[i];
            }
            return tensor_subview<R,dynamic_extent>(*this,new_start,new_end,new_step);
        }
    };

    template<typename R>
    struct to_dynamic_view_t<R,dynamic_extent>: public tensor_view<R,dynamic_extent>{};

    template<typename R,std::size_t Rank>
    struct to_static_view_t : public tensor_view<R,Rank>
    {
        tensor_view<R,dynamic_extent> &src;
        std::array<std::size_t,Rank> _shape;
        to_static_view_t(tensor_view<R,dynamic_extent> &src,std::array<std::size_t,Rank> _shape):src(src),_shape(_shape){}
        to_static_view_t(tensor_view<R,dynamic_extent> &src):src(src)
        {
            auto s=src.shape();
            for(int i=0;i<std::min(Rank,src.rank());i++)
                _shape[i]=s[i];
        }
        R& at(std::array<std::size_t,Rank> indexes) override
        {
            std::vector<std::size_t> new_indexes(Rank);
            for(int i=0;i<Rank;i++)
                new_indexes[i]=indexes[i];
            return src.at(new_indexes);
        }
        const R& at(std::array<std::size_t,Rank> indexes) const override
        {
            std::vector<std::size_t> new_indexes(Rank);
            for(int i=0;i<Rank;i++)
                new_indexes[i]=indexes[i];
            return src.at(new_indexes);
        }
        std::array<std::size_t,Rank> shape() const override
        {
            return _shape;
        }
    };

    template<typename R>
    tensor_subview<R,dynamic_extent> tensor_view<R,dynamic_extent>::slice(std::vector<std::size_t> start,std::vector<std::size_t> end)
    {
        return tensor_subview<R,dynamic_extent>(*this,start,end);
    }

    template<typename R>
    tensor_subview<R,dynamic_extent> tensor_view<R,dynamic_extent>::slice(std::vector<std::size_t> start,std::vector<std::size_t> end,std::vector<std::size_t> step)
    {
        return tensor_subview<R,dynamic_extent>(*this,start,end,step);
    }

    template<typename R,std::size_t Rank>
    to_dynamic_view_t<R,Rank> to_dynamic_view(tensor_view<R,Rank> &src)
    {
        return to_dynamic_view_t<R,Rank>(src);
    }

    template<typename R,std::size_t Rank>
    to_static_view_t<R,Rank> to_static_view(tensor_view<R,dynamic_extent> &src,std::array<std::size_t,Rank> _shape)
    {
        return to_static_view_t<R,Rank>(src,_shape);
    }

    template<std::size_t Rank,typename R>
    to_static_view_t<R,Rank> to_static_view(tensor_view<R,dynamic_extent> &src)
    {
        return to_static_view_t<R,Rank>(src);
    }
}

#endif //CPLIBRARY_VIEW_H
#include <complex>
#include <cstdint>
namespace cp::signals
{

    enum class FFTNormalization
    {
        None,
        Sqrt,
        Normalized,
        Auto
    };

    using cp::linalg::dynamic_extent;

    template<typename R>
    struct abstract_fft
    {
        virtual void transform(linalg::tensor_view<R,1> &&v, bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const
        {
            transform(v,inverse,normalization);
        }
        virtual void transform(linalg::tensor_view<R,1> &v, bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const = 0;
        virtual void transform(linalg::tensor_view<R,dynamic_extent> &&v, bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const
        {
            transform(v,inverse,normalization);
        }
        virtual void transform(linalg::tensor_view<R,dynamic_extent> &v, bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const
        {
            if(v.rank()>1)
                throw std::invalid_argument("rank must be one");
            auto x=to_static_view<1,R>(v);
            transform(x,inverse,normalization);
        }
        R& operator()(R &x, bool inverse=false, FFTNormalization normalization=FFTNormalization::None) const
        {
            transform(x,inverse,normalization);
            return x;
        }
        virtual ~abstract_fft()= default;
    };

    template<typename R>
    void normalize(linalg::tensor_view<std::complex<R>,1> &v,FFTNormalization normalized)
    {
        R r;
        switch (normalized)
        {
            case FFTNormalization::None:
                r=1;
                break;
            case FFTNormalization::Sqrt:
                r=std::sqrt(v.size());
                break;
            case FFTNormalization::Normalized:
                r=v.size();
                break;
            case FFTNormalization::Auto:
                throw std::invalid_argument("cannot normalize with auto");
        }
        if(normalized!=FFTNormalization::None) for (std::complex<R> & x : v)
            x /= r;
    }

    template<typename R>
    void normalize(linalg::tensor_view<std::complex<R>,1> &&v,FFTNormalization normalized)
    {
        normalize(v,normalized);
    }

    template<typename R>
    void inplace_fft2(cp::linalg::tensor_view<std::complex<R>,1> & a, bool inverse, FFTNormalization normalized = FFTNormalization::Sqrt)
    {
        if(normalized==FFTNormalization::Auto)
            normalized=FFTNormalization::Sqrt;
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++)
        {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            if (i < j)
                swap(a(i), a(j));
        }
        for (int len = 2; len <= n; len <<= 1)
        {
            R ang = 2 * std::numbers::pi / len * (inverse ? -1 : 1);
            std::complex<R> wlen = std::polar<R>(1.,ang);
            for (int i = 0; i < n; i += len)
            {
                std::complex<R> w(1);
                for (int j = 0; j < len / 2; j++)
                {
                    std::complex<R> u = a(i+j), v = a(i+j+len/2) * w;
                    a(i+j) = u + v;
                    a(i+j+len/2) = u - v;
                    w *= wlen;
                }
            }
        }
        normalize(a,normalized);
    }

    template<typename R>
    void inplace_fft2(cp::linalg::tensor_view<std::complex<R>,1> && a, bool inverse, FFTNormalization normalized = FFTNormalization::Sqrt)
    {
        inplace_fft2(a,inverse,normalized);
    }

    template<typename R>
    struct radix2_fft: public abstract_fft<R>
    {
        using abstract_fft<R>::transform;
        void transform(linalg::tensor_view<R,1> &v,bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const override
        {
            if(v.size()!= bit_ceil(v.size()))
                throw std::invalid_argument("size of vector must be a power of 2");
            inplace_fft2(v,inverse,normalization);
        }
    };

    template<typename R>
    struct mixed_radix_fft;


    template<std::floating_point Real>
    struct mixed_radix_fft<std::complex<Real>> : public cp::signals::abstract_fft<std::complex<Real>>, protected cp::default_factoriser_t
    {
        using R=std::complex<Real>;
        using cp::signals::abstract_fft<R>::transform;
        std::shared_ptr<cp::abstract_factoriser> F;
        mixed_radix_fft(std::shared_ptr<cp::abstract_factoriser> _F=default_factoriser):F(_F){}
        void transform_rec(cp::linalg::tensor_view<R,1> &v, bool inverse=false, cp::signals::FFTNormalization normalization = cp::signals::FFTNormalization::None) const
        {
            auto n=v.size();
            if(n==1)
                return;
            std::uint32_t p = F->smallest_divisor(n);
            auto q=n/p;
            std::vector<cp::linalg::tensor_subview<R,1>> V;
            for(unsigned i=0;i<p;i++)
                V.push_back(v.slice({i},{n},{p}));
            for(auto &v:V)
                transform_rec(v,inverse,normalization);
            R w=std::polar(1.0,2*std::numbers::pi/n);
            R z=std::polar(1.0,2*std::numbers::pi/p);
            if(inverse)
            {
                w = std::conj(w);
                z = std::conj(z);
            }
            R t=1;
            std::vector<R> result(n);
            for(int i=0;i<p;i++,t*=z)
            {
                R h1=1,h2=1;
                for (int j = 0; j < p; j++,h1*=t,h2*=w)
                {
                    R h3=1;
                    for (int k = 0; k < q; k++,h3*=h2)
                        result[i*q+k] += h1 * h3 * V[j](k);
                }
            }
            for(int i=0;i<n;i++)
                v(i)=result[i];
        }

        void transform(cp::linalg::tensor_view<R,1> &v, bool inverse=false, cp::signals::FFTNormalization normalization = cp::signals::FFTNormalization::None) const override
        {
            transform_rec(v,inverse,normalization);
            normalize(v,normalization);
        }
    };

    template<typename R>
    R mod_operator(R x, R y)
    {
        return (x%y+y)%y;
    }

    //Bluestein's algorithm
    template<typename R>
    std::vector<std::complex<R>> general_fft(const cp::linalg::tensor_view<std::complex<R>,1> &a, bool inverse, FFTNormalization normalized = FFTNormalization::Sqrt)
    {
        using cp::linalg::vector_view;
        unsigned int n = a.size();
        if(bit_ceil(n)==n)
        {
            std::vector<std::complex<R>> b(n);
            for(int i=0;i<n;i++)
                b[i]=a(i);
            inplace_fft2(vector_view(b),inverse);
            return b;
        }
        //m>=2*n
        auto m=bit_ceil(2*n-1);
        std::vector<std::complex<R>> w(m);
        R ang = std::numbers::pi  / n * (inverse ? 1 : -1);
        w[1]=std::polar<R>(1.,ang);
        w[0]=1;
        for(int i=2;i<m;i++)
            w[i]=w[i-1]*w[1];
        std::vector<std::complex<R>> A(m),B(m),W(m);
        for(size_t i=0;i<n;i++)
        {
            auto r=mod_operator<std::int64_t>(i*i,2*n);
            W[i]=w[r];
            A[i]=a(i)*w[2*n-r];
            B[i]=w[r];
        }
        for(size_t i=1;i<n;i++)
            B[m-i]=B[i];
        inplace_fft2(vector_view(A),false,FFTNormalization::None);
        inplace_fft2(vector_view(B),false,FFTNormalization::None);
        for(size_t i=0;i<m;i++)
            A[i]*=B[i];
        inplace_fft2(vector_view(A),true,FFTNormalization::Normalized);
        for(size_t i=0;i<n;i++)
            A[i]*=std::conj(W[i]);
        A.resize(n);
        normalize(vector_view(A),normalized);
        return A;
    }

    template<typename R>
    struct bluestein_fft : public abstract_fft<R>
    {
        using abstract_fft<R>::transform;
        void transform(linalg::tensor_view<R,1> &v,bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const override
        {
            auto b=general_fft(v,inverse,normalization);
            for(int i=0;i<v.size();i++)
                v(i)=b[i];
        }
    };

    template<typename R>
    struct chirpz_transform
    {
        radix2_fft<R> fft;
        R z;
        template<typename ...Args>
        chirpz_transform(R _z,Args&&... args):z(_z),fft(std::forward<Args>(args)...)
        {
        }

        std::vector<R> transform(const cp::linalg::tensor_view<R,1> &a)
        {
            using linalg::vector_view;
            auto n=a.size();
            auto m=bit_ceil(2*n-1);
            std::vector<R> A(m),B(m),W(m);
            for(size_t i=0;i<n;i++)
            {
                W[i]=pow(z,i*i);
                A[i]=a(i)*W[i];
            }
            for(size_t i=0;i<n;i++)
                B[i]=R(1)/W[i];
            for(size_t i=1;i<n;i++)
                B[m-i]=B[i];
            fft.transform(vector_view(A),false,FFTNormalization::None);
            fft.transform(vector_view(B),false,FFTNormalization::None);
            for(size_t i=0;i<m;i++)
                A[i]*=B[i];
            fft.transform(vector_view(A),true,FFTNormalization::Normalized);
            for(size_t i=0;i<n;i++)
                A[i]/=W[i];
            A.resize(n);
            return A;
        }

        std::vector<R> transform(const cp::linalg::tensor_view<R,1> &&a)
        {
            return transform(a);
        }

    };

}

#endif //CPLIBRARY_FFT_H
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_DYN_SEGMENT_TREE_H
#define CPLIBRARY_DYN_SEGMENT_TREE_H
#include <vector>
#include <memory>

namespace cp::data_structures::dynamic
{
    template<typename R>
    struct segment_tree
    {
        std::vector<std::vector<R>> S;
        std::vector<R> A;
        int n,h;
        binary_operation_ptr<R> F;
        segment_tree(const std::vector<R> &_A, std::shared_ptr<binary_operation<R>> _F):A(_A),F(_F)
        {
            n=bit_ceil(A.size());
            A.resize(n,F.neutral_element());
            int m=n;
            h=0;
            while(m)
            {
                m/=2;
                h++;
            }
            S.resize(h);
            for(int i=0;i<h;i++)
                S[i].resize(1<<i);
            build();
        }

        void update(int i,R u)
        {
            A[i]=u;
            S[h-1][i]=u;
            int m=h-2;
            i/=2;
            while(m>=0)
            {
                S[m][i]=F(S[m+1][2*i],S[m+1][2*i+1]);
                m--;
                i/=2;
            }
        }

        R query(int l,int r)
        {
            return query(std::max(l,0),std::min(r,n),0,n,0);
        }
    private:
        void build()
        {
            for(int i=0;i<n;i++)
                S.back()[i]=A[i];
            for(int i=h-2;i>=0;i--) for(int k=0;k<(1<<i);k++)
                    S[i][k]=F(S[i+1][2*k],S[i+1][2*k+1]);
        }
        R query(int l,int r,int a,int b,int depth)
        {
            if(l>=r)
                return F.neutral_element();
            if(l==a && r==b)
                return S[depth][l>>(h-1-depth)];
            int mid=(a+b)/2;
            if(mid>r)
                return query(l,r,a,mid,depth+1);
            else if(mid<l)
                return query(l,r,mid,b,depth+1);
            else
                return F(query(l,mid,a,mid,depth+1),query(mid,r,mid,b,depth+1));
        }
    };
}

#endif //CPLIBRARY_SEGMENT_TREE_H

namespace cp
{

    /**
 * @brief Karatsuba multiplication
* @details Applies Karatsuba multiplication between two polynomials
* @Requirements
* None
*/

    template<typename R>
    polynomial<R> karatsuba_multiplication(const polynomial<R> &p,const polynomial<R> &q)
    {
        constexpr int L=64;
        if(std::min(p.degree(),q.degree())<=L)
            return p*q;
        polynomial<R> a1,b1,a2,b2;
        int n=p.degree(),m=q.degree(),r=std::max(n,m)+1;
        std::vector<R> &u1=a1.data(),&u2=a2.data(),
                &v1=b1.data(),&v2=b2.data();
        u1.resize(std::min(n+1,r/2));
        u2.resize(std::min(m+1,r/2));
        v1.resize(std::max(n+1-r/2,0));
        v2.resize(std::max(m+1-r/2,0));
        for(int i=0;i<u1.size();i++)
            u1[i]=p[i];
        for(int i=0;i<u2.size();i++)
            u2[i]=q[i];
        for(int i=0;i<v1.size();i++)
            v1[i]=p[i+r/2];
        for(int i=0;i<v2.size();i++)
            v2[i]=q[i+r/2];
        polynomial<R> r1= karatsuba_multiplication(a1,a2),
                r3= karatsuba_multiplication(b1,b2),
                t=karatsuba_multiplication(a1+b1,a2+b2),
                r2=t-r1-r3;
        polynomial<R> h;
        int s=r-r%2;
        auto &c=h.data();
        c.resize(n+m+1);
        for(int i=0;i<=r1.degree();i++)
            c[i]+=r1[i];
        for(int i=0;i<=r2.degree();i++)
            c[i+r/2]+=r2[i];
        for(int i=0;i<=r3.degree();i++)
            c[i+s]+=r3[i];
        return h;
    }

    template<typename R>
    sparse_polynomial<R> karatsuba_multiplication(const sparse_polynomial<R>& p, const sparse_polynomial<R>& q)
    {
        constexpr int recursion_limit = 30;
        if (std::min(p.size(), q.size()) <= recursion_limit)
            return p * q;
        sparse_polynomial<R> a1, b1, a2, b2;
        int n = p.degree(), m = q.degree(), r = std::max(n, m) + 1;
        const auto &mapper1 = static_cast<const std::map<int, R>&>(p),&mapper2=static_cast<const std::map<int, R>&>(q);
        auto it1 = mapper1.begin(),it2=mapper2.begin();
        for (; it1!=mapper1.end()  && it1->first<  r / 2; ++it1)
            a1[it1->first] = it1->second;
        for (; it2 != mapper2.end() && it2->first < r / 2; ++it2)
            a2[it2->first] = it2->second;
        for (; it1 != mapper1.end(); ++it1)
            b1[it1->first-r/2] = it1->second;
        for (; it2 != mapper2.end(); ++it2)
            b2[it2->first-r/2] = it2->second;
        sparse_polynomial<R> r1 = karatsuba_multiplication(a1, a2),
                r3 = karatsuba_multiplication(b1, b2),
                t = karatsuba_multiplication(a1 + b1, a2 + b2),
                r2 = t - r1 - r3;
        sparse_polynomial<R> h;
        int s = r - r % 2;
        auto& c = static_cast<std::map<int,R>&>(h);
        c = r1;
        for (auto [k, w] : static_cast<std::map<int, R>&>(r2))
            c[k + r / 2] += w;
        for (auto [k, w] : static_cast<std::map<int, R>&>(r3))
            c[k + s] += w;
        return h;
    }

    template<typename R>
    struct polynomial_operation;
    template<typename R>
    struct polynomial_operation<polynomial<R>> : public binary_operation<polynomial<R>>
    {
        std::shared_ptr<binary_operation<std::vector<R>>> m_op;
        std::shared_ptr<invertible_operation<R>> m_inv;
        polynomial_operation(std::shared_ptr<binary_operation<std::vector<R>>> _op,std::shared_ptr<invertible_operation<R>> _inv):m_op(_op),m_inv(_inv){}
        polynomial_operation(std::shared_ptr<binary_operation<std::vector<R>>> _op):m_op(_op),m_inv(nullptr){}
        polynomial<R> reduce(const polynomial<R>&a,const polynomial<R>&b) const override
        {
            return m_op->reduce(a.data(),b.data());
        }

        R inv(const R&a) const
        {
            return m_inv->inv(a);
        }

        polynomial<R> neutral_element() const override
        {
            return polynomial<R>(m_op->neutral_element());
        }
        std::shared_ptr<binary_operation<std::vector<R>>> underlying_operator() const
        {
            return m_op;
        }
        std::shared_ptr<invertible_operation<R>> scalar_inverter() const
        {
            return m_inv;
        }
    };

    template<typename R>
    struct karatsuba_multiplies_t;
    template<typename R>
    struct karatsuba_multiplies_t<polynomial<R>> : public binary_operation<polynomial<R>>
    {
        polynomial<R> reduce(const polynomial<R>&a,const polynomial<R>&b) const override
        {
            return karatsuba_multiplication(a,b);
        }
    };

    template<typename R>
    struct karatsuba_multiplies_t<std::vector<R>> : public binary_operation<std::vector<R>>
    {
        std::vector<R> reduce(const std::vector<R>&a,const std::vector<R>&b) const override
        {
            return karatsuba_multiplication(polynomial(a),polynomial(b)).data();
        }
    };

    template<typename R>
    struct fast_multiplies_t;

    template<typename R>
    struct fast_multiplies_t<std::vector<R>> : public binary_operation<std::vector<R>>
    {
        signals::radix2_fft<R> fft;
        template<typename ...Args>
        fast_multiplies_t(Args&&...args):fft(std::forward<Args>(args)...){}
        std::vector<R> reduce(const std::vector<R>&a,const std::vector<R>&b) const override
        {
            if(a.size()==0 || b.size()==0)
                return {};
            std::vector<R> A(a),B(b);
            auto r=std::bit_ceil<unsigned>(A.size()+B.size()-1);
            A.resize(r);
            B.resize(r);
            linalg::vector_view U(A),V(B);
            fft.transform(U,false,signals::FFTNormalization::None);
            fft.transform(V,false,signals::FFTNormalization::None);
            for(int i=0;i<r;i++)
                A[i]*=B[i];
            fft.transform(U,true,signals::FFTNormalization::Normalized);
            A.resize(a.size()+b.size()-1);
            return A;
        }
        inline static std::vector<R> neutral=std::vector<R>{1};
        std::vector<R> neutral_element() const override
        {
            return neutral;
        }
    };

    template<std::floating_point R>
    struct fast_multiplies_t<std::vector<R>> : public binary_operation<std::vector<R>>
    {
        signals::radix2_fft<std::complex<R>> fft;
        std::vector<R> reduce(const std::vector<R>&a,const std::vector<R>&b) const override
        {
            if(a.size()==0 || b.size()==0)
                return {};
            auto r=std::bit_ceil<unsigned>(a.size()+b.size()-1);
            std::vector<std::complex<R>> A(r);
            for(int i=0;i<a.size();i++)
            {
                A[i].real(a[i]);
                A[i].imag(b[i]);
            }
            linalg::vector_view U(A);
            fft.transform(U,false,signals::FFTNormalization::None);
            std::vector<std::complex<R>> C(r);
            for(unsigned i=0;i<r;i++)
            {
                auto j=(r-i)&(r-1);
                C[i]=A[i]*A[i]-std::conj(A[j]*A[j]);
                C[i]*=std::complex<R>(0,-0.25);
            }
            linalg::vector_view V(C);
            fft.transform(V,true,signals::FFTNormalization::Normalized);
            std::vector<R> C_real(r);
            for(int i=0;i<r;i++)
                C_real[i]=C[i].real();
            C_real.resize(a.size()+b.size()-1);
            return C_real;
        }
        inline static std::vector<R> neutral=std::vector<R>{1};
        std::vector<R> neutral_element() const override
        {
            return neutral;
        }
    };

    template<typename R>
    struct fast_multiplies_t<polynomial<R>> : public binary_operation<polynomial<R>>
    {
        fast_multiplies_t<std::vector<R>> fft;
        polynomial<R> reduce(const polynomial<R>&a,const polynomial<R>&b) const override
        {
            return polynomial<R>(fft.reduce(a.data(),b.data()));
        }
        inline static polynomial<R> neutral=polynomial<R>{1};
    };

    template<typename R>
    std::vector<R> fast_multiplication(const std::vector<R> &A,const std::vector<R> &B)
    {
        static fast_multiplies_t<std::vector<R>> multiplies;
        return multiplies.reduce(A,B);
    }

    template<typename R>
    std::vector<R> fast_multiplication(const std::vector<R> &A,const std::vector<R> &B,std::shared_ptr<binary_operation<std::vector<R>>> multiplies)
    {
        return multiplies->reduce(A,B);
    }

    template<typename R>
    polynomial<R> fast_multiplication(const polynomial<R> &A,const polynomial<R> &B)
    {
        static fast_multiplies_t<polynomial<R>> multiplies;
        return multiplies.reduce(A,B);
    }

    template<typename Real=real,cp::integer decompositions=2, cp::integer m>
    std::vector<cp::cyclic<m>> fast_modular_multiplication_real(const std::vector<cp::cyclic<m>> &a,const std::vector<cp::cyclic<m>> &b)
    {
        if(a.size()==0)
            return {};
        using namespace cp;
        std::array<std::vector<Real>,decompositions> A,B;
        auto m_=a.front().modulus();
        integer block=std::ceil(std::pow(m_,1./decompositions));
        for(int i=0;i<a.size();i++)
        {
            auto z=static_cast<integer>(a[i]);
            for(int j=0;j<decompositions;j++)
            {
                auto [q,r]=std::div(z,block);
                A[j].push_back(r);
                z=q;
            }
        }
        for(int i=0;i<b.size();i++)
        {
            auto z=static_cast<integer>(b[i]);
            for(int j=0;j<decompositions;j++)
            {
                auto [q,r]=std::div(z,block);
                B[j].push_back(r);
                z=q;
            }
        }
        std::array<std::array<std::vector<Real>,decompositions>,decompositions> C;
        for(int i=0;i<decompositions;i++) for(int j=0;j<decompositions;j++)
                C[i][j]= fast_multiplication(A[i],B[j]);
        std::vector<cyclic<m>> R(a.size()+b.size()-1);
        for(int i=0;i<R.size();i++)
        {
            integer x=0;
            integer t1=1;
            for(int j=0;j<decompositions;j++)
            {
                integer t2=t1;
                for(int k=0;k<decompositions;k++)
                {
                    x+=std::llround(C[j][k][i])%m_*t2%m_;
                    x%=m_;
                    t2*=block;
                    t2%=m_;
                }
                t1*=block;
            }
            R[i]=x;
        }
        return R;
    }



    template<typename R>
    std::vector<R> formal_inv_2(const std::vector<R> &A,int m)
    {
        if(m==1)
            return {R(1)/A.front()};
        auto B=A;
        for(int i=1;i<A.size();i+=2)
            B[i]=-B[i];
        auto C= fast_multiplication(A,B);
        std::vector<R> T;
        T.resize(m/2);
        for(int i=0;i<T.size() && 2*i < C.size();i++)
            T[i]=C[2*i];
        auto S=formal_inv_2(T,m/2);
        std::vector<R> Q;
        Q.resize(m);
        for(int i=0;i<m/2;i++)
            Q[2*i]=S[i];
        return fast_multiplication(B, Q);
    }

    template<typename R>
    std::vector<R> formal_inv_2(const std::vector<R> &A,int m,std::shared_ptr<binary_operation<std::vector<R>>> multiplies,std::shared_ptr<invertible_operation<R>> inv)
    {
        if(m==1)
            return {inv->inv(A.front())};
        auto B=A;
        for(int i=1;i<A.size();i+=2)
            B[i]=-B[i];
        auto C= multiplies->reduce(A,B);
        std::vector<R> T;
        T.resize(m/2);
        for(int i=0;i<T.size() && 2*i < C.size();i++)
            T[i]=C[2*i];
        auto S=formal_inv_2(T,m/2,multiplies,inv);
        std::vector<R> Q;
        Q.resize(m);
        for(int i=0;i<m/2;i++)
            Q[2*i]=S[i];
        return multiplies->reduce(B, Q);
    }

    template<typename R>
    polynomial<R> formal_inv_2(const polynomial<R> &A,int m)
    {
        return formal_inv_2(A.data(),m);
    }

    template<typename R>
    polynomial<R> formal_inv_2(const polynomial<R> &A,int m,std::shared_ptr<polynomial_operation<polynomial<R>>> multiplies)
    {
        return formal_inv_2(A.data(),m,multiplies->underlying_operator(),multiplies->scalar_inverter());
    }

    template<typename R>
    std::vector<R> formal_inv(const std::vector<R> &A,int m)
    {
        auto C=formal_inv_2(A,std::bit_ceil<unsigned>(m));
        C.resize(m);
        return C;
    }

    template<typename R>
    std::vector<R> formal_inv(const std::vector<R> &A,int m,std::shared_ptr<binary_operation<std::vector<R>>> multiplies,std::shared_ptr<invertible_operation<R>> inv)
    {
        auto C=formal_inv_2(A,std::bit_ceil<unsigned>(m),multiplies,inv);
        C.resize(m);
        return C;
    }

    template<typename R>
    polynomial<R> formal_inv(const polynomial<R> &A,int m)
    {
        auto C=formal_inv_2(A,std::bit_ceil<unsigned>(m));
        C.data().resize(m);
        return C;
    }

    template<typename R>
    polynomial<R> formal_inv(const polynomial<R> &A,int m,std::shared_ptr<polynomial_operation<polynomial<R>>> multiplies)
    {
        auto C=formal_inv_2(A,std::bit_ceil<unsigned>(m),multiplies, multiplies->scalar_inverter());
        C.data().resize(m);
        return C;
    }

    template<typename R>
    std::vector<R> fast_division(std::vector<R> A,std::vector<R> Q)
    {
        if(A.size()<Q.size())
            return {};
        int m=A.size()-Q.size()+1;
        std::reverse(A.begin(),A.end());
        std::reverse(Q.begin(),Q.end());
        auto P= fast_multiplication(A, formal_inv(Q,m));
        P.resize(m);
        std::reverse(P.begin(),P.end());
        return P;
    }

    template<typename R>
    std::vector<R> fast_division(std::vector<R> A,std::vector<R> Q,std::shared_ptr<binary_operation<std::vector<R>>> multiplies,std::shared_ptr<invertible_operation<R>> inv)
    {
        if(A.size()<Q.size())
            return {};
        int m=A.size()-Q.size()+1;
        std::reverse(A.begin(),A.end());
        std::reverse(Q.begin(),Q.end());
        auto P= multiplies->reduce(A, formal_inv(Q,m,multiplies,inv));
        P.resize(m);
        std::reverse(P.begin(),P.end());
        return P;
    }

    template<typename R>
    polynomial<R> fast_division(const polynomial<R> &A,const polynomial<R> &B)
    {
        return fast_division(A.data(),B.data());
    }

    template<typename R>
    polynomial<R> fast_division(const polynomial<R> &A,const polynomial<R> &B, std::shared_ptr<polynomial_operation<polynomial<R>>> multiplies)
    {
        return fast_division(A.data(),B.data(), multiplies->underlying_operator(),multiplies->scalar_inverter());
    }


    template<typename R>
    polynomial<R> fast_mod(const polynomial<R>&A,const polynomial<R>& B)
    {
        auto P= fast_division(A,B);
        auto Z= A - fast_multiplication(B,P);
        Z.data().resize(B.degree());
        return Z;
    }

    template<typename R>
    polynomial<R> fast_mod(const polynomial<R>&A, const polynomial<R>& B, std::shared_ptr<polynomial_operation<polynomial<R>>> multiplies)
    {
        auto P= fast_division(A,B,multiplies);
        auto Z= A - multiplies->reduce(B,P);
        Z.data().resize(B.degree());
        return Z;
    }

    template<typename R>
    std::pair<polynomial<R>,polynomial<R>> fast_euclidean_division(const polynomial<R> &A,const polynomial<R>& B)
    {
        auto P= fast_division(A,B);
        auto Q=A- fast_multiplication(B,P);
        Q.data().resize(B.degree());
        return std::make_pair(P,Q);
    }

    template<typename R>
    polynomial<R> fast_gcd(const polynomial<R> &A,const polynomial<R> &B)
    {
        if(B==R{})
            return A;
        return fast_gcd(B,fast_mod(A,B).reduce());
    }

    template<typename R>
    polynomial<R> fast_polynomial_expansion(const std::vector<R> &X)
    {
        int n=X.size();
        std::vector<polynomial<R>> P(X.size());
        for(int i=0;i<n;i++)
            P[i]=polynomial<R>({-X[i],1});
        data_structures::fixed::segment_tree<fast_multiplies_t<polynomial<R>>> S(P);
        return S.S[0][0];
    }

    template<typename R>
    polynomial<R> fast_polynomial_expansion(const std::vector<R> &X,std::shared_ptr<polynomial_operation<polynomial<R>>> multiplies)
    {
        int n=X.size();
        std::vector<polynomial<R>> P(X.size());
        for(int i=0;i<n;i++)
            P[i]=polynomial<R>({-X[i],1});
        data_structures::dynamic::segment_tree S(P,multiplies);
        return S.S[0][0];
    }


    template<typename R>
    std::vector<R> fast_multi_evaluation(const polynomial<R> &A,const std::vector<R> &X)
    {
        int n=X.size();
        std::vector<polynomial<R>> P(X.size());
        for(int i=0;i<n;i++)
            P[i]=polynomial<R>({-X[i],1});
        data_structures::fixed::segment_tree<fast_multiplies_t<polynomial<R>>> S(P);
        std::vector<polynomial<R>> Z(1<<(S.h-1));
        Z[0]=fast_mod(A,S.S[0][0]);
        for(int i=1;i<S.h;i++)
            for(int j=(1<<i)-1;j>=0;j--)
                Z[j]=fast_mod(Z[j>>1],S.S[i][j]);
        std::vector<R> Y;
        Y.reserve(n);
        for(int i=0;i<n;i++)
            Y.push_back(Z[i](R{}));
        return Y;
    }

    template<typename R>
    std::vector<R> fast_multi_evaluation(const polynomial<R> &A,const std::vector<R> &X,std::shared_ptr<polynomial_operation<polynomial<R>>> multiplies)
    {
        int n=X.size();
        std::vector<polynomial<R>> P(X.size());
        for(int i=0;i<n;i++)
            P[i]=polynomial<R>({-X[i],1});
        data_structures::dynamic::segment_tree S(P,std::dynamic_pointer_cast<binary_operation<polynomial<R>>>(multiplies));
        std::vector<polynomial<R>> Z(1<<(S.h-1));
        Z[0]=fast_mod(A,S.S[0][0],multiplies);
        for(int i=1;i<S.h;i++)
            for(int j=(1<<i)-1;j>=0;j--)
                Z[j]=fast_mod(Z[j>>1],S.S[i][j],multiplies);
        std::vector<R> Y;
        Y.reserve(n);
        for(int i=0;i<n;i++)
            Y.push_back(Z[i](R{}));
        return Y;
    }

    template<typename R>
    polynomial<R> fast_interpolation(const std::vector<R> &X,const std::vector<R> &Y)
    {
        int n=X.size();
        std::vector<polynomial<R>> P(X.size());
        for(int i=0;i<n;i++)
            P[i]=polynomial<R>({-X[i],1});
        data_structures::fixed::segment_tree<fast_multiplies_t<polynomial<R>>> S(P);
        std::vector<std::vector<polynomial<R>>> Z(S.h);
        for(int i=0;i<S.h;i++)
            Z[i].resize(1<<i);
        Z[0][0]=polynomial<R>(Y);
        for(int i=1;i<S.h;i++)
            for(int j=0;j<Z[i].size();j++)
                Z[i][j]=fast_mod(Z[i-1][j>>1],S.S[i][j]);
        return Z.back()[0];
    }

    template<typename R>
    polynomial<R> fast_interpolation(const std::vector<R> &X,const std::vector<R> &Y,std::shared_ptr<polynomial_operation<polynomial<R>>> multiplies)
    {
        int n=X.size();
        std::vector<polynomial<R>> P(X.size());
        for(int i=0;i<n;i++)
            P[i]=polynomial<R>({-X[i],1});
        data_structures::dynamic::segment_tree S(P,std::dynamic_pointer_cast<binary_operation<polynomial<R>>>(multiplies));
        auto P_=S.S[0][0].derivative();
        auto Y_=fast_multi_evaluation(P_,X,multiplies);
        std::vector<polynomial<R>> Z(1<<(S.h-1));
        for(int i=0;i<Y_.size();i++)
            Z[i]=multiplies->reduce(polynomial<R>(Y[i]),polynomial<R>(multiplies->inv(Y_[i])));
        for(int i=S.h-2;i>=0;i--) for(int j=0;j<(1<<i);j++)
            Z[j]=multiplies->reduce(Z[2*j],S.S[i+1][2*j+1])+multiplies->reduce(Z[2*j+1],S.S[i+1][2*j]);
        return Z[0];
    }

    template<typename R,integer m>
    std::vector<cyclic<m>> fast_complex_modular_multiplication(const std::vector<cyclic<m>> &A,const std::vector<cyclic<m>> &B)
    {
        signals::radix2_fft<std::complex<R>> fft;
        if(A.empty())
            return {};
        auto n=std::bit_ceil(A.size()+B.size()-1);
        integer block=std::sqrt(A.front().modulus());
        std::vector<cyclic<m>> P(n),Q(n);
        for(auto a:A)
        {
            auto [x,y]=std::div(static_cast<integer>(a),block);
            P[x]=std::complex<R>(x,y);
        }
        for(auto b:B)
        {
            auto [x,y]=std::div(static_cast<integer>(b),block);
            Q[x]=std::complex<R>(x,y);
        }
        linalg::vector_view U(P),V(Q);
        fft.transform(U,false,signals::FFTNormalization::None);
        fft.transform(V,false,signals::FFTNormalization::None);

    }
}
#endif //ACPC_PREPARATION_FFT_H

namespace cp
{
    // Formal Newton method. Requires z0 to be a solution of P(z)=0 in R[x]/x
    template<typename R,typename X,typename Functional,typename Derivative>
    polynomial<R> formal_newton_method_2(polynomial<R> P, const X &z0, int n,const Functional& F, const Derivative & dF)
    {
        if(n==1)
            return z0;
        else
        {
            auto z=formal_newton_method_2(P,z0,n/2,F,dF);
            auto w=F(z,n),dw_inv=formal_inv(dF(z,n),n);
            auto dz=fast_multiplication(w, dw_inv);
            dz.data().resize(n);
            return z - dz;
        }
    }

    template<typename R,typename X,typename Functional,typename Derivative>
    polynomial<R> formal_newton_method(polynomial<R> P, const X &z0, int n,const Functional& F, const Derivative & dF)
    {
        auto Q= formal_newton_method_2(P,z0,std::bit_ceil<unsigned>(n),F,dF);
        Q.data().resize(n);
        return Q;
    }

    // Formal logarithm
    template<typename R>
    polynomial<R> formal_logarithm(const polynomial<R> &P,int n)
    {
        auto Q= fast_multiplication(P.derivative(),formal_inv(P,n));
        for(int i=Q.data().size();i>0;i--)
        {
            Q[i] = Q[i - 1];
            Q[i] /= i;
        }
        if(!Q.data().empty())
            Q[0]=0;
        Q.data().resize(n);
        return Q;
    }

    template<typename R>
    std::vector<R> formal_logarithm(const std::vector<R> &P,int n)
    {
        auto dP=P;
        dP.erase(dP.begin());
        for(int i=0;i<dP.size();i++)
            dP[i]*=i+1;
        auto Q= fast_multiplication(dP,formal_inv(P,n));
        Q.resize(n);
        for(int i=Q.size();i>0;i--)
        {
            Q[i] = Q[i - 1];
            Q[i] /= i;
        }
        if(!Q.empty())
            Q[0]=0;
        return Q;
    }

    template<typename R>
    polynomial<R> formal_exp_2(const polynomial<R> &P,int n)
    {
        if(n==1)
            return 1;
        else
        {
            auto Q=formal_exp_2(P,n/2);
            Q=fast_multiplication(Q,P + R(1) - formal_logarithm(Q,n));
            Q.data().resize(n);
            return Q;
        }
    }

    template<typename R>
    polynomial<R> formal_exp(const polynomial<R> &P,int n)
    {
        auto Q= formal_exp_2(P,std::bit_ceil<unsigned>(n));
        Q.data().resize(n);
        return Q;
    }

}

#endif //CPLIBRARY_FORMAL_SERIES_H
//
// Created by ramizouari on 28/11/23.
//

#ifndef CPLIBRARY_NTT_H
#define CPLIBRARY_NTT_H

#include <memory>
#include <set>
#include <optional>

namespace cp::signals
{

    template<integer n>
    struct abstract_ntt : public abstract_fft<cyclic<n>>, protected default_factoriser_t
    {
        using R=cyclic<n>;
        using abstract_fft<R>::transform;
        abstract_ntt(std::shared_ptr<abstract_factoriser> _F=default_factoriser):F(_F){}
        mutable integer version=0;
        mutable std::array<std::unordered_map<integer,R>,2> cache;
        mutable std::optional<integer> phi;
        mutable R w1,w2;
        mutable std::shared_ptr<abstract_factoriser> F;

        void build() const
        {
            version=cyclic<n>::modulus();
            if(!F)
                F=default_factoriser;
            phi= carmichael_totient(cyclic<n>::modulus(),*F);
            w1= primitive_root_of_unity(cyclic<n>::modulus(),*F);
            w2=w1.pinv();
        }
        virtual R root_of_unity(integer size,integer m,bool inverse) const
        {
            if(version!=cyclic<n>::modulus())
                build();
            R w;
            if(cache[inverse].count(size))
                w=cache[inverse][size];
            else
            {
                w=inverse?w2:w1;
                auto [q,r]=std::div(*phi,size);
                if(r!=0)
                    throw std::invalid_argument("size must divide phi(m)");
                w=pow(w,q);
                cache[inverse][size]=w;
            }
            return w;
        }
        virtual ~abstract_ntt()= default;
    };

    template<>
    struct abstract_ntt<dynamic_modulus> : public abstract_fft<cyclic<dynamic_modulus>>, protected default_factoriser_t
    {
        using R=cyclic<dynamic_modulus>;
        using abstract_fft<R>::transform;
        abstract_ntt(std::shared_ptr<abstract_factoriser> _F=default_factoriser):F(_F){}
        mutable std::array<std::map<std::pair<integer,integer>,R>,2> cache;
        mutable std::map<integer,integer> phi;
        mutable std::map<integer,R> w1,w2;
        mutable std::set<integer> versions;
        mutable std::shared_ptr<abstract_factoriser> F;

        void build(integer m) const
        {
            versions.emplace(m);
            if(!F)
                F=default_factoriser;
            phi[m]= carmichael_totient(m,*F);
            w1[m]= primitive_root_of_unity(m,*F);
            w2[m]=w1[m].pinv();
        }
        virtual R root_of_unity(integer size,integer m,bool inverse) const
        {
            if(!versions.count(m))
                build(m);
            R w;
            if(cache[inverse].count({m,size}))
                w=cache[inverse][{m,size}];
            else
            {
                w=inverse?w2[m]:w1[m];
                auto [q,r]=std::div(phi[m],size);
                if(r!=0)
                    throw std::invalid_argument("size must divide phi(m)");
                w=pow(w,q);
                cache[inverse][{m,size}]=w;
            }
            return w;
        }
        virtual ~abstract_ntt()= default;
    };

    inline void cyclic_normalize(linalg::tensor_view<cyclic<dynamic_modulus>,1> &v,integer modulus,FFTNormalization normalized)
    {
        cyclic<dynamic_modulus> r(0,modulus);
        switch (normalized)
        {
            case FFTNormalization::None:
                r=1;
                break;
            case FFTNormalization::Sqrt:
                //r=cp::sqrt(cyclic<dynamic_modulus>(v.size(),modulus));
                throw std::invalid_argument("not implemented");
                break;
            case FFTNormalization::Normalized:
                r=v.size();
                break;
        }
        r=r.pinv();
        for(auto &x:v)
            x*=r;
    }

    template<integer m>
    void cyclic_normalize(linalg::tensor_view<cyclic<m>,1> &v,FFTNormalization normalized)
    {
        if constexpr (m==dynamic_modulus)
        {
            return cyclic_normalize(v,v(0).modulus(),normalized);
        }
        else
        {
            cyclic<m> r;
            switch (normalized)
            {
                case FFTNormalization::None:
                    r=1;
                    break;
                case FFTNormalization::Sqrt:
                    //r=cp::sqrt(cyclic<m>(v.size()));
                    throw std::invalid_argument("not implemented");
                    break;
                case FFTNormalization::Normalized:
                    r=v.size();
                    break;
            }
            r=r.pinv();
            for(auto &x:v)
                x*=r;
        }
    }

    template<integer m>
    void cyclic_normalize(linalg::tensor_view<cyclic<m>,1> &&v,FFTNormalization normalized)
    {
        cyclic_normalize(v,normalized);
    }



    template<integer m>
    void inplace_ntt2(cp::linalg::tensor_view<cyclic<m>,1> & a, cyclic<m> theta, FFTNormalization normalized = FFTNormalization::Sqrt)
    {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++)
        {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            if (i < j)
                std::swap(a(i), a(j));
        }
        std::vector<cyclic<m>> W;
        W.push_back(theta);
        for (int len = 2; len <= n; len <<= 1)
            W.push_back(W.back()*W.back());
        W.pop_back();
        for (int len = 2,r=W.size()-1; len <= n; len <<= 1,r--)
        {
            auto wlen = W[r];
            for (int i = 0; i < n; i += len)
            {
                cyclic<m> w(1);
                for (int j = 0; j < len / 2; j++)
                {
                    cyclic<m> u = a(i+j), v = a(i+j+len/2) * w;
                    a(i+j) = u + v;
                    a(i+j+len/2) = u - v;
                    w *= wlen;
                }
            }
        }
        cyclic_normalize(a,normalized);
    }

    void inplace_ntt2(cp::linalg::tensor_view<cyclic<dynamic_modulus>,1> & a, cyclic<dynamic_modulus> theta, FFTNormalization normalized = FFTNormalization::Sqrt)
    {
        using Cyclic=cyclic<dynamic_modulus>;
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++)
        {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            if (i < j)
                std::swap(a(i), a(j));
        }
        std::vector<Cyclic> W;
        W.push_back(theta);
        for (int len = 2; len <= n; len <<= 1)
            W.push_back(W.back()*W.back());
        W.pop_back();
        for (int len = 2,r=W.size()-1; len <= n; len <<= 1,r--)
        {
            auto wlen = W[r];
            for (int i = 0; i < n; i += len)
            {
                Cyclic w(1,theta.modulus());
                for (int j = 0; j < len / 2; j++)
                {
                    Cyclic u = a(i+j), v = a(i+j+len/2) * w;
                    a(i+j) = u + v;
                    a(i+j+len/2) = u - v;
                    w *= wlen;
                }
            }
        }
        cyclic_normalize(a,normalized);
    }

    template<typename Cyclic>
    void inplace_ntt2(cp::linalg::tensor_view<Cyclic,1> && a, Cyclic theta, FFTNormalization normalized = FFTNormalization::Sqrt)
    {
        inplace_ntt2(a,theta,normalized);
    }

    template<integer n>
    struct radix2_fft<cyclic<n>> : public abstract_ntt<n>
    {
        using R=cyclic<n>;
        using abstract_fft<cyclic<n>>::transform;
        using abstract_ntt<n>::root_of_unity;
        using abstract_ntt<n>::build;
        using default_factoriser_t::default_factoriser;
        radix2_fft(std::shared_ptr<abstract_factoriser> _F=default_factoriser):abstract_ntt<n>(_F)
        {
        }
        void transform(linalg::tensor_view<R,1> &v, bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const override
        {
            auto w= root_of_unity(v.size(),cyclic<n>::modulus(),inverse);
            if(normalization==FFTNormalization::Auto)
                normalization=inverse?FFTNormalization::Normalized:FFTNormalization::None;
            inplace_ntt2(v, w, normalization);
        }
    };

    template<>
    struct radix2_fft<cyclic<dynamic_modulus>> : public abstract_ntt<dynamic_modulus>
    {
        using R=cyclic<dynamic_modulus>;
        using abstract_fft<cyclic<dynamic_modulus>>::transform;
        using abstract_ntt<dynamic_modulus>::root_of_unity;
        using abstract_ntt<dynamic_modulus>::build;
        using default_factoriser_t::default_factoriser;
        radix2_fft(std::shared_ptr<abstract_factoriser> _F=default_factoriser):abstract_ntt<dynamic_modulus>(_F)
        {
        }
        void transform(linalg::tensor_view<R,1> &v, bool inverse=false, FFTNormalization normalization = FFTNormalization::None) const override
        {
            if(!std::has_single_bit(v.size()))
                throw std::invalid_argument("size must be a power of 2");
            auto w= root_of_unity(v.size(),v(0).modulus(),inverse);
            if(normalization==FFTNormalization::Auto)
                normalization=inverse?FFTNormalization::Normalized:FFTNormalization::None;
            inplace_ntt2(v, w, normalization);
        }
    };


    //Bluestein's algorithm
    template<integer q>
    std::vector<cyclic<q>> general_ntt(const abstract_ntt<q> &ntt,const cp::linalg::tensor_view<cyclic<q>,1> &a, bool inverse,
                                       FFTNormalization normalized = FFTNormalization::Sqrt)
    {
        using cp::linalg::vector_view;
        unsigned int n = a.size();
        using R=cyclic<q>;
        if(bit_ceil(n)==n)
        {
            std::vector<R> b(n);
            for(int i=0;i<n;i++)
                b[i]=a(i);
            ntt.transform(vector_view(b),inverse);
            cyclic_normalize(vector_view(b),normalized);
            return b;
        }
        //m>=2*n
        auto m=bit_ceil(2*n-1);
        std::vector<cyclic<q>> w(m);
        w[1]=ntt.root_of_unity(2*n,q,inverse);
        w[0]=1;
        for(int i=2;i<m;i++)
            w[i]=w[i-1]*w[1];
        std::vector<cyclic<q>> A(m),B(m),W1(m),W2(m);
        for(size_t i=0;i<n;i++)
        {
            auto r=mod_operator<std::int64_t>(i*i,2*n);
            W1[i]=w[r];
            W2[i]=w[2*n-r];
            A[i]=a(i)*w[2*n-r];
            B[i]=w[r];
        }
        for(size_t i=1;i<n;i++)
            B[m-i]=B[i];
        ntt.transform(vector_view(A),false,FFTNormalization::None);
        ntt.transform(vector_view(B),false,FFTNormalization::None);
        for(size_t i=0;i<m;i++)
            A[i]*=B[i];
        ntt.transform(vector_view(A),true,FFTNormalization::Normalized);
        for(size_t i=0;i<n;i++)
            A[i]*=W2[i];
        A.resize(n);
        cyclic_normalize(vector_view(A),normalized);
        return A;
    }

    template<integer n>
    struct bluestein_fft<cyclic<n>> : public abstract_fft<cyclic<n>>, private default_factoriser_t
    {
        using abstract_fft<cyclic<n>>::transform;
        using default_factoriser_t::default_factoriser;
        using R=cyclic<n>;
        std::unique_ptr<radix2_fft<R>> fft;
        bluestein_fft(std::shared_ptr<abstract_factoriser> _F=default_factoriser):fft(std::make_unique<radix2_fft<R>>(_F)){}
        void transform(linalg::tensor_view<R,1> &v,bool inverse, FFTNormalization normalization = FFTNormalization::None) const override
        {
            auto b=general_ntt(*fft,v,inverse,normalization);
            for(int i=0;i<v.size();i++)
                v(i)=b[i];
        }
    };

    template<integer m>
    struct mixed_radix_fft<cyclic<m>> : public abstract_ntt<m>
    {
        using R=cyclic<m>;
        using cp::signals::abstract_fft<R>::transform;
        using default_factoriser_t::default_factoriser;
        mixed_radix_fft(std::shared_ptr<cp::abstract_factoriser> _F=default_factoriser):abstract_ntt<m>(_F){}
        void transform_rec(cp::linalg::tensor_view<R,1> &v, bool inverse=false, cp::signals::FFTNormalization normalization = cp::signals::FFTNormalization::None) const
        {
            auto n=v.size();
            if(n==1)
                return;
            std::uint32_t p = this->F->smallest_divisor(n);
            auto q=n/p;
            std::vector<cp::linalg::tensor_subview<R,1>> V;
            for(unsigned i=0;i<p;i++)
                V.push_back(v.slice({i},{n},{p}));
            for(auto &v:V)
                transform_rec(v,inverse,normalization);
            R w=this->root_of_unity(n,m,inverse);
            R z=this->root_of_unity(p,m,inverse);
            R t=1;
            std::vector<R> result(n);
            for(int i=0;i<p;i++,t*=z)
            {
                R h1=1,h2=1;
                for (int j = 0; j < p; j++,h1*=t,h2*=w)
                {
                    R h3=1;
                    for (int k = 0; k < q; k++,h3*=h2)
                        result[i*q+k] += h1 * h3 * V[j](k);
                }
            }
            for(int i=0;i<n;i++)
                v(i)=result[i];
        }

        void transform(cp::linalg::tensor_view<R,1> &v, bool inverse=false, cp::signals::FFTNormalization normalization = cp::signals::FFTNormalization::None) const override
        {
            transform_rec(v,inverse,normalization);
            cyclic_normalize(v,normalization);
        }
    };

}

#endif //CPLIBRARY_NTT_H
//
// Created by ramizouari on 12/01/24.
//

#ifndef CPLIBRARY_GENERATING_H
#define CPLIBRARY_GENERATING_H
//
// Created by ramizouari on 11/01/24.
//

#ifndef CPLIBRARY_FACTORIAL_H
#define CPLIBRARY_FACTORIAL_H
#include <concepts>

namespace cp
{
    template<typename T>
    struct abstract_factorial
    {
        virtual ~abstract_factorial()= default;
        virtual T factorial(integer n) const=0;
        T operator()(integer n) const
        {
            return factorial(n);
        }
    };

    template<typename T>
    struct abstract_inverse_factorial
    {
        virtual T inverse_factorial(integer n) const=0;
        T operator()(integer n) const
        {
            return inverse_factorial(n);
        }
    };

    template<typename T>
    struct naive_factorial : public abstract_factorial<T>
    {
        T factorial(integer n) const override
        {
            T res=1;
            for(int i=2;i<=n;i++)
                res*=i;
            return res;
        }
    };

    template<typename T>
    struct precomputed_factorial : public abstract_factorial<T>
    {
        std::vector<T> F;
        explicit precomputed_factorial(T n)
        {
            F.resize(n+1);
            F[0]=1;
            for(T i=1;i<=n;i++)
                F[i]=F[i-1]*i;
        }
        T factorial(integer n)  const override
        {
            return F[n];
        }
    };

    template<integer n>
    struct precomputed_factorial<cp::cyclic<n>> : public abstract_factorial<cp::cyclic<n>>,public abstract_inverse_factorial<cp::cyclic<n>>
    {
        std::vector<cp::cyclic<n>> F,F_inv;

        explicit precomputed_factorial(integer m, const std::vector<integer> &I):F(m+1),F_inv(m+1)
        {
            F[0]=1;
            F_inv[0]=1;
            for(integer i=1;i<=m;i++)
            {
                F[i] = F[i - 1] * i;
                F_inv[i] = F_inv[i - 1] * I[i];
            }
        }

        explicit precomputed_factorial(integer m): precomputed_factorial(m,cp::inverse_table(m,cp::cyclic<n>::modulus()))
        {

        }

        cp::cyclic<n> factorial(integer m)  const override
        {
            return F[m];
        }

        cp::cyclic<n> inverse_factorial(integer m) const override
        {
            return F_inv[m];
        }
        using abstract_factorial<cp::cyclic<n>>::operator();
    };

    template<>
    struct precomputed_factorial<cp::cyclic<cp::dynamic_modulus>> : public abstract_factorial<cp::cyclic<cp::dynamic_modulus>>,public abstract_inverse_factorial<cp::cyclic<cp::dynamic_modulus>>
    {
        std::vector<cp::cyclic<cp::dynamic_modulus>> F,F_inv;
        explicit precomputed_factorial(integer m,integer p, const std::vector<integer> &I):F(m+1),F_inv(m+1)
        {
            F[0]={1,p};
            F_inv[0]={1,p};
            for(integer i=1;i<=m;i++)
            {
                F[i] = F[i - 1] * i;
                F_inv[i] = F_inv[i - 1] * I[i];
            }
        }

        explicit precomputed_factorial(integer m,integer p): precomputed_factorial(m,p,cp::inverse_table(m,p))
        {

        }

        cp::cyclic<cp::dynamic_modulus> factorial(integer m)  const override
        {
            return F[m];
        }

        cp::cyclic<cp::dynamic_modulus> inverse_factorial(integer m) const override
        {
            return F_inv[m];
        }
        using abstract_factorial::operator();
    };
}

#endif //CPLIBRARY_FACTORIAL_H
namespace cp
{
    template<cp::integer m>
    struct ogf
    {
        virtual ~ogf()= default;
        std::vector<cp::cyclic<m>> P;
        ogf(std::vector<cp::cyclic<m>> _P): P(_P)
        {
        }

        ogf operator+(const ogf &G) const
        {
            std::vector<cp::cyclic<m>> res;
            for(int i=0;i<P.size();i++)
                res.push_back(P[i]+G.P[i]);
            return ogf(res);
        }
        ogf& operator+=(const ogf &G)
        {
            for(int i=0;i<P.size();i++)
                P[i]+=G.P[i];
            return *this;
        }

        operator const std::vector<cp::cyclic<m>> &() const
        {
            return P;
        }

        operator cp::polynomial<cp::cyclic<m>> () const
        {
            return cp::polynomial<cp::cyclic<m>>(P);
        }

        virtual cp::cyclic<m> weight(cp::integer n) const
        {
            return n<P.size()?P[n]:0;
        }


        const std::vector<cp::cyclic<m>> &data() const
        {
            return P;
        }

        std::vector<cp::cyclic<m>> &data()
        {
            return P;
        }
    };

    template<cp::integer m>
    using ordinary_generating_function=ogf<m>;

    template<integer m>
    ogf<m> multiplication(const ogf<m> &A, const ogf<m> &B)
    {
        return ogf<m>(fast_multiplication(A.data(), B.data()));
    }

    template<cp::integer m>
    ogf<m> power(const ogf<m> &A, integer n)
    {
        if(n==0)
            return 1;
        else
        {
            auto Q=power(A,n/2);
            Q=multiplication(Q,Q);
            if(n%2)
                Q=multiplication(Q,A);
            return Q;
        }
    }

    template<integer m>
    ogf<m> multiplication(const ogf<m> &A, const ogf<m> &B, binary_operation_ptr<std::vector<cyclic<m>>> Op)
    {
        return ogf<m>(Op(A, B));
    }

    template<integer m>
    ogf<m> exp(const ogf<m> &A)
    {
        return cp::formal_exp(cp::polynomial(A.data()),A.data().size()).data();
    }

    template<integer m>
    ogf<m> sequence(const ogf<m> &A)
    {
        int n=A.data().size();
        polynomial<cyclic<m>> P(A);
        P.data().resize(n);
        return formal_inv(cyclic<m>{1}-P,n).data();
    }

    template<integer m>
    ogf<m> multiset(const ogf<m> &A)
    {
        int n=A.data().size();
        polynomial<cyclic<m>> P(A);
        P.data().resize(n);
        for(integer i=2;i<n;i++) for(integer j=1;j<n && i*j < n;j++)
            P[i*j]+=A.data()[j]/i;
        return formal_exp(P,P.data().size()).data();
    }

    template<integer m>
    ogf<m> multiset(const ogf<m> &A, std::vector<integer> &I)
    {
        int n=A.data().size();
        polynomial<cyclic<m>> P(A);
        P.data().resize(n);
        for(integer i=2;i<n;i++) for(integer j=1;j<n && i*j < n;j++)
                P[i*j]+=A.data()[j]*I[i];
        return formal_exp(P,P.data().size()).data();
    }

    template<integer m>
    ogf<m> set(const ogf<m> &A)
    {
        int n=A.data().size();
        polynomial<cyclic<m>> P(A);
        P.data().resize(n);
        bool invert=true;
        for(integer i=2;i<n;i++,invert=!invert) for(integer j=1;j<n && i*j < n;j++)
        {
            if(invert)
                P[i*j]-=A.data()[j]/i;
            else
                P[i*j]+=A.data()[j]/i;
        }
        return formal_exp(P,P.data().size()).data();
    }

    template<integer m>
    ogf<m> set(const ogf<m> &A, std::vector<integer> &I)
    {
        int n=A.data().size();
        polynomial<cyclic<m>> P(A);
        P.data().resize(n);
        bool invert=true;
        for(integer i=2;i<n;i++,invert=!invert) for(integer j=1;j<n && i*j < n;j++)
            {
                if(invert)
                    P[i*j]-=A.data()[j]*I[i];
                else
                    P[i*j]+=A.data()[j]*I[i];
            }
        return formal_exp(P,P.data().size()).data();
    }

}
#endif //CPLIBRARY_GENERATING_H
//
// Created by ramizouari on 12/01/24.
//

#ifndef CPLIBRARY_EGF_H
#define CPLIBRARY_EGF_H


namespace cp
{
    template<cp::integer m>
    struct egf
    {
        std::vector<cp::cyclic<m>> P;
        std::shared_ptr<abstract_factorial<cp::cyclic<m>>> factorial;
        std::shared_ptr<abstract_inverse_factorial<cp::cyclic<m>>> inverse_factorial;
        egf(const std::vector<cp::cyclic<m>> &_P, std::shared_ptr<abstract_factorial<cp::cyclic<m>>> _factorial, bool normalized=true,
            std::shared_ptr<abstract_inverse_factorial<cp::cyclic<m>>> _inverse_factorial=nullptr): P(_P)
        {
            factorial=_factorial;
            inverse_factorial=_inverse_factorial;
            if(!normalized)
            {
                if(inverse_factorial) for(int i=0;i<P.size();i++)
                        P[i]*=inverse_factorial->inverse_factorial(i);
                else for(int i=0;i<P.size();i++)
                        P[i]/=factorial->factorial(i);
            }
        }

        cp::cyclic<m> weight(cp::integer n) const
        {
            return n<P.size()?P[n]*factorial->factorial(n):0;
        }

        egf operator+(const egf &G) const
        {
            std::vector<cp::cyclic<m>> res;
            for(int i=0;i<P.size();i++)
                res.push_back(P[i]+G.P[i]);
            return ogf(res);
        }
        egf& operator+=(const egf &G)
        {
            for(int i=0;i<P.size();i++)
                P[i]+=G.P[i];
            return *this;
        }

        operator const std::vector<cp::cyclic<m>> &() const
        {
            return P;
        }

        operator cp::polynomial<cp::cyclic<m>> () const
        {
            return cp::polynomial<cp::cyclic<m>>(P);
        }

        const std::vector<cp::cyclic<m>> &data() const
        {
            return P;
        }

        std::vector<cp::cyclic<m>> &data()
        {
            return P;
        }
    };

    template<cp::integer m>
    using exponential_generating_function=egf<m>;

    template<integer m>
    egf<m> exp_series(integer x,integer n,std::shared_ptr<abstract_factorial<cp::cyclic<m>>> F)
    {
        std::vector<cyclic<m>> P(n);
        P[0]=1;
        for(int i=1;i<n;i++)
            P[i]=P[i-1]*x/F->factorial(i);
        return egf<m>(P,F);
    }

    template<integer m>
    egf<m> exp_series(integer x,integer n,std::shared_ptr<abstract_factorial<cp::cyclic<m>>> F,std::shared_ptr<abstract_inverse_factorial<cp::cyclic<m>>> F_inv)
    {
        std::vector<cyclic<m>> P(n);
        P[0]=1;
        for(int i=1;i<n;i++)
            P[i] = P[i - 1] * x;
        for(int i=0;i<n;i++)
            P[i]*=F_inv->inverse_factorial(i);
        return egf<m>(P,F,true,F_inv);
    }



    template<integer m>
    egf<m> multiplication(const egf<m> &A, const egf<m> &B)
    {
        auto Z=fast_multiplication(A.data(), B.data());
        Z.resize(std::max(A.P.size(),B.data().size()));
        return egf<m>(Z,A.factorial,true,A.inverse_factorial);
    }

    template<integer m>
    egf<m> multiplication(const egf<m> &A, const egf<m> &B, binary_operation_ptr<std::vector<cyclic<m>>> Op)
    {
        return egf<m>(Op(A, B));
    }

    template<cp::integer m>
    egf<m> power(const egf<m> &A, integer n)
    {
        if(n==0)
            return egf<m>({1},A.factorial,true,A.inverse_factorial);
        else
        {
            auto Q=power(A,n/2);
            Q=multiplication(Q,Q);
            if(n%2)
                Q=multiplication(Q,A);
            return Q;
        }
    }

    template<integer m>
    egf<m> exp(const egf<m> &A)
    {
        return egf<m>(formal_exp(cp::polynomial(A.data()),A.data().size()).data(),A.factorial,true,A.inverse_factorial);
    }

    template<integer m>
    egf<m> sequence(const egf<m> &A)
    {
        int n=A.data().size();
        polynomial<cyclic<m>> P(A);
        P.data().resize(n);
        return egf<m>(formal_inv(cyclic<m>{1}-P,n).data(),A.factorial,true,A.inverse_factorial);
    }


    template<integer m>
    egf<m> set(const egf<m> &A)
    {
        return egf<m>(formal_exp(polynomial(A.data()),A.data().size()).data(),A.factorial,true,A.inverse_factorial);
    }

}

#endif //CPLIBRARY_EGF_H

constexpr cp::integer M=998'244'353,L=10e5;
using IK=cp::cyclic<M>;

cp::integer encode(const std::vector<cp::integer> &mixed_basis,const std::vector<cp::integer> &coefficients)
{
    int n=mixed_basis.size();
    cp::integer r=0;
    cp::integer t=1;
    for(int i=0;i<n;i++)
    {
        r += t * coefficients[i];
        t*=mixed_basis[i];
    }
    return r;
}

std::vector<cp::integer> decode(const std::vector<cp::integer> &mixed_basis,cp::integer m)
{
    std::vector<cp::integer> coefficients(mixed_basis.size());
    for(int i=0;i<mixed_basis.size();i++)
    {
        coefficients[i]=m%mixed_basis[i];
        m/=mixed_basis[i];
    }
    return coefficients;
}

cp::polynomial<IK> seq_series(const std::vector<cp::integer> &mixed_basis,std::shared_ptr<cp::abstract_inverse_factorial<IK>> factorial)
{
    auto size=encode(mixed_basis,mixed_basis);
    std::vector<IK> P(size,1);
    for(int i=0;i<size;i++)
    {
        auto coefficients=decode(mixed_basis,i);
        bool admissible=true;
        for(int i=0;i<coefficients.size();i++)
            if(coefficients[i]>= mixed_basis[i]/2)
                admissible=false;
        if(admissible)for(auto c:coefficients)
                P[i]*=factorial->inverse_factorial(c);
    }
    return P;
}

cp::polynomial<IK> exp_series(const std::vector<cp::integer> &mixed_basis,std::shared_ptr<cp::abstract_inverse_factorial<IK>> factorial)
{
    auto size=encode(mixed_basis,mixed_basis);
    std::vector<IK> P(size,1);
    for(int i=0;i<size;i++)
    {
        auto coefficients=decode(mixed_basis,i);
        bool admissible=true;
        for(int i=0;i<coefficients.size();i++)
            if(coefficients[i]>= mixed_basis[i]/2)
                admissible=false;
        if(admissible)for(auto c:coefficients)
            P[i]*=factorial->inverse_factorial(c);
    }
    return P;
}

cp::polynomial<IK> modular_polynomial_multiplication(const cp::polynomial<IK> &A,const cp::polynomial<IK> &B, const std::vector<cp::integer> &mixed_basis)
{
    auto C=cp::fast_multiplication(A,B);
    for(int i=0;i<C.data().size();i++){
        auto coefficients=decode(mixed_basis,i);
        for(int i=0;i<coefficients.size();i++)
            if(coefficients[i]>= mixed_basis[i]/2)
                C.data()[i]=0;
    }
    return C;
}

IK calculate_coefficient(const std::vector<cp::integer> &mixed_basis, std::shared_ptr<cp::precomputed_factorial<IK>> factorial,int w ) {
    auto Z = exp_series(mixed_basis, factorial);
    cp::polynomial<IK> R;
    Z[0] = 0;
    cp::polynomial<IK> P = Z;
    for (int i = 1; i <= w; i++) {
        R += P;
        P = modular_polynomial_multiplication(P, Z, mixed_basis);
    }
    std::vector<cp::integer> last_coefficients(mixed_basis.size());
    IK K = 1;
    for (int i = 0; i < mixed_basis.size(); i++) {
        last_coefficients[i] = mixed_basis[i] / 2;
        K *= factorial->factorial(last_coefficients[i]);
    }
    auto e = encode(mixed_basis, last_coefficients);
    auto result = K * R[e];
    return result;
}

IK count_factorisations(cp::integer n,cp::integer d,std::map<std::pair<cp::integer,cp::integer>,IK> &H,const std::vector<cp::integer> &factors)
{
    if(n==1)
        return 1;
    if(d>n)
        return 0;
    if(H.count({n,d}))
        return H[{n,d}];
    IK R=0;
    auto it=std::lower_bound(factors.begin(),factors.end(),d);
    for(;it!=factors.end();it++) if(*it && n%*it==0)
        R+=count_factorisations(n/(*it),*it,H,factors);
    return H[{n,d}]=R;
}

IK count_factorisations(cp::integer n,const std::vector<cp::integer> &factors) {
    std::map<std::pair<cp::integer, cp::integer>, IK> H;
    auto R=count_factorisations(n, 2, H, factors);
    return R;
}



int main()
{
    auto F=std::make_shared<cp::light_factoriser>(L);
    cp::integer n;
    std::cin >> n;
    auto factors=F->divisors_list_sorted(n);
    auto t1=std::chrono::high_resolution_clock::now();
    auto result=0;//calculate_coefficient(mixed_basis,factorial,w);
    auto t2=std::chrono::high_resolution_clock::now();
    auto result2=count_factorisations(n,factors);
    auto t3=std::chrono::high_resolution_clock::now();
    std::cout << (cp::integer)result2 << std::endl;
}
