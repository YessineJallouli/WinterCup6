//
// Created by ramizouari on 19/12/23.
//

#include "nt/number_theory.h"
#include "nt/modular_functions.h"
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
    auto t1=std::chrono::high_resolution_clock::now();
    auto F=std::make_shared<factoriser>(L);
    default_factoriser_t::default_factoriser=F;
    std::vector<cp::integer> d(L),I(L);
    std::iota(I.begin(),I.end(),0);
    for(int i=1;i<L;i++)
        d[i]=cp::divisors_count(i,*F);
    std::vector<integer> I_table(L);
    I_table=cp::inverse_table(L,M);
    std::vector<IK> K_pow(L),cache(L);
    K_pow[0]=1;
    for(int i=1;i<L;i++)
        K_pow[i]=K_pow[i-1]*K;
    IK R=sector_colours_count_normal(A,K,*F,K_pow,I_table,cache);
    std::fill(cache.begin(),cache.end(),0);
    IK X=sector_colours_count_with_local_reverse(A,K,*F,K_pow,I_table,cache);
    std::fill(cache.begin(),cache.end(),0);
    IK Y=sector_colours_count_with_global_reverse(A,K,*F,K_pow,I_table,cache);
    auto t2=std::chrono::high_resolution_clock::now();
    std::cout << "V1: " << (integer)R << std::endl;
    std::cout << "V2: " << (integer)X << std::endl;
    std::cout << "V3: " << (integer)Y << std::endl;
    std::cout << "Naive V1: " << (integer)naive_count<rotation_equal>(A,K) << std::endl;
    std::cout << "Naive V2: " << (integer)naive_count<dihedral_equal>(A,K) << std::endl;
    std::cout << "Naive V3: " << (integer)naive_count<rotation_equal_mod2>(A,K) << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "ms" << std::endl;
}