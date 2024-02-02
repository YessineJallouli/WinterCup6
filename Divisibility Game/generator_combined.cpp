
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
//
// Created by ramizouari on 24/10/23.
//

#ifndef CPLIBRARY_TREE_H
#define CPLIBRARY_TREE_H

#include <optional>
#include <stdexcept>
//
// Created by ramizouari on 21/11/22.
//

#ifndef CPLIBRARY_GRAPH_H
#define CPLIBRARY_GRAPH_H

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <span>
//
// Created by ramizouari on 21/11/22.
//

#ifndef CPLIBRARY_UNION_FIND_H
#define CPLIBRARY_UNION_FIND_H
#include <vector>
#include <map>
#include <unordered_map>
namespace cp
{
    class UnionFind
    {
        int n;
        std::vector<int> parent,rank;
    public:
        UnionFind(int n):n(n),rank(n),parent(n)
        {
            for(int i=0;i<n;i++)
                parent[i]=i;
        }

        void connect(int a,int b)
        {
            auto u= representative(a),v= representative(b);
            if(u==v)
                return;
            if(rank[u]<rank[v])
                parent[u]=parent[v];
            else if(rank[v]<rank[u])
                parent[v]=parent[u];
            else
            {
                parent[u]=parent[v];
                rank[v]++;
            }
        }

        int representative(int a)
        {
            if(parent[a]==a)
                return a;
            else return parent[a]= representative(parent[a]);
        }

        bool equivalent(int a,int b)
        {
            return representative(a)== representative(b);
        }
    };

    template<typename OrderedSet>
    class OrderedUnionFind
    {
        struct ParentData
        {
            const OrderedSet* parent;
            int rank;
        };
        std::map<OrderedSet,ParentData> data{};
    public:
        void connect(const OrderedSet& A,const OrderedSet&B)
        {
            if(equivalent(A,B))
                return;
            auto &[C,NodeData1]=get(representative(A));
            auto &[D,NodeData2]=get(representative(B));
            auto &[p1,r1]=NodeData1;
            auto &[p2,r2]=NodeData2;
            if(r1<r2)
                p1=p2;
            else if(r1>r2)
                p2=p1;
            else
            {
                p1=p2;
                r2++;
            }
        }
        std::pair<const OrderedSet,ParentData>& get(const OrderedSet &A)
        {
            if(!data.contains(A))
            {
                auto [it,_]=data.emplace(A,ParentData{nullptr,0});
                auto &[B,NodeData]=*it;
                NodeData.parent=&B;
                return *it;
            }
            return *data.find(A);
        }
        const OrderedSet& representative(const OrderedSet&A)
        {
            auto &[B,NodeData]=get(A);
            auto &C=NodeData.parent;
            if(&B==C)
                return B;
            else {
                NodeData.parent = &representative(*NodeData.parent);
                return *NodeData.parent;
            }
        }

        bool equivalent(const OrderedSet&A,const OrderedSet&B)
        {
            return &representative(get(A).first)== &representative(get(B).first);
        }

    };

    template<typename UnorderedSet>
    class UnorderedUnionFind
    {
        struct ParentData
        {
            const UnorderedSet* parent;
            int rank;
        };
        std::unordered_map<UnorderedSet,ParentData> data{};
    public:
        void connect(const UnorderedSet& A,const UnorderedSet&B)
        {
            if(equivalent(A,B))
                return;
            auto &[C,NodeData1]=get(representative(A));
            auto &[D,NodeData2]=get(representative(B));
            auto &[p1,r1]=NodeData1;
            auto &[p2,r2]=NodeData2;
            if(r1<r2)
                p1=p2;
            else if(r1>r2)
                p2=p1;
            else
            {
                p1=p2;
                r2++;
            }
        }
        std::pair<const UnorderedSet,ParentData>& get(const UnorderedSet &A)
        {
            if(!data.contains(A))
            {
                auto [it,_]=data.emplace(A,ParentData{nullptr,0});
                auto &[B,NodeData]=*it;
                NodeData.parent=&B;
                return *it;
            }
            return *data.find(A);
        }
        const UnorderedSet& representative(const UnorderedSet&A)
        {
            auto &[B,NodeData]=get(A);
            auto &C=NodeData.parent;
            if(&B==C)
                return B;
            else {
                NodeData.parent = &representative(*NodeData.parent);
                return *NodeData.parent;
            }
        }

        bool equivalent(const UnorderedSet&A,const UnorderedSet&B)
        {
            return &representative(get(A).first)== &representative(get(B).first);
        }

    };
}


#endif //CPLIBRARY_UNION_FIND_H
//
// Created by ramizouari on 27/10/23.
//

#ifndef CPLIBRARY_GENERAL_H
#define CPLIBRARY_GENERAL_H
#include <span>
#include <variant>
#include <vector>
namespace cp
{
    template<typename T>
    struct view_or_value
    {
        std::variant<std::vector<T>,std::span<const T>> value;
        view_or_value(std::vector<T> _value):value(std::move(_value)){}
        view_or_value(std::span<const T> _value):value(std::move(_value)){}
        std::span<const T> get() const
        {
            if(value.index()==0) return std::span<const T>(std::get<0>(value).data(),std::get<0>(value).size());
            return std::get<1>(value);
        }
        operator std::span<const T>() const
        {
            return get();
        }

        auto begin() const
        {
            return get().begin();
        }
        auto end() const
        {
            return get().end();
        }

    };



    namespace graph
    {
        template<typename T>
        struct AbstractGraph
        {
            [[nodiscard]] virtual int size() const = 0;
            virtual view_or_value<T> adjacentNodes(const T&u, bool direction) const =0;
            virtual view_or_value<T> adjacentNodes(const T&u) const =0;
            virtual view_or_value<T> nodes() const =0;
        };

        template<typename T,typename Weight>
        struct AbstractWeightedGraph
        {
            [[nodiscard]] virtual int size() const = 0;
            using AdjacentType=std::pair<T,Weight>;
            virtual view_or_value<AdjacentType> adjacentNodes(const T&u, bool direction) const =0;
            virtual view_or_value<AdjacentType> adjacentNodes(const T&u) const =0;
            virtual view_or_value<T> nodes() const =0;
        };
    }

}
#endif //CPLIBRARY_GENERAL_H

/**
 * @brief This the class of directed Graphs
 * @details Each Graph is a couple G=(V,E) where V={0,...,n-1} are nodes, and E is a subset of VxV
 * @param n the size of the Graph
 * */

namespace cp::graph
{
    struct Graph : public AbstractGraph<int>
    {
        int n;
        std::vector<std::vector<int>> adjacencyList,reverseList;
    protected:

        void topologicalSort(int r,std::vector<int> &L,std::vector<bool> &visited)
        {
            if(!visited[r])
            {
                visited[r]=true;
                for(auto s:adjacencyList[r])
                    topologicalSort(s,L,visited);
                L.push_back(r);
            }
        }

        void assignComponents(int a,int u,UnionFind& C,std::vector<bool> &visited)
        {
            if(!visited[a])
            {
                visited[a]=true;
                C.connect(a,u);
                for(auto s:reverseList[a])
                    assignComponents(s,u,C,visited);
            }
        }

    public:
        Graph(int _n):n(_n),adjacencyList(n),reverseList(n){}
        void connect(int a,int b)
        {
            adjacencyList[a].emplace_back(b);
            reverseList[b].emplace_back(a);
        }

        std::vector<int> topologicalSort()
        {
            std::vector<bool> visited(n);
            std::vector<int> L;
            for(int i=0;i<n;i++)
                topologicalSort(i,L,visited);
            std::reverse(L.begin(),L.end());
            return L;
        }

        struct ConnectedComponentMetaData
        {
            std::vector<std::vector<int>> components;
            std::vector<int> componentId;
            UnionFind classes;
            std::vector<int> topologicalOrder;
            ConnectedComponentMetaData(std::vector<std::vector<int>> && A,std::vector<int> &&B, UnionFind&&C, std::vector<int> &&D):components(std::move(A)),
                                                                                                                                    componentId(std::move(B)), classes(std::move(C)),topologicalOrder(std::move(D)){}
        };


        ConnectedComponentMetaData getConnectedComponentsWithMetaData()
        {

            std::vector<bool> componentAssigned(n);
            UnionFind C(n);
            std::vector<int> topologicalOrder;
            for(auto l: topologicalSort())
            {
                assignComponents(l, l, C, componentAssigned);
                if(l==C.representative(l))
                    topologicalOrder.push_back(l);
            }
            std::vector<bool> idAssigned(n);
            std::vector<int> componentId(n);
            std::vector<std::vector<int>> components;
            for(int i=0;i<n;i++)
            {
                auto r=C.representative(i);
                if(!idAssigned[r])
                {
                    idAssigned[r]=true;
                    componentId[r]=components.size();
                    components.emplace_back();
                }
                componentId[i]=componentId[r];
                components[componentId[r]].push_back(i);
            }
            return ConnectedComponentMetaData(std::move(components),std::move(componentId),std::move(C),std::move(topologicalOrder));
        }

        std::pair<Graph,std::vector<int>> condensationGraph()
        {
            auto [components,componentId,classes,topologicalOrder]=getConnectedComponentsWithMetaData();
            Graph DAG(components.size());
            std::vector<std::set<int>> S(components.size());
            for(int i=0;i<n;i++) for(auto j:adjacencyList[i]) if(!classes.equivalent(i,j) && !S[componentId[i]].contains(componentId[j]))
            {
                auto u=componentId[i],v=componentId[j];
                S[u].insert(v);
                DAG.connect(u,v);
            }
            return std::make_pair(DAG,componentId);
        }

        std::vector<std::vector<int>> getConnectedComponents()
        {
            return getConnectedComponentsWithMetaData().components;
        }

        int size() const override
        {
            return n;
        }

        view_or_value<int> adjacentNodes(const int&u, bool direction) const override
        {
            if(direction)
                return std::span<const int>(adjacencyList[u].data(),adjacencyList[u].size());
            return std::span<const int>(reverseList[u].data(),reverseList[u].size());
        }

        view_or_value<int> adjacentNodes(const int&u) const override
        {
            return adjacentNodes(u,true);
        }

        view_or_value<int> nodes() const override
        {
            std::vector<int> A(n);
            for(int i=0;i<n;i++)
                A[i]=i;
            return A;
        }
    };

    class PlanarGraph
    {
        int n,m;
        using couple=std::pair<int,int>;
        std::vector<std::vector<std::vector<couple>>> adjacencyList,reverseList;
    public:
        PlanarGraph(int n,int m):n(n),m(m),adjacencyList(n,std::vector<std::vector<couple>>(m)),reverseList(n,std::vector<std::vector<couple>>(m)){}
        void connect(int u1,int v1,int u2,int v2)
        {
            adjacencyList[u1][v1].emplace_back(u2,v2);
        }
    };


/**
 * @brief This the class of directed Graphs over an ordered set
 * @details Each Graph is a couple G=(V,E) where V is an ordered set
 * @tparam OrderedSet The ordered set V
 * */
    template<typename OrderedSet>
    class OrderedGraph
    {
        std::map<OrderedSet,std::vector<OrderedSet>> adjacencyList,reverseList;
        std::set<OrderedSet> states;

        void topologicalSort(const OrderedSet& r,std::vector<OrderedSet> &L,std::set<OrderedSet> &visited)
        {
            if(!visited.contains(r))
            {
                visited.insert(r);
                for(const auto& s:adjacencyList[r])
                    topologicalSort(s,L,visited);
                L.push_back(r);
            }
        }

        void assignComponents(const OrderedSet& a,const OrderedSet& u,OrderedUnionFind<OrderedSet>& C,std::set<OrderedSet> &visited)
        {
            if(!visited.contains(a))
            {
                visited.insert(a);
                C.connect(a,u);
                for(auto s:reverseList[a])
                    assignComponents(s,u,C,visited);
            }
        }

    public:
        void connect(const OrderedSet&A,const OrderedSet&B)
        {
            if(!states.contains(A))
                states.insert(A);
            if(!states.contains(B))
                states.insert(B);
            adjacencyList[A].push_back(B);
            reverseList[B].push_back(A);
        }

        void addState(const OrderedSet&A)
        {
            states.insert(A);
        }

        std::vector<OrderedSet> topologicalSort()
        {
            std::set<OrderedSet> visited;
            std::vector<OrderedSet> L;
            for(auto &u:states)
                topologicalSort(u,L,visited);
            std::reverse(L.begin(),L.end());
            return L;
        }

        struct ConnectedComponentMetaData
        {
            std::vector<std::vector<OrderedSet>> components;
            std::vector<OrderedSet> componentId;
            OrderedUnionFind<OrderedSet> classes;
            std::vector<OrderedSet> topologicalOrder;
            ConnectedComponentMetaData(std::vector<std::vector<OrderedSet>> && A,std::vector<OrderedSet> &&B, OrderedUnionFind<OrderedSet>&&C,
                                       std::vector<OrderedSet>&&topologicalOrder):
                    components(std::move(A)),componentId(std::move(B)), classes(std::move(C)),topologicalOrder(std::move(topologicalOrder)){}
        };

        ConnectedComponentMetaData getConnectedComponentsWithMetaData()
        {
            std::set<OrderedSet> componentAssigned;
            OrderedUnionFind<OrderedSet> C;
            std::vector<OrderedSet> topologicalOrder;
            for(auto l: topologicalSort()) {
                assignComponents(l, l, C, componentAssigned);
                if(l==C.representative(l))
                    topologicalOrder.push_back(l);
            }
            std::set<OrderedSet> idAssigned;
            std::map<OrderedSet,int> componentId;
            std::vector<std::vector<OrderedSet>> components;
            for(auto &u:states)
            {
                auto r=C.representative(u);
                if(!idAssigned.contains(r))
                {
                    idAssigned.insert(r);
                    componentId[r]=components.size();
                    components.emplace_back();
                }
                components[componentId[r]].push_back(u);
            }
            return ConnectedComponentMetaData(std::move(components),std::move(componentId),std::move(C),std::move(topologicalOrder));
        }

        std::pair<OrderedGraph,OrderedUnionFind<OrderedSet>> condensationGraph()
        {
            auto [components,componentId,classes,topologicalOrder]=getConnectedComponentsWithMetaData();
            OrderedGraph DAG(components.size());
            for(auto [u,v]:adjacencyList) if(!classes.equivalent(u,v))
                    DAG.connect(classes.representative(u),classes.representative(v));
            return std::make_pair(DAG,classes);
        }

        std::vector<std::vector<OrderedSet>> getConnectedComponents()
        {
            return getConnectedComponentsWithMetaData().components;
        }
    };


/**
 * @brief This the class of directed Graphs over a hashable set
 * @details Each Graph is a couple G=(V,E) where V is a hashable set
 * @tparam UnorderedSet The hashable set V
 * */
    template<typename UnorderedSet>
    class UnorderedGraph
    {
        std::unordered_map<UnorderedSet,std::vector<UnorderedSet>> adjacencyList,reverseList;
        std::set<UnorderedSet> states;

        void topologicalSort(const UnorderedSet& r,std::vector<UnorderedSet> &L,std::unordered_set<UnorderedSet> &visited)
        {
            if(!visited.contains(r))
            {
                visited.insert(r);
                for(const auto& s:adjacencyList[r])
                    topologicalSort(s,L,visited);
                L.push_back(r);
            }
        }

        void assignComponents(const UnorderedSet& a,const UnorderedSet& u,UnorderedUnionFind<UnorderedSet>& C,std::unordered_set<UnorderedSet> &visited)
        {
            if(!visited.contains(a))
            {
                visited.insert(a);
                C.connect(a,u);
                for(auto s:reverseList[a])
                    assignComponents(s,u,C,visited);
            }
        }

    public:
        void connect(const UnorderedSet&A,const UnorderedSet&B)
        {
            if(!states.contains(A))
                states.insert(A);
            if(!states.contains(B))
                states.insert(B);
            adjacencyList[A].push_back(B);
            reverseList[B].push_back(A);
        }

        void addState(const UnorderedSet&A)
        {
            states.insert(A);
        }

        std::vector<UnorderedSet> topologicalSort()
        {
            std::unordered_set<UnorderedSet> visited;
            std::vector<UnorderedSet> L;
            for(auto &u:states)
                topologicalSort(u,L,visited);
            std::reverse(L.begin(),L.end());
            return L;
        }

        struct ConnectedComponentMetaData
        {
            std::vector<std::vector<UnorderedSet>> components;
            std::vector<UnorderedSet> componentId;
            UnorderedUnionFind<UnorderedSet> classes;
            std::vector<UnorderedSet> topologicalOrder;
            ConnectedComponentMetaData(std::vector<std::vector<UnorderedSet>> && A,std::vector<UnorderedSet> &&B, OrderedUnionFind<UnorderedSet>&&C,
                                       std::vector<UnorderedSet> &&D):
                    components(std::move(A)),componentId(std::move(B)), classes(std::move(C)),
                    topologicalOrder(std::move(D)){}
        };

        ConnectedComponentMetaData getConnectedComponentsWithMetaData()
        {
            std::set<UnorderedSet> componentAssigned;
            UnorderedUnionFind<UnorderedSet> C;
            std::vector<UnorderedSet> topologicalOrder;
            for(auto l: topologicalSort()) {
                assignComponents(l, l, C, componentAssigned);
                if(l==C.representative(l))
                    topologicalOrder.push_back(l);
            }
            std::unordered_set<UnorderedSet> idAssigned;
            std::unordered_map<UnorderedSet,int> componentId;
            std::vector<std::vector<UnorderedSet>> components;
            for(auto &u:states)
            {
                auto r=C.representative(u);
                if(!idAssigned.contains(r))
                {
                    idAssigned.insert(r);
                    componentId[r]=components.size();
                    components.emplace_back();
                }
                components[componentId[r]].push_back(u);
            }
            return ConnectedComponentMetaData(std::move(components),std::move(componentId),std::move(C),std::move(topologicalOrder));
        }

        std::pair<UnorderedGraph,UnorderedUnionFind<UnorderedSet>> condensationGraph()
        {
            auto [components,componentId,classes,topologicalOrder]=getConnectedComponentsWithMetaData();
            UnorderedGraph DAG(components.size());
            for(auto [u,v]:adjacencyList) if(!classes.equivalent(u,v))
                    DAG.connect(classes.representative(u),classes.representative(v));
            return std::make_pair(DAG,classes);
        }

        std::vector<std::vector<UnorderedSet>> getConnectedComponents()
        {
            return getConnectedComponentsWithMetaData().components;
        }
    };


/**
 * @brief This the class of directed Graphs
 * @details Each Graph is a couple G=(V,E) where V={0,...,n-1} are nodes, and E is a subset of VxV
 * @param n the size of the Graph
 * */
    template<typename Weight>
    struct WeightedGraph : public AbstractWeightedGraph<int,Weight>
    {
        int n;
        using AdjacentType=std::pair<int,Weight>;
        std::vector<std::vector<AdjacentType>> adjacencyList,reverseList;
    protected:

        void topologicalSort(int r,std::vector<int> &L,std::vector<bool> &visited)
        {
            if(!visited[r])
            {
                visited[r]=true;
                for(auto [s,_]:adjacencyList[r])
                    topologicalSort(s,L,visited);
                L.push_back(r);
            }
        }

        void assignComponents(int a,int u,UnionFind& C,std::vector<bool> &visited)
        {
            if(!visited[a])
            {
                visited[a]=true;
                C.connect(a,u);
                for(const auto& [s,_]:reverseList[a])
                    assignComponents(s,u,C,visited);
            }
        }

    public:
        explicit WeightedGraph(int _n):n(_n),adjacencyList(n),reverseList(n){}
        void connect(int a,int b, const Weight & w)
        {
            adjacencyList[a].emplace_back(b,w);
            reverseList[b].emplace_back(a,w);
        }

        std::vector<int> topologicalSort()
        {
            std::vector<bool> visited(n);
            std::vector<int> L;
            for(int i=0;i<n;i++)
                topologicalSort(i,L,visited);
            std::reverse(L.begin(),L.end());
            return L;
        }

        struct ConnectedComponentMetaData
        {
            std::vector<std::vector<int>> components;
            std::vector<int> componentId;
            UnionFind classes;
            std::vector<int> topologicalOrder;
            ConnectedComponentMetaData(std::vector<std::vector<int>> && A,std::vector<int> &&B, UnionFind&&C, std::vector<int> &&D):components(std::move(A)),
                                                                                                                                    componentId(std::move(B)), classes(std::move(C)),topologicalOrder(std::move(D)){}
        };


        ConnectedComponentMetaData getConnectedComponentsWithMetaData()
        {

            std::vector<bool> componentAssigned(n);
            UnionFind C(n);
            std::vector<int> topologicalOrder;
            for(auto l: topologicalSort())
            {
                assignComponents(l, l, C, componentAssigned);
                if(l==C.representative(l))
                    topologicalOrder.push_back(l);
            }
            std::vector<bool> idAssigned(n);
            std::vector<int> componentId(n);
            std::vector<std::vector<int>> components;
            for(int i=0;i<n;i++)
            {
                auto r=C.representative(i);
                if(!idAssigned[r])
                {
                    idAssigned[r]=true;
                    componentId[r]=components.size();
                    components.emplace_back();
                }
                componentId[i]=componentId[r];
                components[componentId[r]].push_back(i);
            }
            return ConnectedComponentMetaData(std::move(components),std::move(componentId),std::move(C),std::move(topologicalOrder));
        }

        std::pair<Graph,std::vector<int>> condensationGraph()
        {
            auto [components,componentId,classes,topologicalOrder]=getConnectedComponentsWithMetaData();
            Graph DAG(components.size());
            std::vector<std::set<int>> S(components.size());
            for(int i=0;i<n;i++) for(auto j:adjacencyList[i])
                    if(!classes.equivalent(i,j) && !S[componentId[i]].contains(componentId[j]))
                    {
                        auto u=componentId[i],v=componentId[j];
                        S[u].insert(v);
                        DAG.connect(u,v);
                    }
            return std::make_pair(DAG,componentId);
        }

        std::vector<std::vector<int>> getConnectedComponents()
        {
            return getConnectedComponentsWithMetaData().components;
        }

        int size() const override
        {
            return n;
        }

        view_or_value<AdjacentType> adjacentNodes(const int&u, bool direction) const override
        {
            if(direction)
                return std::span<const AdjacentType>(adjacencyList[u].data(),adjacencyList[u].size());
            return std::span<const AdjacentType>(reverseList[u].data(),reverseList[u].size());
        }

        view_or_value<AdjacentType> adjacentNodes(const int&u) const override
        {
            return adjacentNodes(u,true);
        }

        view_or_value<int> nodes() const override
        {
            std::vector<int> A(n);
            for(int i=0;i<n;i++)
                A[i]=i;
            return A;
        }
    };

}

#endif //CPLIBRARY_GRAPH_H
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_DYN_SPARSE_ARRAY_H
#define CPLIBRARY_DYN_SPARSE_ARRAY_H
#include <vector>
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
#include <memory>
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

namespace cp::data_structures::dynamic
{
    template<typename T>
    struct sparse_array
    {
        int n,h;
        std::vector<std::vector<T>> S;
        binary_operation_ptr<T> F;
    public:
        sparse_array(const std::vector<T>&A, std::shared_ptr<binary_operation<T>> _F):n(bit_ceil(A.size())),h(bit_log(n)),S(h+1),F(_F)
        {
            int r=1;
            for(int i=h;i>=0;i--,r*=2)
                S[i].resize(n-r+1,F.neutral_element());
            for(int i=0;i<A.size();i++)
                S[h][i]=A[i];
            r=1;
            for(int i=h-1;i>=0;i--,r*=2) for(int j=0;j<=n-2*r;j++)
                    S[i][j]=F(S[i+1][j],S[i+1][j+r]);
        }

        T query(int l,int r) const
        {
            if(l>=r)
                return F.neutral_element();
            auto d=r-l;
            auto s=bit_floor(d);
            auto b=bit_log(s);
            return F(S[h-b][l],S[h-b][r-s]);
        }
    };
}

#endif //CPLIBRARY_SPARSE_ARRAY_H
//
// Created by ramizouari on 26/10/23.
//

#ifndef CPLIBRARY_FIXED_SPARSE_ARRAY_H
#define CPLIBRARY_FIXED_SPARSE_ARRAY_H
#include <vector>
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
#include <memory>
#include <queue>
#include <random>
#include <chrono>

namespace cp::graph
{

    namespace ds= data_structures;

    struct HLDIndex
    {
        int hld_id;
        int index;
        HLDIndex(int _hld_id, int _index): hld_id(_hld_id), index(_index){}
    };

    template<typename Weight>
    struct HeavyLightDecomposition
    {
        std::vector<bool> is_heavy;
        std::vector<std::pair<int,int>> heavy_path_endpoints;
        std::vector<int> component_size;
        std::vector<HLDIndex> HLD_mapper;
        std::vector<std::vector<Weight>> components;
    };

    template<>
    struct HeavyLightDecomposition<void>
    {
        std::vector<bool> is_heavy;
        std::vector<std::pair<int,int>> heavy_path_endpoints;
        std::vector<int> component_size;
        std::vector<HLDIndex> HLD_mapper;
    };

    enum class TreeStats
    {
        NONE=0,
        SIZE=0b00001,
        HEAVY_EDGES=0b00011,
        LCA=0b00100,
        HLD=0b01111,
        RANGE_QUERIES=0b11111,
    };
    bool operator &(TreeStats a,TreeStats b)
    {
        return static_cast<int>(a) & static_cast<int>(b);
    }

    struct Tree : Graph
    {

        bool reversed=false;
        std::vector<int> subtree_size;
        std::vector<std::optional<int>> parent;
        int root;
        HeavyLightDecomposition<void> HLD;
        Tree(int n,int _root):Graph(n),root(_root),subtree_size(n),parent(n)
        {
            HLD.is_heavy.resize(n,false);
        }

        Tree(const Graph &G): Graph(G),root(G.n),subtree_size(G.n),parent(G.n)
        {
            HLD.is_heavy.resize(G.n,false);
        }

        Tree(Graph &&G): Graph(std::move(G)),root(n),subtree_size(n),parent(n)
        {
            HLD.is_heavy.resize(n,false);
        }

        explicit Tree(int n):Tree(n,0){}

        void setParent(int u,int v)
        {
            if(reversed) std::swap(u,v);
            this->connect(u,v);
            parent[u].emplace(v);
        }

        std::vector<int> &children(int u)
        {
            return reversed?this->adjacencyList[u] : this->reverseList[u] ;
        }

        [[nodiscard]] const std::vector<int> &children(int u) const
        {
            return reversed?this->adjacencyList[u] : this->reverseList[u] ;
        }

        void buildStatistics(TreeStats stats=TreeStats::HLD)
        {
            updateRoot();
            if(stats & TreeStats::SIZE) updateSize(root);
            //TODO: Optimize heavy edges
            if(stats & TreeStats::HEAVY_EDGES) updateHeavyEdges(root);
            if(stats & TreeStats::LCA) buildLCA();
            auto t1=std::chrono::high_resolution_clock::now();
            //TODO: Optimize HLD
            if(stats & TreeStats::HLD) buildHeavyLightDecomposition();
        }

        void adjacentReRoot(int new_root)
        {
            if(parent[new_root] != root)
                throw std::invalid_argument("new root must be adjacent to old root");
            auto u=*parent[new_root];
            parent[new_root]=std::nullopt;
            parent[root].emplace(new_root);
            auto delta=subtree_size[new_root];
            subtree_size[new_root]=subtree_size[root];
            subtree_size[root]-=delta;
            root=new_root;
        }

        void reRoot(int new_root)
        {
            std::queue<int> Q;
            std::vector<bool> visited(n);
            Q.push(new_root);
            visited[new_root]=true;
            std::vector<std::vector<int>> newAdjacencyList(n),newReverseList(n);
            while(!Q.empty())
            {
                auto u=Q.front();
                Q.pop();
                for(auto v:this->adjacencyList[u]) if(!visited[v])
                    {
                        visited[v]=true;
                        Q.push(v);
                        newReverseList[u].emplace_back(v);
                        newAdjacencyList[v].emplace_back(u);
                    }
                for(auto v:this->reverseList[u]) if(!visited[v])
                    {
                        visited[v]=true;
                        Q.push(v);
                        newReverseList[u].emplace_back(v);
                        newAdjacencyList[v].emplace_back(u);
                    }
            }
            this->adjacencyList=std::move(newAdjacencyList);
            this->reverseList=std::move(newReverseList);
            updateRoot();
        }

        int leastCommonAncestor(int u,int v)
        {
            if(lca_data)
            {
                auto [a,b]=euler_tour_endpoints[u];
                auto [c,d]=euler_tour_endpoints[v];
                if(a>c)
                {
                    std::swap(a,c);
                    std::swap(b,d);
                }
                if(b<d)
                    return lca_data->query(a,c).second;
                else
                    return lca_data->query(a,d).second;
            }
            else
            {
                while(u!=v)
                {
                    if(subtree_size[u]>subtree_size[v])
                        u=*parent[u];
                    else
                        v=*parent[v];
                }
                return u;
            }
        }
        void updateSize(int u)
        {
            subtree_size[u]=1;
            for(auto v:children(u))
            {
                updateSize(v);
                subtree_size[u]+=subtree_size[v];
            }
        }

        void updateRoot()
        {
            for(int i=0;i<n;i++)
                parent[i]=this->adjacencyList[i].empty()?std::nullopt:std::make_optional(this->adjacencyList[i][0]);
            for(int i=0;i< n;i++)
                if(!parent[i])
                    root=i;
        }

        void updateHeavyEdges(int u)
        {
            for(int i=0;i<children(u).size();i++)
            {
                auto v=children(u)[i];
                if(subtree_size[v]>=(subtree_size[u]+1)/2)
                    HLD.is_heavy[v]=true;
                updateHeavyEdges(v);
            }
        }
        void buildLCA()
        {
            std::vector<HeightData> A;
            euler_tour_endpoints.resize(n);
            eulerTour(root,0,A);
            min_t<HeightData>::neutral.first=std::numeric_limits<int>::max();
            lca_data=std::make_unique<ds::fixed::sparse_array<min_t<HeightData>>>(A);
        }

        void buildHeavyLightDecomposition()
        {
            std::vector<int> stack;
            stack.push_back(root);
            HLD.HLD_mapper.resize(n, HLDIndex(-1, -1));
            HLD.HLD_mapper[root]={0, 0};
            HLD.heavy_path_endpoints.emplace_back(root,root);
            int components=1;
            while(!stack.empty())
            {
                auto u=stack.back();
                stack.pop_back();
                for(auto v:children(u))
                {
                    if(HLD.is_heavy[v])
                    {
                        auto &[_,y] = HLD.heavy_path_endpoints[HLD.HLD_mapper[u].hld_id];
                        stack.push_back(v);
                        HLD.HLD_mapper[v]={HLD.HLD_mapper[u].hld_id, HLD.HLD_mapper[u].index + 1};
                        y=v;
                    }
                    else
                    {
                        HLD.heavy_path_endpoints.emplace_back(v,v);
                        stack.push_back(v);
                        HLD.HLD_mapper[v]=HLDIndex(components, 0);
                        components++;
                    }
                }
            }
        }

        int distance(int a,int b)
        {
            auto lca=leastCommonAncestor(a,b);
            return distance_with_lca(a,lca)+distance_with_lca(b,lca);
        }

        int distance_with_lca(int u,int lca)
        {
            int d=0;
            while(HLD.HLD_mapper[u].hld_id != HLD.HLD_mapper[lca].hld_id)
            {
                if(HLD.is_heavy[u])
                {
                    d+=HLD.HLD_mapper[u].index;
                    u=HLD.heavy_path_endpoints[HLD.HLD_mapper[u].hld_id].first;
                }
                else
                {
                    d++;
                    u=*parent[u];
                }
            }
            return d+= HLD.HLD_mapper[u].index - HLD.HLD_mapper[lca].index;
        }

        int centroid()
        {
            reRoot(centroid(root,root));
            return root;
        }

    protected:
        int centroid(int u,std::optional<int> p)
        {
            for(auto v:children(u)) if(v!= p && subtree_size[v]>=(subtree_size[u]+1)/2)
                {
                    adjacentReRoot(v);
                    return centroid(v,u);
                }
            return u;
        }
        using HeightData=std::pair<int,int>;
        using EnpointsData = std::pair<int,int>;
        std::unique_ptr<ds::fixed::sparse_array<min_t<HeightData>>> lca_data;
        std::vector<EnpointsData> euler_tour_endpoints;
        void eulerTour(int u,int height,std::vector<HeightData> &A)
        {
            euler_tour_endpoints[u].first=A.size();
            for(auto v: children(u))
            {
                A.emplace_back(height,u);
                eulerTour(v,height+1,A);
            }
            A.emplace_back(height,u);
            euler_tour_endpoints[u].second=A.size();
        }
    };

    template<typename Weight>
    struct WeightedTree : public WeightedGraph<Weight>
    {

        bool reversed=false;
        std::vector<int> subtree_size;
        using AdjacentType=WeightedGraph<Weight>::AdjacentType;
        std::vector<std::optional<AdjacentType>> parent;
        int root;
        HeavyLightDecomposition<Weight> HLD;
        WeightedTree(int n,int _root):WeightedGraph<Weight>(n),root(_root),subtree_size(n),parent(n)
        {
            HLD.is_heavy.resize(n);
        }
        explicit WeightedTree(int n):WeightedTree(n,0){}

        void setParent(int u,int v,Weight w)
        {
            if(reversed) std::swap(u,v);
            this->connect(u,v,w);
            parent[u].emplace(v,w);
        }

        std::vector<AdjacentType> &children(int u)
        {
            return reversed?this->adjacencyList[u] : this->reverseList[u] ;
        }

        void buildStatistics(TreeStats stats=TreeStats::HLD)
        {
            updateRoot();
            if(stats & TreeStats::SIZE) updateSize(root);
            if(stats & TreeStats::HEAVY_EDGES) updateHeavyEdges(root);
            if(stats & TreeStats::LCA) buildLCA();
            if(stats & TreeStats::HLD) buildHeavyLightDecomposition();
        }

        void adjacentReRoot(int new_root)
        {
            if(!parent[new_root] || parent[new_root]->first != root)
                throw std::invalid_argument("new root must be adjacent to old root");
            auto [u,w]=*parent[new_root];
            parent[new_root]=std::nullopt;
            parent[root].emplace(new_root,w);
            auto delta=subtree_size[new_root];
            subtree_size[new_root]=subtree_size[root];
            subtree_size[root]-=delta;
        }

        void reRoot(int new_root)
        {
            std::queue<int> Q;
            std::vector<bool> visited(WeightedGraph<Weight>::n);
            Q.emplace(new_root);
            visited[new_root]=true;
            std::vector<std::vector<AdjacentType>> newAdjacencyList(WeightedGraph<Weight>::n),newReverseList(WeightedGraph<Weight>::n);
            while(!Q.empty())
            {
                auto u=Q.front();
                Q.pop();
                for(auto [v,w]:this->adjacencyList[u]) if(!visited[v])
                    {
                        visited[v]=true;
                        Q.emplace(v);
                        newReverseList[u].emplace_back(v,w);
                        newAdjacencyList[v].emplace_back(u,w);
                    }
                for(auto [v,w]:this->reverseList[u]) if(!visited[v])
                    {
                        visited[v]=true;
                        Q.emplace(v);
                        newReverseList[u].emplace_back(v,w);
                        newAdjacencyList[v].emplace_back(u,w);
                    }
            }
            this->adjacencyList=std::move(newAdjacencyList);
            this->reverseList=std::move(newReverseList);
            updateRoot();
        }

        int leastCommonAncestor(int u,int v)
        {
            if(lca_data)
            {
                auto [a,b]=euler_tour_endpoints[u];
                auto [c,d]=euler_tour_endpoints[v];
                if(a>c)
                {
                    std::swap(a,c);
                    std::swap(b,d);
                }
                if(b<d)
                    return lca_data->query(a,c).second;
                else
                    return lca_data->query(a,d).second;
            }
            else
            {
                while(u!=v)
                {
                    if(subtree_size[u]>subtree_size[v])
                        u=parent[u]->first;
                    else
                        v=parent[v]->first;
                }
                return u;
            }
        }
        void updateSize(int u)
        {
            subtree_size[u]=1;
            for(auto [v,_]:children(u))
            {
                updateSize(v);
                subtree_size[u]+=subtree_size[v];
            }
        }

        void updateRoot()
        {
            for(int i=0;i<WeightedGraph<Weight>::n;i++)
                parent[i]=this->adjacencyList[i].empty()?std::nullopt:std::make_optional(this->adjacencyList[i][0]);
            for(int i=0;i<WeightedGraph<Weight>::n;i++)
                if(!parent[i])
                    root=i;
        }

        void updateHeavyEdges(int u)
        {
            for(int i=0;i<children(u).size();i++)
            {
                auto [v,_]=children(u)[i];
                if(subtree_size[v]>=(subtree_size[u]+1)/2)
                    HLD.is_heavy[v]=true;
                updateHeavyEdges(v);
            }
        }
        void buildLCA()
        {
            std::vector<HeightData> A;
            euler_tour_endpoints.resize(WeightedGraph<Weight>::n);
            eulerTour(root,0,A);
            min_t<HeightData>::neutral.first=std::numeric_limits<int>::max();
            lca_data=std::make_unique<ds::fixed::sparse_array<min_t<HeightData>>>(A);
        }

        void buildHeavyLightDecomposition()
        {
            std::vector<int> stack;
            stack.push_back(root);
            HLD.HLD_mapper.resize(WeightedTree<Weight>::n, HLDIndex(-1, -1));
            HLD.HLD_mapper[root]={0, 0};
            HLD.components.emplace_back();
            HLD.heavy_path_endpoints.emplace_back(root,root);
            while(!stack.empty())
            {
                auto u=stack.back();
                stack.pop_back();
                for(auto [v,w]:children(u))
                {
                    if(HLD.is_heavy[v])
                    {
                        auto &C=HLD.components[HLD.HLD_mapper[u].hld_id];
                        auto &[_,y] = HLD.heavy_path_endpoints[HLD.HLD_mapper[u].hld_id];
                        stack.push_back(v);
                        HLD.HLD_mapper[v]={HLD.HLD_mapper[u].hld_id, HLD.HLD_mapper[u].index + 1};
                        y=v;
                        C.push_back(w);
                    }
                    else
                    {
                        HLD.heavy_path_endpoints.emplace_back(v,v);
                        stack.push_back(v);
                        HLD.HLD_mapper[v]=HLDIndex(HLD.components.size(), 0);
                        HLD.components.emplace_back();
                    }
                }
            }
            for(const auto &C:HLD.components)
                HLD.component_size.push_back(C.size());
        }

        int distance(int a,int b)
        {
            auto lca=leastCommonAncestor(a,b);
            return distance_with_lca(a,lca)+distance_with_lca(b,lca);
        }

        int distance_with_lca(int u,int lca)
        {
            int d=0;
            while(HLD.HLD_mapper[u].hld_id != HLD.HLD_mapper[lca].hld_id)
            {
                if(WeightedTree<Weight>::HLD.is_heavy[u])
                {
                    d+=HLD.HLD_mapper[u].index;
                    u=HLD.heavy_path_endpoints[HLD.HLD_mapper[u].hld_id].first;
                }
                else
                {
                    d++;
                    u=parent[u]->first;
                }
            }
            return d+= HLD.HLD_mapper[u].index - HLD.HLD_mapper[lca].index;
        }


    protected:
        using HeightData=std::pair<int,int>;
        using EnpointsData = std::pair<int,int>;
        std::unique_ptr<ds::fixed::sparse_array<min_t<HeightData>>> lca_data;
        std::vector<EnpointsData> euler_tour_endpoints;
        void eulerTour(int u,int height,std::vector<HeightData> &A)
        {
            euler_tour_endpoints[u].first=A.size();
            for(auto [v,_]: children(u))
            {
                A.emplace_back(height,u);
                eulerTour(v,height+1,A);
            }
            A.emplace_back(height,u);
            euler_tour_endpoints[u].second=A.size();
        }
    };

}


#endif //CPLIBRARY_TREE_H
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
    std::uint64_t seed;
    std::vector<std::string> arguments;
};

ParsedArguments parse_arguments(int argc,char **argv)
{
    ParsedArguments A;
    if(argc<3)
        throw std::invalid_argument("Not enough arguments");
    A.seed=std::stoull(argv[1]);
    for(int i=2;i<argc && strncmp(argv[i],"##",2);i++)
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

using ArrayDistribution=Distribution<std::vector<cp::integer>>;
using ArrayDistributionManager=DistributionManager<ArrayDistribution>;

struct DivisibilityGameDistribution : public ArrayDistribution
{
    cp::integer n,k;

    void update(std::span<std::string> &params) override
    {
        n=std::stoll(params[0]);
        k=std::stoll(params[1]);
        params = params.subspan(2);
    }
};

struct OneMoveWin : public DivisibilityGameDistribution
{
    std::uniform_int_distribution<cp::integer> d;
    int parameters_count() const
    {
        return 1;
    }
    std::vector<cp::integer> sample() override
    {
        std::vector<cp::integer> A(n);
        A[0]=d(rng);
        A[1]=A[0]?k-A[0]:A[0];
        return A;
    }

    void update(std::span<std::string> &params) override
    {
        DivisibilityGameDistribution::update(params);
        d=std::uniform_int_distribution<cp::integer>(0,k-1);
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<OneMoveWin>(*this);
    }

};

struct RandomDivisibilityArray : public DivisibilityGameDistribution
{
    std::uniform_int_distribution<cp::integer> d;
    int parameters_count() const
    {
        return 1;
    }
    std::vector<cp::integer> sample() override
    {
        std::vector<cp::integer> A(n);
        std::generate(A.begin(),A.end()-1,std::bind(d,rng));
        A.back()=(k-std::reduce(A.begin(),A.end()-1,0,[k=k](auto a,auto b){return (a+b)%k;}))%k;
        return A;
    }

    void update(std::span<std::string> &params) override
    {
        DivisibilityGameDistribution::update(params);
        d=std::uniform_int_distribution<cp::integer>(0,k);
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<RandomDivisibilityArray>(*this);
    }

};

struct RandomArray : public DivisibilityGameDistribution
{
    std::uniform_int_distribution<cp::integer> d;
    cp::integer L;
    int parameters_count() const
    {
        return 1;
    }
    std::vector<cp::integer> sample() override
    {
        std::vector<cp::integer> A(n);
        std::generate(A.begin(),A.end()-1,std::bind(d,rng));
        A.back()=(k-std::reduce(A.begin(),A.end()-1,0,[k=k](auto a,auto b){return (a+b)%k;}))%k;
        return A;
    }

    void update(std::span<std::string> &params) override
    {
        DivisibilityGameDistribution::update(params);
        L=std::stoll(params[0]);
        d=std::uniform_int_distribution<cp::integer>(0,L);
    }

    [[nodiscard]] std::shared_ptr<Cloneable> clone() const override
    {
        return std::make_unique<RandomArray>(*this);
    }

};

int main(int argc, char**argv)
{
    auto [seed, args] = parse_arguments(argc,argv);
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
    arrayManager.add<OneMoveWin>("one-move");
    arrayManager.add<RandomDivisibilityArray>("divisibility-array");
    arrayManager.add<RandomArray>("random-array");
    std::span params(args);
    auto distribution = std::dynamic_pointer_cast<DivisibilityGameDistribution>(setup_distribution(params,arrayManager));
    auto G=distribution->sample();
    std::cout << distribution->n << ' ' << distribution->k << '\n';
    std::shuffle(G.begin(),G.end(),RandomNumberGenerator{});
    for(int i=0;i<distribution->n;i++)
        std::cout << G[i] << (i<distribution->n-1?' ':'\n');
    return 0;
}

