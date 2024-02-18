#include <queue>
#include <iostream>
#include <set>
#include <unordered_set>
#include <chrono>
#include <random>

using integer = long long;
integer H(integer x,integer n)
{
    return n*(n-1)/2-x*(n-x-1);
}

integer S(integer x,integer y,integer n, integer m)
{
    return m*H(x,n)+n*H(y,m);
}

struct GridCell
{
    integer x,y,n,m;
    GridCell()= default;
    GridCell(integer x,integer y,integer n,integer m):x(x),y(y),n(n),m(m){}

    std::strong_ordering operator<=>(const GridCell &other) const
    {
        if(cost()!=other.cost())
            return cost()<=>other.cost();
        return std::tie(x,y,n,m)<=>std::tie(other.x,other.y,other.n,other.m);
    }

    bool operator==(const GridCell &other) const = default;

    [[nodiscard]] integer cost() const
    {
        return S(x,y,n,m);
    }
};

template<>
struct std::hash<GridCell>
{
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<integer> dis;
    integer a,b;
    inline static const integer M=1e9+7;
public:
    hash():gen(rd()),dis(1,M),a(dis(gen)),b(dis(gen)){}

    std::size_t operator()(const GridCell &cell) const
    {
        return (a*cell.x+b*cell.y)%M;
    }
};

struct GridId
{

public:

    std::size_t operator()(const GridCell &cell) const
    {
        return cell.m*cell.x+cell.y;
    }
};


constexpr int N_max=10000,M_max=10000;
int main()
{
    int T;
    std::cin >> T;
    std::vector<std::vector<bool>> visited(N_max,std::vector<bool>(M_max,false));
    while(T--)
    {
        integer n,m,k;
        std::cin >> n >> m >> k;
        auto t1=std::chrono::high_resolution_clock::now();
        std::priority_queue<GridCell> Q;
        Q.emplace(0,0,n,m);
        Q.emplace(n-1,0,n,m);
        Q.emplace(0,m-1,n,m);
        Q.emplace(n-1,m-1,n,m);
        int count=0;
        integer total_cost=0;
        std::vector<std::pair<int,int>> visited_indexes;
        visited_indexes.reserve(k);
        visited[0][0]=true;
        visited[n-1][0]=true;
        visited[0][m-1]=true;
        visited[n-1][m-1]=true;
        visited_indexes.emplace_back(0,0);
        visited_indexes.emplace_back(n-1,0);
        visited_indexes.emplace_back(0,m-1);
        visited_indexes.emplace_back(n-1,m-1);
        while(!Q.empty() && count<k)
        {
            auto cell=Q.top();
            Q.pop();
            total_cost+=cell.cost();
            count++;
            for(int i=-1;i<=1;i++) for(int j=-1;j<=1;j++) if(cell.x+i>=0 && cell.x+i<n && cell.y+j>=0 && cell.y+j<m)
                    {
                        GridCell new_cell(cell.x+i,cell.y+j,n,m);
                        if(!visited[new_cell.x][new_cell.y])
                        {
                            Q.push(new_cell);
                            visited[new_cell.x][new_cell.y]=true;
                            visited_indexes.emplace_back(new_cell.x,new_cell.y);
                        }
                    }
        }
        for(auto &index:visited_indexes)
            visited[index.first][index.second]=false;
        std::cout << total_cost << std::endl;
        auto t2=std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "ms" << std::endl;
    }

}