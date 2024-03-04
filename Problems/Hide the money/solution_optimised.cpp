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

int main()
{
    int T;
    std::cin >> T;
    while(T--)
    {
        integer n,m,k;
        std::cin >> n >> m >> k;
        auto t1=std::chrono::high_resolution_clock::now();
        std::priority_queue<GridCell> Q;
        Q.emplace(0,0,n,m);
        int count=0;
        integer total_cost=0;
        std::unordered_set<GridCell,GridId> visited;
        visited.emplace(0,0,n,m);
        integer a=n/2,b=m/2;
        if(k<=a*b)
        {
            integer last_value=0;
            while(!Q.empty() && count<k)
            {
                auto cell=Q.top();
                Q.pop();
                last_value=cell.cost();
                total_cost+=4*last_value;
                count+=4;
                for(int i=0;i<=1;i++) for(int j=0;j<=1;j++) if(cell.x+i>=0 && cell.x+i<n && cell.y+j>=0 && cell.y+j<m)
                {
                    GridCell new_cell(cell.x+i,cell.y+j,n,m);
                    if(visited.find(new_cell)==visited.end())
                    {
                        visited.emplace(new_cell);
                        Q.emplace(new_cell);
                    }
                }
            }
            total_cost-=last_value*(count-k);
        }
        else
        {
            std::vector<integer> X;
            for(int i=0;i<n;i++)
                X.push_back(GridCell(i,m/2,n,m).cost());
            for(int j=0;j<m;j++)  if(j!=m/2)
                X.push_back(GridCell(n/2,j,n,m).cost());
            std::sort(X.rbegin(),X.rend());
            total_cost=std::accumulate(X.begin(),X.begin()+k,0LL);
        }

        std::cout << total_cost << std::endl;
        auto t2=std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "ms" << std::endl;
    }

}