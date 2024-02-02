#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <map>
#include <span>
#include <algorithm>

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

using integer=long long;

using RealDistribution = Distribution<double>;
using IntegerDistribution = Distribution<integer>;

template<typename Dist>
class DistributionManager
{
    std::map<std::string,std::shared_ptr<Dist>> distributions;
public:
    using result_type=typename Dist::result_type;
    template<typename Generator,typename ...H>
    void add(const std::string &name,H&& ... x)
    {
        distributions.emplace(name,std::make_unique<Generator>(std::forward<H>(x)...));
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
using IntegerDistributionManager=DistributionManager<IntegerDistribution>;


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

    using params_type = std::tuple<Parameters...>;
    using result_type=typename Dist::result_type;
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
        std::vector<double> A;
        auto r=std::min<int>(parameters_count(),params.size());
        for(int i=0;i<r;i++)
            A.push_back(std::stod(params[i]));
        d = construct_from_vector<Dist,double,Parameters...>(A);
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
    std::vector<std::string> distribution_arguments;
};

ParsedArguments parse_arguments(int argc,char **argv)
{
    ParsedArguments A;
    if(argc<3)
        throw std::invalid_argument("Not enough arguments");
    A.seed=std::stoull(argv[1]);
    A.T=std::stoull(argv[2]);
    for(int i=3;i<argc;i++)
        A.distribution_arguments.emplace_back(argv[i]);
    return A;
}

struct Output
{
    int n,m;
};

struct IndependentDistribution : public Distribution<Output>
{
    IntegerDistributionManager integerManager;
    std::shared_ptr<IntegerDistribution> d1,d2;
    IndependentDistribution(IntegerDistributionManager _integerManager) :integerManager(_integerManager)
    {
    }

    Output sample() override
    {
        Output O;
        O.n=d1->sample();
        O.m=d2->sample();
        std::tie(O.n,O.m)=std::minmax(O.n,O.m);
        return O;
    }

    void update(std::span<std::string> &params) override
    {
        d1= setup_distribution(params,integerManager);
        d2= setup_distribution(params,integerManager);
    }

    int parameters_count() const override
    {
        return 0;
    }


    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<IndependentDistribution>(*this);
    }
};


int main(int argc, char**argv)
{
    auto [seed,T,distribution_arguments] = parse_arguments(argc,argv);
    RealDistributionManager realManager;
    IntegerDistributionManager integerManager;
    DistributionManager<Distribution<Output>> outputManager;
    RandomNumberGenerator::seed(seed);
    realManager.add<ParameterizedDistribution<std::exponential_distribution<double>,double>>("exponential");
    realManager.add<ParameterizedDistribution<std::uniform_real_distribution<double>,double,double>>("uniform-real");
    realManager.add<ParameterizedDistribution<std::normal_distribution<double>,double,double>>("normal");
    realManager.add<ParameterizedDistribution<std::cauchy_distribution<double>,double,double>>("cauchy");
    realManager.add<ParameterizedDistribution<std::gamma_distribution<double>,double,double>>("gamma");
    integerManager.add<ParameterizedDistribution<std::uniform_int_distribution<integer>,integer,integer>>("uniform");
    integerManager.add<ParameterizedDistribution<std::binomial_distribution<integer>,integer,double>>("binomial");
    integerManager.add<ParameterizedDistribution<std::poisson_distribution<integer>,double>>("poisson");
    integerManager.add<ParameterizedDistribution<std::geometric_distribution<integer>,double>>("geometric");
    outputManager.add<IndependentDistribution>("independent",integerManager);
    std::cout << T << '\n';
    std::span params(distribution_arguments);
    auto d=setup_distribution(params,outputManager);
    while(T--)
    {
        Output O=d->sample();
        std::cout << O.n << ' ' << O.m << '\n';
    }
}


