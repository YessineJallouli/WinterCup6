#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,m; cin >> n >> m;
    int x; cin >> x;
    vector<vector<tuple<int,double,double>>> adj(n+1);
    for (int i = 0; i < m; i++) {
        int u,v; double r,f; cin >> u >> v >> r >> f;
        adj[u].emplace_back(v,r,f);
    }
    int lft = 0, rt = x;
    int ans = -1;
    while (lft <= rt) {
        int md = (lft+rt)/2;
        vector<int> v = {1};
        set<int> vis;
        bool ok = false;
        double res[n+1];
        for (int i = 0; i <= n; i++)
            res[i] = 0;
        res[1] = md;
        while (! v.empty()) {
            for (int i : v)
                vis.insert(i);
            vector<int> aux;
            for (int i : v) {
                for (auto [j,r,f] : adj[i]) {
                    res[j] = max(res[j], (res[i]-f)*r);
                    if (! vis.count(j) || j == 1) {
                        aux.push_back(j);
                        vis.insert(j);
                    }
                }
            }
            aux.swap(v);
        }
        if (res[1] > md) {
            rt = md-1;
            ans = md;
        }
        else {
            lft = md+1;
        }
    }
    cout << ans;
}