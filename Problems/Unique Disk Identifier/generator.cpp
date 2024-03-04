
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
#include "nt/functions.h"

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
    std::uint64_t seed,sectors,colors;
    std::vector<std::string> arguments;
};

ParsedArguments parse_arguments(int argc,char **argv)
{
    ParsedArguments A;
    if(argc<3)
        throw std::invalid_argument("Not enough arguments");
    A.seed=std::stoull(argv[1]);
    A.sectors=std::stoull(argv[2]);
    A.colors=std::stoull(argv[3]);
    for(int i=4;i<argc && strncmp(argv[i],"##",2);i++)
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

using IntegerDistribution = Distribution<integer>;
using IntegerDistributionManager=DistributionManager<IntegerDistribution>;

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

struct HighlyCompositeDistribution : public Distribution<integer>
{
    std::vector<cp::integer> I;
    int index=0;
    int start;
    HighlyCompositeDistribution(const std::vector<integer> &I):I(I){}

    int parameters_count() const
    {
        return 1;
    }
    integer sample() override
    {
        if(index==I.size())
            throw std::out_of_range("No more numbers");
        return I[index++];
    }

    void update(std::span<std::string> &params) override
    {
        start=std::stoi(params[0]);
        params = params.subspan(1);
        auto it=std::lower_bound(I.begin(),I.end(),start,std::greater<>());
        if(it!=I.end())
            index=it-I.begin();
        else throw std::invalid_argument("Invalid start");
    }


    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<HighlyCompositeDistribution>(*this);
    }
};

int main(int argc, char**argv)
{
    auto [seed,sectors,colors, args] = parse_arguments(argc,argv);
    cp::light_factoriser factoriser(MAXN);
    std::vector<cp::integer> I(MAXN),D(MAXN+1);
    for(int i=1;i<=MAXN;i++)
        I[i-1]=i;
    std::reverse(I.begin(),I.end());
    std::stable_sort(I.begin(),I.end(),[&factoriser,&D](cp::integer a,cp::integer b)
    {
        cp::integer d1,d2;
        if(D[a])
            d1=D[a];
        else
            D[a]=d1=cp::divisors_count(a,factoriser);
        if(D[b])
            d2=D[b];
        else
            D[b]=d2=cp::divisors_count(b,factoriser);
        return d1 > d2;
    });
    RealDistributionManager realManager;
    IntegerDistributionManager integerManager;
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
    integerManager.add<HighlyCompositeDistribution>("high-divisors",I);
    std::span params(args);
    auto distribution = setup_distribution(params,integerManager);
    std::cout << sectors << ' ' << colors << '\n';
    for(int i=0;i<sectors;i++)
        std::cout << std::max<cp::integer>(distribution->sample(),1) << (i==sectors-1?'\n':' ');
    return 0;
}

