#include <bits/stdc++.h>
using namespace std;

int n,m;
const int N = 5;
vector<vector<char>> grid(N);

vector<vector<char>> removeCell(vector<vector<char>> &v, int x, int y) {
    for (int i = x; i > 0; i--) {
        v[i][y] = v[i-1][y];
    }
    v[0][y] = '#';
    return v;
}

int delta[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

bool visited[N][N];
deque<pair<int,int>> component;

void dfs(vector<vector<char>> &g, int i, int j, char c) {
    if (visited[i][j])
        return;
    component.emplace_back(i,j);
    visited[i][j] = true;

    for (int t = 0; t < 4; t++) {
        int newI = i+delta[t][0];
        int newJ = j+delta[t][1];
        if (newI >= 0 && newI < n && newJ >= 0 && newJ < m && g[newI][newJ] == c)
            dfs(g,newI, newJ, c);
    }
}

int main() {
    cin >> n >> m;
    set<char> ss;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < m; j++) {
            grid[i].push_back(s[j]);
            ss.insert(s[j]);
        }
    }

    vector<char> p;
    for (char c : ss)
        p.push_back(c);

    int ans = INT_MAX;
    do {
        vector<vector<char>> newGrid;
        for (int i = 0; i < n; i++)
            newGrid.push_back(grid[i]);
        int nb = 0;
        for (int c = 0; c < 5; c++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    component.clear();
                    if (newGrid[i][j] == p[c])
                        dfs(newGrid, i, j, p[c]);
                    if (!component.empty()) {
                        memset(visited, false, sizeof visited);
                        nb++;
                    }
                    while (!component.empty()) {
                        auto [a, b] = component[0];
                        for (int l = 1; l < component.size(); l++) {
                            auto [a1, b1] = component[l];
                            if (b == b1 && a1 < a) {
                                a1++;
                            }
                            component[l] = {a1, b1};
                        }
                        newGrid = removeCell(newGrid, a, b);
                        component.pop_front();
                    }

                    for (int l = 0; l < m-1; l++) {
                        bool allEmpty = true;
                        for (int b = 0; b < n; b++) {
                            if (newGrid[b][l] != '#')
                                allEmpty = false;
                        }
                        if (allEmpty) {
                            for (int b = 0; b < n; b++) {
                                swap(newGrid[b][l], newGrid[b][l+1]);
                            }
                        }
                    }

                }
            }
        }
        ans = min(ans, nb);
    } while (next_permutation(p.begin(), p.end()));
    cout << ans;
}