
//
// Created by ramizouari on 10/11/23.
//

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <memory>
#include <algorithm>
#include <map>
#include <list>
#include <cstring>
#include <array>
#include <string>
#include "graph/tree/tree.h"
#include "nt/number_theory.h"

using namespace cp;
struct CloneableException : public std::logic_error
{
    CloneableException() : std::logic_error("This object type cannot be cloned"){}
};


struct Cloneable
{
    [[nodiscard]] virtual std::shared_ptr<Cloneable> clone() const
    {
        throw CloneableException();
    }
};


struct RandomNumberGenerator : public Cloneable
{
    inline static std::mt19937_64 rng{};
    using result_type=std::mt19937_64::result_type;
    auto operator()()
    {
        return rng();
    }

    static auto seed(std::uint64_t seed)
    {
        return rng.seed(seed);
    }

    constexpr static auto max()
    {
        return rng.max();
    }

    constexpr static auto min()
    {
        return rng.min();
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<RandomNumberGenerator>(*this);
    }
};


template<typename T>
class Distribution : public Cloneable
{
protected:
    RandomNumberGenerator rng;
public:
    using result_type=T;
    virtual int parameters_count() const = 0;
    virtual T sample() = 0;
    T operator()()
    {
        return sample();
    }

    virtual void update(std::span<std::string> &params) = 0;

};

using RealDistribution = Distribution<double>;

template<typename Dist>
class DistributionManager
{
    std::map<std::string,std::shared_ptr<Dist>> distributions;
public:
    using result_type=typename Dist::result_type;
    template<typename Generator,typename ...H>
    void add(const std::string &name,H&& ... x)
    {
        distributions.emplace(name,std::make_shared<Generator>(std::forward<H>(x)...));
    }

    template<typename ...SamplingParameters>
    result_type sample(const std::string& name,const SamplingParameters & ... x)
    {
        return distributions.at(name)->sample(x...);
    }

    int parameters_count(const std::string& name) const
    {
        return distributions.at(name)->parameters_count();
    }

    void update(const std::string & name, std::span<std::string> &params)
    {
        return distributions.at(name)->update(params);
    }

    std::shared_ptr<Dist> at(const std::string &name)
    {
        if(distributions.count(name))
            return distributions.at(name);
        auto pos = name.find_last_of('@');
        if(pos!=std::string::npos)
        {
            auto sub_name = name.substr(0,pos);
            auto id = name.substr(pos+1);
            if(!id.empty())
            {
                auto it = distributions.emplace(name, std::dynamic_pointer_cast<Dist>(distributions.at(sub_name)->clone()));
                return it.first->second;
            }
            else return  std::dynamic_pointer_cast<Dist>(distributions.at(sub_name)->clone());
        }
        else throw std::out_of_range("Distribution not recognized");
    }
};

using RealDistributionManager=DistributionManager<RealDistribution>;


template<typename Tuple, typename Iterator>
size_t copy_range_to_tuple(Tuple& tup, Iterator begin, Iterator end) {
    size_t count = 0;
    auto copy = [&begin,&end,&count] (auto& value) -> bool {
        if (begin != end) {
            value = *(begin++);
            ++count;
            return true;
        } else {
            return false;
        }
    };
    std::apply([&copy](auto&... values) {
        std::initializer_list<bool>{copy(values)...};
    }, tup);
    return count;
}

template <class T, class Tuple, size_t... Is>
T construct_from_tuple(Tuple&& tuple, std::index_sequence<Is...> ) {
    return T{std::get<Is>(std::forward<Tuple>(tuple))...};
}

template <class T, class Tuple>
T construct_from_tuple(Tuple&& tuple) {
    return construct_from_tuple<T>(std::forward<Tuple>(tuple),
                                   std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{}
    );
}

template<typename Y,typename ...X>
struct type_join
{
    using type = std::tuple<X...,Y>;
};

template<typename Y,typename ...X>
struct type_join<std::tuple<X...>,Y>
{
    using type= typename type_join<Y,X...>::type;
};



template<typename T,int n>
struct repeat_type
{
    using type = typename type_join<typename repeat_type<T,n-1>::type,T>::type;
};

template<typename T>
struct repeat_type<T,0>
{
    using type=std::tuple<>;
};

template<typename O,typename T,int n>
O construct_from_vector(const std::vector<T> A)
{
    typename repeat_type<T,n>::type X;
    copy_range_to_tuple(X,A.begin(),A.end());
    return construct_from_tuple<O>(X);
};

template<typename O,typename T,typename ... P>
O construct_from_vector(const std::vector<T> A)
{
    std::tuple<P...> X;
    copy_range_to_tuple(X,A.begin(),A.end());
    return construct_from_tuple<O>(X);
};



template<typename Dist, typename ...Parameters>
class ParameterizedDistribution : public Distribution<typename Dist::result_type>
{
    Dist d;
public:
    using result_type=typename Dist::result_type;
    using params_type = std::tuple<Parameters...>;
    ParameterizedDistribution() = default;

    ParameterizedDistribution(Parameters&& ... a) : d(std::forward<Parameters>(a)...)
    {
    }

    result_type sample() override
    {
        return d(this->rng);
    }

    void update(Parameters && ... a)
    {
        d.params(a...);
    }

    void update(std::span<std::string> & params) override
    {
        std::vector<double > A;
        auto r=std::min<int>(parameters_count(),params.size());
        for(int i=0;i<r;i++)
            A.push_back(std::stod(params[i]));
        d = construct_from_vector<Dist,double ,Parameters...>(A);
        params = params.subspan(r);
    }

    void update(params_type  &params)
    {
        d = construct_from_tuple<Dist>(params);
    }


    int parameters_count() const override
    {
        return sizeof...(Parameters);
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<ParameterizedDistribution>(*this);
    }

};


template<typename Dist>
auto setup_distribution(std::span<std::string> & params, DistributionManager<Dist> &manager)
{
    auto name = *params.begin();
    auto d= manager.at(name);
    params= params.subspan(1);
    d->update(params);
    return d;
}

struct ParsedArguments
{
    std::uint64_t seed,T;
    std::vector<std::string> arguments;
};

ParsedArguments parse_arguments(int argc,char **argv)
{
    ParsedArguments A;
    if(argc<3)
        throw std::invalid_argument("Not enough arguments");
    A.seed=std::stoull(argv[1]);
    A.T=std::stoull(argv[2]);
    int i;
    for(i=3;i<argc && strncmp(argv[i],"##",2);i++)
        A.arguments.emplace_back(argv[i]);
    return A;
}



struct DiracDeltaDistribution : public RealDistribution
{
    double x;
    int parameters_count() const
    {
        return 1;
    }
    double sample() override
    {
        return x;
    }

    void update(std::span<std::string> &params) override
    {
        x=std::stod(params[0]);
        params = params.subspan(1);
    }


    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<DiracDeltaDistribution>(*this);
    }


};

struct IntegerDiracDeltaDistribution : public Distribution<integer>
{
    integer x;
    int parameters_count() const
    {
        return 1;
    }
    integer sample() override
    {
        return x;
    }

    void update(std::span<std::string> &params) override
    {
        x=std::stoll(params[0]);
        params = params.subspan(1);
    }


    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<IntegerDiracDeltaDistribution>(*this);
    }


};

struct DiscreteDistribution : public Distribution<integer>
{
    integer a,b;
    std::vector<integer> X;
    std::uniform_int_distribution<integer> d;
    bool custom_range;

    DiscreteDistribution(const std::vector<integer> &X_,bool custom_range_=false):X(X_),custom_range(custom_range_)
    {
        a=std::numeric_limits<cp::integer>::min();
        b=std::numeric_limits<cp::integer>::max();
        if(!custom_range_)
            d=std::uniform_int_distribution<integer>(0,X.size()-1);
        std::sort(X.begin(),X.end());
    }
    int parameters_count() const
    {
        return 1;
    }
    integer sample() override
    {

        return X[d(rng)];
    }

    void update(std::span<std::string> &params) override
    {
        if(!custom_range)
            return;
        a=std::stoll(params[0]);
        b=std::stoll(params[1]);
        auto u=std::lower_bound(X.begin(),X.end(),a);
        auto v=std::upper_bound(X.begin(),X.end(),b);
        if(v-u <= 0)
            throw std::invalid_argument("Invalid range");
        d=std::uniform_int_distribution<integer>(0,v-u-1);
        params = params.subspan(2);
    }


    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<DiscreteDistribution>(*this);
    }


};


bool less_than_upper_bound(cp::integer x, cp::integer p, cp::integer x_upper)
{
    constexpr cp::integer arithmetic_limit=1e16;
    return x < arithmetic_limit && x * p < x_upper || x >=arithmetic_limit && std::log(x) + std::log(p) < std::log(x_upper);
}


struct smooth_numbers_graph
{
    const std::vector<cp::integer> &primes;
public:
    smooth_numbers_graph(const std::vector<cp::integer> &_primes) : primes(_primes)
    {
    }



    void highly_composite_sieve(std::map<cp::integer,cp::integer> &Z,std::vector<cp::integer> &X, cp::integer x, cp::integer d, cp::integer x_upper)
    {
        if(!Z.count(d))
            Z[d]=x;
        else Z[d]=std::min(Z[d],x);
        if(x > x_upper)
            return;
        int i=X.size()-1;
        if((i==0 || i > 0 && X[i] < X[i - 1]) && less_than_upper_bound(x,primes[i],x_upper))
        {
            X[i]++;
            highly_composite_sieve(Z, X, x * primes[i], d / X[i] * (X[i]+1), x_upper);
            X[i]--;
        }
        auto p=primes[X.size()];
        if(less_than_upper_bound(x,p,x_upper))
        {
            X.push_back(1);
            highly_composite_sieve(Z,X, x*p, 2*d, x_upper);
            X.pop_back();
        }
    }

    std::map<cp::integer,cp::integer> highly_composite_sieve(cp::integer x_upper)
    {
        std::map<cp::integer,cp::integer> Z;
        std::vector<cp::integer> X;
        highly_composite_sieve(Z,X,1,1,x_upper);
        return Z;
    }
};


struct Query
{
    integer n,m,x,y,d;
    Query() = default;
    Query(integer _n,integer _m,integer _x,integer _y,integer _d):n(_n),m(_m),x(_x),y(_y),d(_d){}
};

using QueryDistribution = Distribution<Query>;

using IntegerDistribution = Distribution<integer>;
using IntegerDistributionManager=DistributionManager<IntegerDistribution>;
struct UniformQueryDistribution : public QueryDistribution
{
    IntegerDistributionManager integerManager;
    std::shared_ptr<Distribution<integer>> N_dist,M_dist,day_dist;
public:
    explicit UniformQueryDistribution(IntegerDistributionManager &manager): integerManager(manager)
    {
    }

    Query sample()
    {
        auto n=N_dist->sample(),m=M_dist->sample(),d=day_dist->sample();
        Query Q(n,m,0,0,d);
        std::uniform_int_distribution<integer> d_x(0,n-1);
        std::uniform_int_distribution<integer> d_y(0,m-1);
        Q.x=d_x(rng);
        Q.y=d_y(rng);
        return Q;
    }

    void update(std::span<std::string> &params)
    {
        N_dist = setup_distribution(params,integerManager);
        M_dist = setup_distribution(params,integerManager);
        day_dist = setup_distribution(params,integerManager);
    }

    int parameters_count() const override
    {
        return 0;
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<UniformQueryDistribution>(*this);
    }

};

struct UniformDivisorQueryDistribution : public QueryDistribution
{
    IntegerDistributionManager integerManager;
    std::shared_ptr<Distribution<integer>> N_dist,M_dist;
    cp::abstract_factoriser &factoriser;
public:
    explicit UniformDivisorQueryDistribution(cp::abstract_factoriser &factoriser,IntegerDistributionManager &manager): integerManager(manager),factoriser(factoriser)
    {
    }

    Query sample()
    {
        auto n=N_dist->sample(),m=M_dist->sample();
        Query Q(n,m,0,0,0);
        auto D1=factoriser.divisors_list(n),D2=factoriser.divisors_list(m);
        std::sort(D1.begin(),D1.end());
        std::sort(D2.begin(),D2.end());
        std::vector<integer> D;
        std::set_intersection(D1.begin(),D1.end(),D2.begin(),D2.end(),std::back_inserter(D));
        std::uniform_int_distribution<integer> day(0,D.size()-1),d1(0,D1.size()-1),d2(0,D2.size()-1);
        std::uniform_int_distribution<integer> M1(0,n),M2(0,m);
        Q.d=D[day(rng)];
        Q.x=D1[d1(rng)]*M1(rng) % n;
        Q.y=D2[d2(rng)]*M2(rng) % m;
        return Q;
    }

    void update(std::span<std::string> &params)
    {
        N_dist = setup_distribution(params,integerManager);
        M_dist = setup_distribution(params,integerManager);
    }

    int parameters_count() const override
    {
        return 0;
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<UniformDivisorQueryDistribution>(*this);
    }
};

using QueryDistributionManager=DistributionManager<QueryDistribution>;
constexpr integer MAXN=1000000,MAX_EXP_SIEVE=1e9;

std::vector<cp::integer> build_highly_composite_numbers(const std::map<cp::integer,cp::integer> &candidates)
{
    std::vector<std::pair<cp::integer,cp::integer>> Y;
    for(auto [n,d]:candidates) if(n)
            Y.emplace_back(d,n);
    std::sort(Y.begin(),Y.end());
    std::vector<cp::integer> HCN;
    cp::integer d_max=0;
    for(auto [n,d]:Y) if(d>d_max)
        {
            d_max=d;
            HCN.push_back(n);
        }
    return HCN;
}

std::vector<cp::integer> build_fibonacci_numbers(cp::integer x_upper)
{
    std::vector<cp::integer> F={1,1};
    while(F.back() <= x_upper)
        F.push_back(F[F.size()-1]+F[F.size()-2]);
    if(F.back() > x_upper)
        F.pop_back();
    return F;
}

template<>
struct Distribution<std::vector<integer>> : public Cloneable
{
    virtual std::vector<integer> sample(int n) = 0;
    virtual void update(std::span<std::string> &params) =0;
    virtual int parameters_count() const=0;

    using result_type=std::vector<integer>;

};

using ArrayDistribution = Distribution<std::vector<integer>>;

using ArrayDistributionManager=DistributionManager<ArrayDistribution>;

struct IndependentArrayDistribution : public ArrayDistribution {
    IntegerDistributionManager integerManager;
    std::shared_ptr<Distribution<integer>> X_dist;
public:
    explicit IndependentArrayDistribution(IntegerDistributionManager &manager) : integerManager(manager) {}

    std::vector<integer> sample(int n) override {
        std::vector<integer> A(n);
        for (int i = 0; i < n; i++)
            A[i] = X_dist->sample();
        return A;
    }

    void update(std::span<std::string> &params) override {
        X_dist = setup_distribution(params, integerManager);
    }

    int parameters_count() const override {
        return 0;
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override {
        return std::make_unique<IndependentArrayDistribution>(*this);
    }
};

struct SortedArrayDistribution : public ArrayDistribution {
    ArrayDistributionManager &arrayDistributionManager;
    std::shared_ptr<ArrayDistribution> arrayDist;
public:
    explicit SortedArrayDistribution(ArrayDistributionManager &manager) : arrayDistributionManager(manager) {}

    std::vector<integer> sample(int n) override {
        auto A = arrayDist->sample(n);
        std::sort(A.begin(), A.end());
        return A;
    }

    void update(std::span<std::string> &params) override {
        arrayDist = setup_distribution(params, arrayDistributionManager);
    }

    int parameters_count() const override {
        return 0;
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override {
        return std::make_unique<SortedArrayDistribution>(*this);
    }
};

struct SortedComponentsDistribution : public ArrayDistribution {
    IntegerDistributionManager &integerDistributionManager;
    ArrayDistributionManager &arrayDistributionManager;
    std::shared_ptr<IntegerDistribution > sizeDist;
    std::shared_ptr<ArrayDistribution> arrayDist;

public:
    explicit SortedComponentsDistribution(IntegerDistributionManager &manager,
                                          ArrayDistributionManager &manager2) : integerDistributionManager(manager), arrayDistributionManager(manager2) {}

    std::vector<integer> sample(int n) override {
        int i=0,j=i;
        std::vector<integer> A(n);
        while(i<n)
        {
            int m=sizeDist->sample();
            m=std::min(m,n-i);
            auto B=arrayDist->sample(m);
            std::sort(B.begin(),B.end());
            for(int k=0;k<m;k++)
                A[j++]=B[k];
            i=j;
        }
        return A;
    }

    void update(std::span<std::string> &params) override {
        sizeDist = setup_distribution(params, integerDistributionManager);
        arrayDist = setup_distribution(params, arrayDistributionManager);
    }

    int parameters_count() const override {
        return 0;
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override {
        return std::make_unique<SortedComponentsDistribution>(*this);
    }
};


int main(int argc, char**argv)
{
    auto [seed,T, args] = parse_arguments(argc,argv);
    cp::light_factoriser factoriser(MAXN);
    smooth_numbers_graph G(factoriser.prime_list());
    auto HCN_candidates=G.highly_composite_sieve(MAX_EXP_SIEVE);
    auto HCN=build_highly_composite_numbers(HCN_candidates);
    RealDistributionManager realManager;
    IntegerDistributionManager integerManager;
    ArrayDistributionManager arrayManager;
    RandomNumberGenerator::seed(seed);
    realManager.add<ParameterizedDistribution<std::exponential_distribution<double>,double>>("exponential");
    realManager.add<ParameterizedDistribution<std::uniform_real_distribution<double>,double,double>>("uniform-real");
    realManager.add<ParameterizedDistribution<std::normal_distribution<double>,double,double>>("normal");
    realManager.add<ParameterizedDistribution<std::cauchy_distribution<double>,double,double>>("cauchy");
    realManager.add<ParameterizedDistribution<std::gamma_distribution<double>,double,double>>("gamma");
    realManager.add<DiracDeltaDistribution>("dirac");
    integerManager.add<ParameterizedDistribution<std::uniform_int_distribution<integer>,integer,integer>>("uniform");
    integerManager.add<ParameterizedDistribution<std::binomial_distribution<integer>,integer,double>>("binomial");
    integerManager.add<ParameterizedDistribution<std::geometric_distribution<integer>,double>>("geometric");
    integerManager.add<ParameterizedDistribution<std::poisson_distribution<integer>,double>>("poisson");
    integerManager.add<IntegerDiracDeltaDistribution>("dirac");
    integerManager.add<DiscreteDistribution>("hcn",HCN);
    integerManager.add<DiscreteDistribution>("hcn-custom",HCN,true);
    integerManager.add<DiscreteDistribution>("fibonacci",build_fibonacci_numbers(MAX_EXP_SIEVE));
    arrayManager.add<IndependentArrayDistribution>("independent",integerManager);
    arrayManager.add<SortedArrayDistribution>("sorted",arrayManager);
    arrayManager.add<SortedComponentsDistribution>("sorted-components",integerManager,arrayManager);
    std::span params(args);
    auto sizeDist = setup_distribution(params, integerManager);
    auto arrayDist = setup_distribution(params,arrayManager);
    std::cout << T << '\n';
    for(int i=0;i<T;i++)
    {
        auto n=sizeDist->sample();
        auto A=arrayDist->sample(n);
        std::cout << A.size() << '\n';
        for(int j=0;j<A.size();j++)
        {
            std::cout << A[j];
            std::cout << (j==A.size()-1?'\n':' ');
        }
    }
}


