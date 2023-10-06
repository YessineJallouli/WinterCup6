//
// Created by ramizouari on 06/10/23.
//
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include <queue>

struct PlanarGraph
{

    int n,m;
    using couple=std::pair<int,int>;

    std::vector<std::vector<std::vector<couple>>> adjacencyList,reverseList;
    PlanarGraph(int n,int m):n(n),m(m),adjacencyList(n,std::vector<std::vector<couple>>(m)),reverseList(n,std::vector<std::vector<couple>>(m)){}
    void connect(int u1,int v1,int u2,int v2)
    {
        adjacencyList[u1][v1].emplace_back(u2,v2);
    }
};

struct WinterCupGrid : public PlanarGraph
{
    std::vector<std::vector<char>> grid;
    static inline constexpr const char* WORD="wintercup";
    std::array<char,26> adjacent_word;
public:
    WinterCupGrid(int n,int m):PlanarGraph(n,m),grid(n,std::vector<char>(m))
    {
        std::fill(adjacent_word.begin(),adjacent_word.end(),'\0');
        for(int i=0;i<std::strlen(WORD);i++)
            adjacent_word[WORD[i]-'a']=WORD[(i+1)%9];
    }

    bool keyword(char c) const
    {
        return adjacent_word[std::tolower(c)-'a'];
    }

    void build()
    {
        for(int i=0;i<n;i++) for(int j=0;j<m;j++) if(keyword(grid[i][j]))
        {
            if(i>0 && grid[i-1][j] == adjacent_word[std::tolower(grid[i][j])-'a'])
                connect(i,j,i-1,j);
            if(i<n-1 && grid[i+1][j] == adjacent_word[std::tolower(grid[i][j])-'a'])
                connect(i,j,i+1,j);
            if(j>0 && grid[i][j-1] == adjacent_word[std::tolower(grid[i][j])-'a'])
                connect(i,j,i,j-1);
            if(j<m-1 && grid[i][j+1] == adjacent_word[std::tolower(grid[i][j])-'a'])
                connect(i,j,i,j+1);
        }
    }

    bool word_connected() const
    {
        std::queue<couple> Q;
        std::vector<std::vector<bool>> visited(n,std::vector<bool>(m));
        if(keyword(grid[0][0]))
        {
            Q.emplace(0, 0);
            visited[0][0]=true;
        }
        while(!Q.empty())
        {
            auto [u,v]=Q.front();
            Q.pop();
            for(auto [p,q] : adjacencyList[u][v]) if (!visited[p][q])
            {
                visited[p][q]=true;
                Q.emplace(p,q);
            }
        }
        return visited[n-1][m-1];
    }
};

std::istream& operator>>(std::istream& H,WinterCupGrid& G)
{
    for(int i=0;i<G.n;i++)
        for(int j=0;j<G.m;j++)
            H >> G.grid[i][j];
    G.build();
    return H;
}


int main()
{
    int n,m;
    std::cin >> n >> m;
    WinterCupGrid G(n,m);
    std::cin >> G;
    std::cout << G.word_connected() << '\n';
}