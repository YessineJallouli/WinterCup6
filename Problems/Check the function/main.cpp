#include <bits/stdc++.h>
using namespace std;

const int N = 2e5+7;
int par[N];

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

bool check(vector<int> &pr) {
    int n = pr.size();
    for (int i = 0; i < n; i++)
        par[i] = i;

    if (pr[0] != 0)
        return false;
    set<int> notSame[n];
    set<int> same[n];

    for (int i = 1; i < n; i++) {
        int k = pr[i];
        for (int j = 0; j < min(k,1); j++) {
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

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        for (int i = 0; i < n; i++)
            par[i] = i;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        if (check(a)) {
            cout << "YES\n";
        }
        else {
            cout << "NO\n";
        }
    }
}
