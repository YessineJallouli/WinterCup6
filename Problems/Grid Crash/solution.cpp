//
// Created by ramizouari on 06/10/23.
//
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>
#include <stack>
#ifdef TIMER_ON
#include <chrono>
#endif

using couple=std::pair<int,int>;

class PlanerUnionFind
{
    int n,m;
    std::vector<std::vector<couple>> parent;
    std::vector<std::vector<int>> rank;
public:
    PlanerUnionFind(int n,int m):n(n),m(m),rank(n,std::vector<int>(m)),parent(n,std::vector<couple>(m))
    {
        for(int i=0;i<n;i++) for(int j=0;j<m;j++)
            parent[i][j]={i,j};
    }

    void connect(const couple &p,const couple & q)
    {
        const auto &[a,b] =p;
        const auto &[c,d] =q;
        auto u=representative(p),v=representative(q);
        const auto &[u1,u2]= u;
        const auto &[v1,v2]= v;
        if(u==v)
            return;
        if(rank[u1][u2]<rank[v1][v2])
            parent[u1][u2]=parent[v1][v2];
        else if(rank[v1][v2]<rank[u1][u2])
            parent[v1][v2]=parent[u1][u2];
        else
        {
            parent[u1][u2]=parent[v1][v2];
            rank[v1][v2]++;
        }
    }

    couple representative(const couple& p)
    {
        const auto &[a,b]=p;
        if(parent[a][b]==p)
            return p;
        else return parent[a][b]= representative(parent[a][b]);
    }

    bool equivalent(const couple &a,const couple& b)
    {
        return representative(a)== representative(b);
    }
};

using small_int=int;
using couple=std::pair<small_int,small_int>;

int diameter(const couple &a)
{
    return a.second-a.first;
}

struct Grid
{
    int n,m;
    std::vector<std::vector<char>> grid;
    void update_shape()
    {
        for(int j=0;j<m;j++)
        {
            int r=0;
            for (int i =n-1;i>=0;i--)
            {
                if(!grid[i][j]) r++;
                else if(r)
                {
                    grid[i+r][j]=grid[i][j];
                    grid[i][j]='\0';
                }
            }
        }
        /*
         * Erase empty columns
         * */
        int r_row=0;
        for(int i=n-1;i>=0;i--)
        {
            bool all_empty=true;
            for(int j=0;j<m && all_empty;j++) if(grid[i][j])
                    all_empty=false;
            if(all_empty) r_row++;
            else for(int j=0;j<m;j++) if(r_row)
            {
                grid[i+r_row][j]=grid[i][j];
                grid[i][j]='\0';
            }
        }
        /*
         * Erase empty columns
         * */
        int r_col=0;
        for(int j=0;j<m;j++)
        {
            bool all_empty=true;
            for(int i=0;i<n && all_empty;i++) if(grid[i][j])
                    all_empty=false;
            if(all_empty)
                r_col++;
            else for(int i=0;i<n;i++) if(r_col)
            {
                grid[i][j-r_col]=grid[i][j];
                grid[i][j]='\0';
            }
        }

        for(int i=0;i+r_row<n;i++)
            grid[i].swap(grid[i+r_row]);


        /*
         * Resize
         * */
        grid.resize(n-r_row);
        for(int i=0;i<n-r_row;i++)
            grid[i].resize(m-r_col);
        n-=r_row;
        m-=r_col;
    }

    inline static std::mt19937_64 rng{std::random_device{}()};

public:
    Grid():n(0),m(0){}
    Grid(int _n,int _m):n(_n),m(_m),grid(n,std::vector<char>(m)){}
    Grid(std::vector<std::vector<char>> _grid):n(_grid.size()),m(_grid[0].size()),grid(std::move(_grid)){}

    void remove(const std::vector<couple> &C)
    {
        for(auto [a,b]:C)
            grid[a][b]='\0';
        update_shape();
    }

    void remove(int a, int b)
    {
        if(!grid[a][b])
            return;
        std::queue<couple> Q;
        Q.emplace(a, b);
        char colour=grid[a][b];
        std::vector<std::vector<bool>> visited(n,std::vector<bool>(m));
        visited[a][b]=true;
        while(!Q.empty())
        {
            auto [u,v]=Q.front();
            grid[u][v]='\0';
            Q.pop();
            if(u < n -1 && grid[u+1][v]==colour && !visited[u+1][v])
            {
                Q.emplace(u+1,v);
                visited[u+1][v]=true;
            }
            if(u>0 && grid[u-1][v]==colour && !visited[u-1][v])
            {
                Q.emplace(u-1,v);
                visited[u-1][v]=true;
            }
            if(v < m -1 && grid[u][v+1]==colour && !visited[u][v+1])
            {
                Q.emplace(u, v + 1);
                visited[u][v+1]=true;

            }
            if(v > 0 && grid[u][v-1]==colour && !visited[u][v-1])
            {
                Q.emplace(u,v-1);
                visited[u][v-1]=true;
            }
        }

        update_shape();
    }


    [[nodiscard]] std::vector<couple> actions() const
    {
        std::vector<couple> A;
        std::vector<std::vector<bool>> visited(n,std::vector<bool>(m));
        std::vector<std::vector<small_int>> clusterSize(n,std::vector<small_int>(m));
        for(int i=0;i<n;i++) for(int j=0;j<m;j++) if(grid[i][j] && !visited[i][j])
        {
            std::stack<couple,std::vector<couple>> Q;
            A.emplace_back(i,j);
            Q.emplace(i, j);
            visited[i][j]=true;
            while(!Q.empty())
            {
                auto [u,v]=Q.top();
                clusterSize[i][j]+=1;
                Q.pop();
                if(u < n -1 && grid[u+1][v]==grid[i][j] && !visited[u+1][v])
                {
                    Q.emplace(u+1,v);
                    visited[u+1][v]=true;
                }
                if(u>0 && grid[u-1][v]==grid[i][j] && !visited[u-1][v])
                {
                    Q.emplace(u-1,v);
                    visited[u-1][v]=true;
                }
                if(v < m -1 && grid[u][v+1]==grid[i][j] && !visited[u][v+1])
                {
                    Q.emplace(u, v + 1);
                    visited[u][v+1]=true;

                }
                if(v > 0 && grid[u][v-1]==grid[i][j] && !visited[u][v-1])
                {
                    Q.emplace(u,v-1);
                    visited[u][v-1]=true;
                }
            }
        }
        //std::sort(A.begin(),A.end(),[&clusterSize](const auto &x,const auto &y){
        //    return clusterSize[x.first][x.second] < clusterSize[y.first][y.second];
        //});
        return A;
    }

    [[nodiscard]] std::vector<std::vector<couple>> cluster_actions() const
    {
        std::vector<std::vector<couple>> C;
        std::vector<std::vector<bool>> visited(n,std::vector<bool>(m));
        for(int i=0;i<n;i++) for(int j=0;j<m;j++) if(!visited[i][j] && grid[i][j])
        {
            std::stack<couple,std::vector<couple>> Q;
            C.emplace_back();
            auto &A=C.back();
            A.emplace_back(i,j);
            Q.emplace(i, j);
            visited[i][j]=true;
            while(!Q.empty())
            {
                auto [u,v]=Q.top();
                Q.pop();
                if(u < n -1 && grid[u+1][v]==grid[i][j] && !visited[u+1][v])
                {
                    Q.emplace(u+1,v);
                    A.emplace_back(u+1,v);
                    visited[u+1][v]=true;
                }
                if(u>0 && grid[u-1][v]==grid[i][j] && !visited[u-1][v])
                {
                    Q.emplace(u-1,v);
                    A.emplace_back(u-1,v);
                    visited[u-1][v]=true;
                }
                if(v < m -1 && grid[u][v+1]==grid[i][j] && !visited[u][v+1])
                {
                    Q.emplace(u, v + 1);
                    A.emplace_back(u,v+1);
                    visited[u][v+1]=true;

                }
                if(v > 0 && grid[u][v-1]==grid[i][j] && !visited[u][v-1])
                {
                    Q.emplace(u,v-1);
                    A.emplace_back(u,v-1);
                    visited[u][v-1]=true;
                }
            }
        }
        //std::sort(A.begin(),A.end(),[&clusterSize](const auto &x,const auto &y){
        //    return clusterSize[x.first][x.second] < clusterSize[y.first][y.second];
        //});
        //std::shuffle(C.begin(),C.end(),rng);
        return C;
    }

    [[nodiscard]] std::vector<std::vector<couple>> cluster_actions_uf() const
    {
        std::vector<std::vector<couple>> C;
        std::map<couple,int> mapper;
        PlanerUnionFind UF(n,m);
        for(int i=0;i<n;i++) for(int j=0;j<m;j++) if(grid[i][j])
        {
            auto &u=i;
            auto &v=j;
            if(u < n -1 && grid[u+1][v]==grid[i][j])
                UF.connect({u,v},{u+1,v});
            if(v < m -1 && grid[u][v+1]==grid[i][j])
                UF.connect({u,v},{u,v+1});
        }

        for(int i=0;i<n;i++) for(int j=0;j<m;j++) if(grid[i][j])
        {
            auto p=UF.representative({i,j});
            if(!mapper.count(p))
            {
                mapper[p] = C.size();
                C.emplace_back();
            }
            C[mapper[p]].emplace_back(i,j);
        }
        //std::sort(A.begin(),A.end(),[&clusterSize](const auto &x,const auto &y){
        //    return clusterSize[x.first][x.second] < clusterSize[y.first][y.second];
        //});
        return C;
    }

    [[nodiscard]] Grid next(int a,int b) const
    {
        Grid G=*this;
        G.remove(a,b);
        return G;
    }

    [[nodiscard]] Grid next(const std::vector<couple> &C) const
    {
        Grid G=*this;
        G.remove(C);
        return G;
    }

    bool operator==(const Grid &G) const
    {
        return grid==G.grid;
    }

    bool operator!=(const Grid &G) const
    {
        return grid!=G.grid;
    }

    bool operator<(const Grid &G) const
    {
        return grid<G.grid;
    }

    [[nodiscard]] bool empty() const
    {
        return n==0 && m==0;
    }
};

const Grid EmptyGrid{};

template<>
class std::hash<Grid>
{
    std::random_device dev;
    std::mt19937_64 g{dev()};
    std::int64_t M=1e9+7;
    std::array<std::int64_t,1024> A;
public:
    hash()
    {
        std::uniform_int_distribution<std::int64_t> d(1,M-1);
        for(auto &a:A)
            a=d(g);
    }
    size_t operator()(const Grid &G) const
    {
        std::int64_t h=A.back();
        for(int i=0;i<G.n;i++) for(int j=0;j<G.m;j++)
            h=(h+A[i*G.m+j]*G.grid[i][j])%M;
        return h;
    }
};

std::ostream& operator<<(std::ostream& H,const Grid& G)
{
    for(int i=0;i<G.n;i++)
        for(int j=0;j<G.m;j++)
            H << (G.grid[i][j]=='\0'?'.':G.grid[i][j]) << (j==G.m-1?'\n':' ');
    return H;
}

std::istream& operator>>(std::istream& H,Grid& G)
{
    for(int i=0;i<G.n;i++) for(int j=0;j<G.m;j++)
    {
        H >> G.grid[i][j];
        if(G.grid[i][j]=='.')
            G.grid[i][j]='\0';
    }
    return H;
}

struct GridMoves
{
    int number;
    std::vector<Grid> grids;
};

int fewest_moves(Grid G)
{
    std::queue<Grid> Q;
    std::unordered_map<Grid,int> D;
    Q.push(G);
    D[G]=0;
    auto R=G;
    while(!Q.empty())
    {
        G=Q.front();
        Q.pop();
        if(G.empty())
            break;
        for(auto [a,b]:G.actions())
        {
            Grid H=G.next(a,b);
            if(!D.count(H))
            {
                D[H]=D[G]+1;
                Q.push(H);
            }
        }
    }
    return D[EmptyGrid];
};

GridMoves fewest_moves_sequence(Grid G)
{
    std::queue<Grid> Q;
    std::unordered_map<Grid,int> D;
    std::unordered_map<Grid,Grid> P;
    Q.push(G);
    D[G]=0;
    auto R=G;
    while(!Q.empty())
    {
        G=Q.front();
        Q.pop();
        if(G.empty())
            break;
        for(auto [a,b]:G.actions())
        {
            Grid H=G.next(a,b);
            if(!D.count(H))
            {
                D[H]=D[G]+1;
                P[H]=G;
                Q.push(H);
            }
        }
    }
    std::vector<Grid> S;
    while(G!=R)
    {
        S.push_back(G);
        G=P[G];
    }
    S.push_back(R);
    std::reverse(S.begin(),S.end());
    return {D[EmptyGrid],S};
};

struct GridColoursHeuristic
{
    Grid grid;
    int h=0;
    int c=0;
    GridColoursHeuristic(Grid _grid,int cost=0):grid(std::move(_grid)),c(cost)
    {
        std::set<char> S;
        for(int i=0;i<grid.n;i++) for(int j=0;j<grid.m;j++) if(grid.grid[i][j])
            S.emplace(grid.grid[i][j]);
        h=S.size();
    }
    bool operator<(const GridColoursHeuristic&b) const
    {
        return (h+c)<(b.h+b.c);
    }

    bool operator>(const GridColoursHeuristic&b) const
    {
        return (h+c)>(b.h+b.c);
    }

    int value() const
    {
        return h;
    }
};

struct GridClusterHeuristic
{
    Grid grid;
    int h=0;
    int c=0;
    GridClusterHeuristic(Grid _grid,int cost=0):grid(std::move(_grid)),c(cost)
    {
        h=grid.actions().size();
    }
    bool operator<(const GridClusterHeuristic&b) const
    {
        return (h+c)<(b.h+b.c);
    }

    bool operator>(const GridClusterHeuristic&b) const
    {
        return (h+c)>(b.h+b.c);
    }
    [[nodiscard]] int value() const
    {
        return h;
    }
};

struct GridCombinedHeuristic
{
    Grid grid;
    double h=0;
    int c=0;
    inline static constexpr double theta=0.1;
    GridCombinedHeuristic(Grid _grid,int cost=0):grid(std::move(_grid)),c(cost)
    {
        std::set<char> S;
        for(int i=0;i<grid.n;i++) for(int j=0;j<grid.m;j++) if(grid.grid[i][j])
                    S.emplace(grid.grid[i][j]);
        h=std::cos(theta)* S.size()+ std::sin(theta)*grid.actions().size();
    }
    bool operator<(const GridCombinedHeuristic&b) const
    {
        return (h+c)<(b.h+b.c);
    }

    bool operator>(const GridCombinedHeuristic&b) const
    {
        return (h+c)>(b.h+b.c);
    }
    [[nodiscard]] double value() const
    {
        return h;
    }
};

template<typename Heuristic>
int fewest_moves_heuristic(Grid G)
{
    if constexpr (std::is_same_v<Heuristic,void>)
        return fewest_moves(G);
    else
    {
        std::priority_queue<Heuristic,std::vector<Heuristic>,std::greater<>> Q;
        std::unordered_set<Grid> visited;
        Q.emplace(G);
        auto R=G;
        while(!Q.empty())
        {
            G=Q.top().grid;
            int c=Q.top().c;
            Q.pop();
            if(visited.count(G))
                continue;
            visited.insert(G);
            if(G.empty())
                return c;
            for(const auto& C:G.cluster_actions())
            {
                Grid H=G.next(C);
                if(!visited.count(H))
                    Q.emplace(H,c+1);
            }
        }
        return -1;
    }
};

template<typename Heuristic>
GridMoves fewest_moves_heuristic_sequence(Grid G)
{
    if constexpr (std::is_same_v<Heuristic,void>)
        return fewest_moves_sequence(G);
    else
    {
        using GridHeuristic=Heuristic;
        std::priority_queue<GridHeuristic,std::vector<GridHeuristic>,std::greater<>> Q;
        std::unordered_map<Grid,int> D;
        std::unordered_map<Grid,Grid> P;
        Q.emplace(G);
        D[G]=0;
        auto R=G;
        while(!Q.empty())
        {
            G=Q.top().grid;
            int c=Q.top().c;
            Q.pop();
            if(G.empty())
                break;
            for(auto [a,b]:G.actions())
            {
                Grid H=G.next(a,b);
                if(!D.count(H))
                {
                    D[H]=D[G]+1;
                    Q.emplace(H,c+1);
                    P[H]=G;
                }
            }
        }
        std::vector<Grid> S;
        G=EmptyGrid;
        while(G!=R)
        {
            S.push_back(G);
            G=P[G];
        }
        S.push_back(R);
        std::reverse(S.begin(),S.end());
        return {D[EmptyGrid],S};
    }
};

#ifdef CLUSTER_HEURISTIC
using Heuristic=GridClusterHeuristic;
#elif defined COLOUR_HEURISTIC
using Heuristic=GridColoursHeuristic;
#elif defined COMBINED_HEURISTIC
using Heuristic=GridCombinedHeuristic;
#else
using Heuristic=void;
#endif

int main()
{
    int n,m;
    std::cin >> n >> m;
    Grid G{n,m};
    std::cin >> G;
#ifdef TIMER_ON
    auto t1=std::chrono::high_resolution_clock::now();
#endif
//    auto x=fewest_moves_heuristic<Heuristic>(G);
    auto [x,S]=fewest_moves_heuristic_sequence<Heuristic>(G);
    std::cout << x << '\n';
    for(auto &H:S)
        std::cout << H << '\n';
#ifdef TIMER_ON
    auto t2=std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << '\n';
#endif
    //for(auto &H:S)
    //    std::cout << H << '\n';
}