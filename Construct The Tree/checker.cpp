#include <bits/stdc++.h>
using namespace std;

const int N = 2e5+7;
int mx[N];
int dp[N];
 
void dfs(int u, const vector<vector<int>> &tree, int p = -1) {
    mx[u] = 0;
    for (auto v : tree[u]) {
        if (v == p)
            continue;
        dfs(v,tree, u);
        mx[u] = max(1+mx[v], mx[u]);
    }
}
 
void reroot(int u, const vector<vector<int>> &tree, int p = -1, int m = 0) {
    dp[u] = max(m, mx[u]);
    multiset<int> st;
    st.insert(-(m));
    for (auto v : tree[u]) {
        if (v == p)
            continue;
        st.insert(-(mx[v]+1));
    }
    for (auto v : tree[u]) {
        if (v == p)
            continue;
        st.erase(st.find(-(mx[v]+1)));
        reroot(v,tree,u, -*st.begin()+1);
        st.insert(-(mx[v]+1));
    }
}
