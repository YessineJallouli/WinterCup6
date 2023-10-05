//
// Created by ramizouari on 11/12/22.
//
#include <iostream>
#include <queue>
#include <algorithm>
#include <set>
#include <numeric>
#include <cmath>

using natural = std::uint64_t;
using integer = std::int64_t;
using real = long double;
using IR=real;
constexpr real epsilon=1e-7;

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
bool is_zero(const R&a)
{
    return a==R{};
}

bool is_zero(const real &a)
{
    return std::abs(a) < epsilon;
}

template<integer mod>
class cyclic
{
    integer n;
public:
    inline static bool assume_prime=true;
    inline static constexpr integer m = mod;
    cyclic(int o=0):n((o+m)%m){}
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

    auto inv() const
    {
        return pow(*this,m-2);
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
};

template<integer m>
auto operator*(int k,cyclic<m> s)
{
    return s*k;
}

template<integer m>
auto operator+(int k,cyclic<m> s)
{
    return s+k;
}

struct v_shape
{
    int n;
};


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
        if(rank[u]<rank[u])
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


struct Graph
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
        for(int i=0;i<n;i++) for(auto j:adjacencyList[i])
                if(!classes.equivalent(i,j) && !S[componentId[i]].count(componentId[j]))
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

constexpr integer M=1e9+7;
using IK=real;

template<typename K>
K inv(K a)
{
    return K{1}/a;
}

int encode(int a,int b,int n)
{
    return a*n+b;
}

template<integer m>
std::ostream& operator<<(std::ostream& H,const cyclic<m>&s)
{
    return H << (integer&)s;
}

int main()
{
    int n, m;
    int s, t;
    std::cin >> n >> m >> s >> t;
    d_matrix<IK> A(0,  m_shape{ n,n });
    d_matrix<IK> S(1,m_shape{n*n,n*n});
    d_vector<IK> v(v_shape{n*n});
    Graph G(n);
    IK inv100=inv<IK>(100);
    //std::cout << inv100;
    for (int i = 0; i < m; i++)
    {
        int a, b;
        real p;
        std::cin >> a >> b >> p;
        if(p!=0)
            G.connect(a,b);
        IK w=std::lround(100*p);
        A[a][b] += w*inv100;
    }
    for(int i=0;i<n;i++)
    {
        auto acc=std::accumulate(A[i].begin(),A[i].end(),IK{});
        if constexpr (std::is_floating_point_v<IK>)
        {
            if( std::abs(acc-1) > epsilon)
                throw std::runtime_error("Transition probabilities add to " + std::to_string(acc) +" instead of 1 in node "+ std::to_string(i));
        }
        else
        {
            if(acc!=1)
                throw std::runtime_error("Transition probabilities add to " + std::to_string(static_cast<int>(acc)) +" instead of 1 in node "+ std::to_string(i));
        }


    }
    for(int a=0;a<n;a++) for(int b=0;b<n;b++)
        {
            if(a==b)
                continue;
            else for (int c = 0; c < n; c++) for (int d = 0; d < n; d++)
                        S[encode(a, b, n)][encode(c, d, n)] -= A[a][c] * A[b][d];
            v[encode(a,b,n)]=1;
        }
    auto [DAG,R]=G.condensationGraph();
    bool isInfinity=false;
    std::vector<std::vector<bool>> visited(n,std::vector<bool>(n));
    std::queue<std::pair<int,int>> Q;
    if(s!=t)
        Q.emplace(s,t);
    while(!Q.empty())
    {
        auto [a,b]=Q.front();
        Q.pop();
        for(auto p:G.adjacencyList[a]) for(auto q:G.adjacencyList[b]) if(!visited[p][q] && p!=q)
                {
                    visited[p][q]=true;
                    Q.emplace(p,q);
                }
    }
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) if(visited[i][j] && DAG.adjacencyList[R[i]].empty() && DAG.adjacencyList[R[j]].empty())
            {
                if(R[i]!=R[j])
                {
                    isInfinity=true;
                    goto exit;
                }
                else
                {
                    std::vector<std::vector<bool>> nodeVisited(n,std::vector<bool>(n));
                    Q.emplace(i,j);
                    nodeVisited[i][j]=true;
                    while(!Q.empty())
                    {
                        auto [a,b]=Q.front();
                        Q.pop();
                        for(auto c:G.adjacencyList[a]) for(auto d:G.adjacencyList[b]) if(!nodeVisited[c][d])
                                {
                                    nodeVisited[c][d]=true;
                                    Q.emplace(c,d);
                                }
                        if(a==b)
                            Q={};
                    }
                    bool reachable=false;
                    for(int k=0;k<n && !reachable;k++)
                        reachable=nodeVisited[k][k];
                    if(!reachable)
                    {
                        isInfinity=true;
                        goto exit;
                    }
                }
            }
    exit:
    std::cout.precision(4);
    if(isInfinity)
        std::cout << std::numeric_limits<real>::infinity() << '\n';
    else
    {
        auto u=S.solve(v);
        std::cout << std::fixed << u[encode(s, t, n)] << '\n';
    }
}