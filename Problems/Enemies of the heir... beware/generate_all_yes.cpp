#include <bits/stdc++.h>
#define ll long long
using namespace std;

int N;

set<vector<int>> s;

const int mx = 100;

int par[mx];

int Find(int node) {
    if (par[node] == node)
        return node;
    return par[node] = Find(par[node]);
}

void Union(int n1, int n2) {
    int a1 = Find(n1);
    int a2 = Find(n2);
    if (a1 == a2)
        return;
    par[a1] = a2;
}

bool check(vector<int> pr) {
    int n = pr.size();
    for (int i = 0; i < n; i++)
        par[i] = i;

    if (pr[0] != 0)
        return false;
    set<int> notSame[n];
    set<int> same[n];

    for (int i = 1; i < n; i++) {
        int k = pr[i];
        for (int j = 0; j < k; j++) {
            same[j].insert(i-k+j+1);
            same[i-k+j+1].insert(j);
            Union(j,i-k+j+1);
        }
    }

    for (int i = 1; i < n; i++) {
        int j = pr[i-1];
        map<int,int> vis;
        if (pr[i] == 0) {
            notSame[Find(i)].insert(Find(0));
            notSame[Find(0)].insert(Find(i));
        }
        while (j > 0 && pr[i] != j+1) {
            if (vis[j])
                return false;
            vis[j] = true;
            notSame[Find(i)].insert(Find(j));
            notSame[Find(j)].insert(Find(i));
            j = pr[j-1];
        }
    }

    for (int i = 0; i < n; i++) {
        if (notSame[i].count(i))
            return false;
        for (int j : same[i]) {
            if (notSame[i].count(j)) {
                return false;
            }
        }
    }
    return true;
}

void solve(vector<int> v, int n) {
    if (n == 0) {
        s.insert(v);
        return;
    }
    if (n == N) {
        v.push_back(0);
        solve(v, n-1);
        return;
    }
    for (int i = v.back()+1; i >= 0; i--) {
        v.push_back(i);
        solve(v, n-1);
        v.pop_back();
    }

}

int main() {
    freopen("all_yes_1.txt", "w", stdout);
    N = 1;
    vector<int> k;
    solve(k,N);
    set<vector<int>> t;
    for (auto v : s) {
        if (check(v)) {
            t.insert(v);
        }
    }
    cout << t.size() << '\n';
    for (auto i : t) {
        cout << N << endl;
        for (int p : i)
            cout << p << ' ';
        cout << '\n';
    }
}
