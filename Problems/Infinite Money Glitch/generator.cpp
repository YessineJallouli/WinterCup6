
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

struct Exchange
{
    double rate,fee;
};

template<>
class Distribution<graph::WeightedGraph<Exchange>> : public Cloneable
{
protected:
    RandomNumberGenerator rng;
public:
    using result_type=graph::WeightedGraph<Exchange>;
    virtual int parameters_count() const = 0;
    virtual graph::WeightedGraph<Exchange> sample(const graph::Graph &G) = 0;
    graph::WeightedGraph<Exchange> operator()(const graph::Graph &G)
    {
        return sample(G);
    }

    virtual void update(std::span<std::string> &params) = 0;
};


using TreeDistribution = Distribution<graph::Tree>;
using GraphDistribution = Distribution<graph::Graph>;
using RealDistribution = Distribution<double>;
using ExchangeGraphDistribution = Distribution<graph::WeightedGraph<Exchange>>;

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

using TreeDistributionManager=DistributionManager<TreeDistribution>;
using GraphDistributionManager=DistributionManager<GraphDistribution>;
using RealDistributionManager=DistributionManager<RealDistribution>;
using ExchangeDistributionManager= DistributionManager<ExchangeGraphDistribution>;


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
class ParameterizedDistribution : public Distribution<double>
{
    Dist d;
public:
    using params_type = std::tuple<Parameters...>;
    ParameterizedDistribution() = default;

    ParameterizedDistribution(Parameters&& ... a) : d(std::forward<Parameters>(a)...)
    {
    }

    double sample() override
    {
        return d(rng);
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
    std::uint64_t seed;
    std::vector<std::string> graph_arguments,exchange_arguments,max_money_arguments;
};

ParsedArguments parse_arguments(int argc,char **argv)
{
    ParsedArguments A;
    if(argc<3)
        throw std::invalid_argument("Not enough arguments");
    A.seed=std::stoull(argv[1]);
    int i;
    for(i=2;i<argc && strncmp(argv[i],"##",2);i++)
        A.graph_arguments.emplace_back(argv[i]);
    for(i=i+1;i<argc && strcmp(argv[i],"##"); i++)
        A.exchange_arguments.emplace_back(argv[i]);
    for(i=i+1;i<argc && strcmp(argv[i],"##"); i++)
        A.max_money_arguments.emplace_back(argv[i]);
    return A;
}

struct CompleteGraph : public GraphDistribution
{
    int n;
    int parameters_count() const
    {
        return 0;
    }
    virtual graph::Graph sample() override
    {
        graph::Graph G(n);
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                G.connect(i,j);
        return G;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        params = params.subspan(1);
    }
};

struct GnpGraph : public GraphDistribution
{
    int n;
    double p;
    bool directed,loops;
    int parameters_count() const
    {
        return 1;
    }
    virtual graph::Graph sample() override
    {
        graph::Graph G(n);
        std::bernoulli_distribution D(p);
        if(directed)
        {
            for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (i != j || loops) if (D(rng))
                G.connect(i, j);
        }
        else
        {
            for (int i = 0; i < n; i++) for (int j = i; j < n; j++) if (i != j || loops) if (D(rng))
                G.connect(i, j);
        }
        return G;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        p=std::stod(params[1]);
        directed=std::stoi(params[2]);
        loops=std::stoi(params[3]);
        params = params.subspan(4);
    }
};

struct GnmGraph : public GraphDistribution
{
    int n,m;
    bool directed,loops;
    int parameters_count() const
    {
        return 1;
    }
    virtual graph::Graph sample() override
    {
        graph::Graph G(n);
        std::vector<std::pair<int,int>> E;
        for(int i=0;i<n;i++) for(int j=directed?0:i;j<n;j++) if(j!=i || loops)
            E.emplace_back(i,j);
        std::shuffle(E.begin(),E.end(),rng);
        for(int i=0;i<m;i++)
            G.connect(E[i].first,E[i].second);
        return G;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        m=std::stoi(params[1]);
        directed=std::stoi(params[2]);
        loops=std::stoi(params[3]);
        params = params.subspan(4);
    }
};

struct RnmGraph : public GraphDistribution
{
    int n,m;
    bool directed,loops;
    int parameters_count() const
    {
        return 1;
    }
    virtual graph::Graph sample() override
    {
        graph::Graph G(n);
        std::uniform_int_distribution U(0,n-1);
        for(int i=0;i<m;i++)
        {
            int u=U(rng);
            int v;
            if(!loops)
            {
                std::uniform_int_distribution V(0,n-2);
                v=V(rng);
                if(v>=u)
                    v++;
            }
            else
                v=U(rng);
            if(!directed)
                std::swap(u,v);
            G.connect(u,v);
        }
        return G;
    }


    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        m=std::stoi(params[1]);
        directed=std::stoi(params[2]);
        loops=std::stoi(params[3]);
        params = params.subspan(4);
    }
};

struct CycleGraph : public GraphDistribution
{
    int n;
    int parameters_count() const
    {
        return 1;
    }

    virtual graph::Graph sample() override
    {
        graph::Graph G(n);
        for(int i=0;i<n;i++)
            G.connect(i,(i+1)%n);
        return G;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        params = params.subspan(1);
    }
};

struct FriendshipGraph : public GraphDistribution
{
    int n,m;
    int parameters_count() const
    {
        return 2;
    }

    int normalize(int x)
    {
        if(x%n==0)
            return 0;
        else return x;
    }

    int normalize(int x,int R)
    {
        if(x==0)
            return 0;
        else return x+R;
    }

    virtual graph::Graph sample() override
    {
        graph::Graph G(m*n-m+1);
        int R=0;
        for(int i=0;i<m;i++)
        {
            int c=n;
            for (int j = 0; j < c; j++)
            {
                auto a = j, b = (j + 1) % c;
                G.connect(normalize(a,R), normalize(b,R));
            }
            R+=c-1;
        }
        return G;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        m=std::stoi(params[1]);
        params = params.subspan(2);
    }
};

struct JoinedCycleGraph : public GraphDistribution
{
    std::vector<int> cycles;
    int parameters_count() const
    {
        return 0;
    }

    int normalize(int x,int c,int R)
    {
        if(x%c==0)
            return 0;
        else return x+R;
    }

    virtual graph::Graph sample() override
    {
        auto nodes=std::reduce(cycles.begin(),cycles.end())-cycles.size()+1;
        graph::Graph G(nodes);
        int R=0;
        for(int c : cycles)
        {
            for (int j = 0; j < c; j++)
            {
                auto a = j, b = (j + 1) % c;
                G.connect(normalize(a,c,R), normalize(b,c,R));
            }
            R+=c-1;
        }
        return G;
    }

    void update(std::span<std::string> &params) override
    {
        int m=std::stoi(params[0]);
        cycles.resize(m);
        for(int i=0;i<m;i++)
            cycles[i]=std::stoi(params[i+1]);
        params = params.subspan(m+1);
    }
};

struct WeightedEdge
{
    int u,v;
    double w;
    WeightedEdge(int _u,int _v,double _w) : u(_u),v(_v),w(_w){}
    std::partial_ordering operator<=>(const WeightedEdge &e) const
    {
        return w<=>e.w;
    }
};

struct RandomSpanningTree : public TreeDistribution
{
    GraphDistributionManager &graphManager;
    RealDistributionManager &realManager;
    std::shared_ptr<GraphDistribution> graphDistribution;
    std::shared_ptr<RealDistribution> realDistribution;
    explicit RandomSpanningTree(GraphDistributionManager &G,RealDistributionManager &X) : graphManager(G),realManager(X)
    {

    }
    int parameters_count() const
    {
        return 0;
    }
    graph::Tree sample() override
    {
        auto G = graphDistribution->sample();
        std::vector<WeightedEdge> E;
        auto n=G.size();
        for(int u=0;u<n;u++) for(auto v:G.adjacencyList[u]) if(u<v)
                    E.emplace_back(u,v,realDistribution->sample());
        UnionFind UF(G.size());
        std::sort(E.begin(),E.end());
        graph::Tree T(n);
        for(auto &e:E) if(!UF.equivalent(e.u,e.v))
            {
                UF.connect(e.u,e.v);
                T.connect(e.u,e.v);
            }
        T.reRoot(0);
        return T;
    }

    void update(std::span<std::string> &params) override
    {
        graphDistribution = setup_distribution(params,graphManager);
        realDistribution = setup_distribution(params,realManager);
    }
};

struct RandomConnectionTree : public TreeDistribution
{
    int n;
    int parameters_count() const
    {
        return 0;
    }
    graph::Tree sample() override
    {
        int components=n;
        UnionFind UF(n);
        std::uniform_int_distribution<int> d(0,n-1);
        graph::Tree T(n);
        while(components>1)
        {
            int u=d(rng),v=d(rng);
            if(!UF.equivalent(u,v))
            {
                UF.connect(u,v);
                components--;
                T.connect(u,v);
            }
        }
        T.reRoot(0);
        return T;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        params = params.subspan(1);
    }
};

struct Edge
{
    int u,v;
};
using Edges=std::vector<Edge>;


struct UniformSpanningTree : public TreeDistribution
{
    GraphDistributionManager &graphManager;
    std::shared_ptr<GraphDistribution> graphDistribution;
    explicit UniformSpanningTree(GraphDistributionManager &G) : graphManager(G)
    {
    }

    int parameters_count() const
    {
        return 0;
    }
    graph::Tree sample() override
    {
        auto G = graphDistribution->sample();
        auto n=G.size();
        UnionFind UF(n);
        RandomNumberGenerator rng;
        std::uniform_int_distribution<int> d_root(0,n-1);
        int root=d_root(rng);
        graph::Tree T(n);
        std::vector<int> nodes;
        for(int i=0;i<n;i++) if(i!=root)
                nodes.push_back(i);
        while(!nodes.empty())
        {
            std::set<int> values(nodes.begin(),nodes.end());
            std::uniform_int_distribution<int> d(0,nodes.size()-1);
            int u=nodes[d(rng)];
            auto Y= loop_erased_random_walk(u,root,G,UF,rng);
            for(auto v:Y)
            {
                values.erase(v);
                UF.connect(u,v);
            }
            for(int i=0;i<Y.size()-1;i++)
                T.connect(Y[i],Y[i+1]);
            nodes.resize(values.size());
            std::copy(values.begin(),values.end(),nodes.begin());
        }
        T.reRoot(root);
        return T;
    }


    void update(std::span<std::string> &params) override
    {
        graphDistribution = setup_distribution(params,graphManager);
    }
protected:
    Edges random_walk(int source,int root,const graph::Graph &G, UnionFind &UF,RandomNumberGenerator & rng)
    {
        Edges edges;
        int u=source;
        while(!UF.equivalent(u,root))
        {
            auto degree=G.adjacencyList[u].size();
            std::uniform_int_distribution<int> d(0,degree-1);
            int v=G.adjacencyList[u][d(rng)];
            edges.emplace_back(u,v);
            u=v;
        }
        return edges;
    }

    std::vector<int> loop_erased_random_walk(int source, int root, const graph::Graph &G, UnionFind &UF, RandomNumberGenerator &rng)
    {
        auto E=random_walk(source,root,G,UF,rng);
        std::vector<int> nodes;
        nodes.push_back(source);
        for(auto [u,v]:E)
            nodes.push_back(v);
        std::vector<int> I={0};
        int dest=nodes.back();
        while(nodes[I.back()]!=dest)
        {
            int j=0;
            for(int i=I.back();i<nodes.size();i++)
                if(nodes[i]==nodes[I.back()])
                    j=i;
            I.push_back(j+1);
        }
        std::vector<int> R;
        for(int i : I)
            R.push_back(nodes[i]);
        return R;
    }
};

struct MAryRandomTree : public TreeDistribution
{
    int n,m;
    int parameters_count() const
    {
        return 0;
    }
    graph::Tree sample() override
    {
        graph::Tree T(n);
        std::vector<int> nodes;
        for(int i=0;i<n;i++)
            nodes.push_back(i);
        std::shuffle(nodes.begin(),nodes.end(),rng);
        for(int i=1;i<n;i++)
            T.setParent(nodes[i],nodes[(i-1)/m]);
        T.reRoot(0);
        return T;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        m=std::stoi(params[1]);
        params = params.subspan(2);
    }
};

struct ChainedList : public TreeDistribution
{
    int n;
    int parameters_count() const
    {
        return 0;
    }
    graph::Tree sample() override
    {
        graph::Tree T(n);
        for(int i=0;i<n-1;i++)
            T.setParent(i,i+1);
        T.reRoot(0);
        return T;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        params = params.subspan(1);
    }
};

struct MAryRandomInsertionTree : public TreeDistribution
{
    int n,m;
    int parameters_count() const
    {
        return 0;
    }
    graph::Tree sample() override
    {
        graph::Tree T(n);
        std::vector<int> nodes;
        for(int i=0;i<n;i++)
            nodes.push_back(i);
        std::shuffle(nodes.begin(),nodes.end(),rng);
        for(int i=1;i<n;i++)
            insert(nodes[i],nodes[0],T,rng);
        T.reRoot(0);
        return T;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        m=std::stoi(params[1]);
        params = params.subspan(2);
    }

    void insert(int u, int s, graph::Tree &T, RandomNumberGenerator & rng)
    {
        std::uniform_int_distribution d(0,m-1);
        auto X=&T.children(s);
        while(X->size()==m)
        {
            s=T.children(s)[d(rng)];
            X=&T.children(s);
        }
        T.setParent(u,s);
    }
};

struct RandomInsertionTree : public TreeDistribution
{
    int n;
    int parameters_count() const
    {
        return 0;
    }
    graph::Tree sample() override
    {
        graph::Tree T(n);
        std::vector<int> nodes;
        for(int i=0;i<n;i++)
            nodes.push_back(i);
        std::shuffle(nodes.begin(),nodes.end(),rng);
        for(int i=1;i<n;i++)
            insert(nodes[i],nodes[0],T,rng);
        T.reRoot(0);
        return T;
    }

    void update(std::span<std::string> &params) override
    {
        n=std::stoi(params[0]);
        params = params.subspan(1);
    }

    void insert(int u, int s, graph::Tree &T, RandomNumberGenerator & rng)
    {
        auto X=&T.children(s);
        std::uniform_int_distribution<int> d(0,X->size());
        int r=d(rng);
        while(r<X->size())
        {
            s=T.children(s)[r];
            X=&T.children(s);
            d=std::uniform_int_distribution<int>(0,X->size());
        }
        T.setParent(u,s);
    }
};

struct IIDExchangeDistribution : public ExchangeGraphDistribution
{
    RealDistributionManager &realManager;
    std::shared_ptr<RealDistribution> rateDistribution,feeDistribution;
    IIDExchangeDistribution(RealDistributionManager &manager): realManager(manager){}
    int parameters_count() const
    {
        return 0;
    }
    graph::WeightedGraph<Exchange> sample(const graph::Graph &G) override
    {
        graph::WeightedGraph<Exchange> E(G.size());
        for(int i=0;i<G.size();i++) for(auto j:G.adjacencyList[i])
            E.connect(i,j,Exchange{rateDistribution->sample(),feeDistribution->sample()});
        return E;
    }

    void update(std::span<std::string> &params) override
    {
        rateDistribution = setup_distribution(params,realManager);
        feeDistribution = setup_distribution(params,realManager);
    }

};

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

void print(std::ostream &H,const graph::WeightedGraph<Exchange>& EG, int M)
{
    auto V=EG.size();
    auto E=std::accumulate(EG.adjacencyList.begin(),EG.adjacencyList.end(),0,[](int s,const auto &x){return s+x.size();});
    H << V << ' ' << E << ' ' << M << '\n';
    for(int i=0;i<EG.size();i++)
        for(auto [j,conversion]:EG.adjacencyList[i])
            H << i+1 << ' ' << j+1 << ' ' << std::fixed << std::setprecision(2)
                      << conversion.rate << ' ' << std::fixed << std::setprecision(2) << conversion.fee << '\n';
}

int main(int argc, char**argv)
{
    auto [seed,distribution_arguments,exchange_arguments,allowed_money_arguments] = parse_arguments(argc,argv);
    TreeDistributionManager treeManager;
    RealDistributionManager realManager;
    GraphDistributionManager graphManager;
    ExchangeDistributionManager exchangeManager;
    RandomNumberGenerator::seed(seed);
    realManager.add<ParameterizedDistribution<std::exponential_distribution<double>,double>>("exponential");
    realManager.add<ParameterizedDistribution<std::uniform_int_distribution<int>,int,int>>("uniform");
    realManager.add<ParameterizedDistribution<std::uniform_real_distribution<double>,double,double>>("uniform-real");
    realManager.add<ParameterizedDistribution<std::normal_distribution<double>,double,double>>("normal");
    realManager.add<ParameterizedDistribution<std::cauchy_distribution<double>,double,double>>("cauchy");
    realManager.add<ParameterizedDistribution<std::gamma_distribution<double>,double,double>>("gamma");
    realManager.add<ParameterizedDistribution<std::binomial_distribution<int>,int,double>>("binomial");
    realManager.add<ParameterizedDistribution<std::poisson_distribution<int>,double>>("poisson");
    realManager.add<ParameterizedDistribution<std::geometric_distribution<int>,double>>("geometric");
    realManager.add<DiracDeltaDistribution>("dirac");
    graphManager.add<GnpGraph>("gnp");
    graphManager.add<GnmGraph>("gnm");
    graphManager.add<RnmGraph>("rnm");
    graphManager.add<CompleteGraph>("complete");
    graphManager.add<CycleGraph>("cycle");
    graphManager.add<FriendshipGraph>("friendship");
    graphManager.add<JoinedCycleGraph>("joined-cycle");
    treeManager.add<RandomSpanningTree>("random-spanning-tree",graphManager,realManager);
    treeManager.add<UniformSpanningTree>("uniform-spanning-tree",graphManager);
    treeManager.add<MAryRandomTree>("m-ary-tree");
    treeManager.add<MAryRandomInsertionTree>("m-ary-insertion-tree");
    treeManager.add<RandomInsertionTree>("insertion-tree");
    treeManager.add<ChainedList>("chained-list");
    treeManager.add<RandomConnectionTree>("random-connection-tree");
    exchangeManager.add<IIDExchangeDistribution>("iid",realManager);
    std::span graph_params(distribution_arguments),exchange_params(exchange_arguments),allowed_money_params(allowed_money_arguments);
    auto d_graph=setup_distribution(graph_params,graphManager);
    auto d_exchange= setup_distribution(exchange_params,exchangeManager);
    auto d_allowed_money= setup_distribution(allowed_money_params,realManager);
    auto G=d_graph->sample();
    auto EG=d_exchange->sample(G);
    auto M=std::llround(d_allowed_money->sample());
    print(std::cout, EG,M);
    std::ofstream file("out.txt");
    print(file,EG,M);
    file.flush();
    return 0;
}

