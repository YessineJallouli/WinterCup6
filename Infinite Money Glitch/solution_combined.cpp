#include <iostream>
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
#include <random>
#include <chrono>

using real=double;

struct Conversion
{
    real rate, fee;
};

using GraphType=cp::graph::WeightedGraph<Conversion>;


real bellman_ford(GraphType &G, int s, real x)
{
    std::vector<real> D(G.size(), 0);
    D[s] = x;
    for (int i = 0; i < G.size(); i++) for (int u = 0; u < G.size(); u++) for (auto [v, conversion] : G.adjacencyList[u])
        D[v] = std::max(D[v], (D[u] - conversion.fee) * conversion.rate);
    return D[s];
}

int minimal_investment(GraphType &G, int s,int x)
{
    int a=0,b=x+1,c;
    while(a<b)
    {
        c=(a+b)/2;
        if(bellman_ford(G,s,c)<=c)
            a=c+1;
        else
            b=c;
    }
    return a;
}

real bellman_ford_growth_upper_bound(GraphType &G, int s)
{
    std::vector<double> D1(G.size(), 0),D2(G.size(),0);
    D1[s] = 1;
    for (int i = 0; i < G.size(); i++)
    {
        std::copy(D1.begin(),D1.end(),D2.begin());
        for (int u = 0; u < G.size(); u++)
            for (auto [v, conversion]: G.adjacencyList[u])
                D2[v] = std::max(D2[v], D1[u] * conversion.rate);
        std::swap(D1, D2);
    }
    return D1[s];
}

real bellman_ford_growth_lower_bound(GraphType &G, int s)
{
    std::vector<double> D1(G.size(), std::numeric_limits<double>::infinity()),D2(G.size(), std::numeric_limits<double>::infinity());
    D1[s] = 1;
    for (int i = 0; i < G.size(); i++)
    {
        std::copy(D1.begin(),D1.end(),D2.begin());
        for (int u = 0; u < G.size(); u++)
            for (auto [v, conversion]: G.adjacencyList[u])
                D2[v] = std::min(D2[v], D1[u] * conversion.rate);
        std::swap(D1, D2);
    }
    return D1[s];
}

int main(int argc,char **argv)
{
    int n,m,x;
    std::cin >> n >> m >> x;
    GraphType G(n);
    if(argc < 2) for(int i=0;i<m;i++)
    {
        int a,b;
        Conversion conversion;
        std::cin >> a >> b >> conversion.rate >> conversion.fee;
        a--;
        b--;
        G.connect(a,b,conversion);
    }
    else
    {
        std::uint64_t seed;
        if(std::string_view(argv[1])=="random")
        {
            std::random_device rd;
            seed=rd();
        }
        else
            seed=std::stoull(argv[1]);
        std::mt19937_64 rng(seed);
        std::uniform_real_distribution<real> rate(0.95,1.05);
        std::uniform_int_distribution<int> node(0,n-1);
        std::uniform_real_distribution<real> fee(0,1);
        for(int i=0;i<m;i++)
        {
            int a,b;
            Conversion conversion;
            a=node(rng);
            b=node(rng);
            conversion.rate=rate(rng);
            conversion.fee=fee(rng);
            G.connect(a,b,conversion);
        }

    }
    auto t1=std::chrono::high_resolution_clock::now();
    auto z=minimal_investment(G,0,x);
    auto t2=std::chrono::high_resolution_clock::now();
    std::cout << (z>x?-1:z) << std::endl;
    std::cerr << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "ms" << std::endl;
    std::cout << "Growth upper bound: " << std::scientific << bellman_ford_growth_upper_bound(G,0) << std::endl;
    std::cout << "Growth lower bound: " << std::scientific << bellman_ford_growth_lower_bound(G,0) << std::endl;
}
