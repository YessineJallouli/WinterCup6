#include <bits/stdc++.h>
using namespace std;
mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());

const int N = 1007;
int par[N];

vector<int> prefix(vector<int> s) {
    int n = s.size();
    vector<int> pr(n);
    pr[0] = 0;
    for (int i = 1; i < n; i++) {
        int j = pr[i - 1];
        while (j > 0 && s[j] != s[i])
            j = pr[j - 1];
        if (s[i] == s[j])
            j++;
        pr[i] = j;
    }
    return pr;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("yes100000_bigValues_16.txt", "w", stdout);
    int T = 2;
    cout << T << endl;
    while (T--) {
//        int n = 1e5;
//        cout << n << endl;
        vector<int> v;
        for (int i = 0; i < 100; i++) {
            v.push_back(mt()%15);
        }

        while ((int) v.size() < 9.9e4) {
            int nb = min(v.size(), mt()%1000);
            for (int i = 0; i < nb; i++) {
                v.push_back(v[i]);
            }
        }
        cout << v.size() << '\n';
        vector<int> p = prefix(v);
        for (int i : p)
            cout << i << ' ';
        cout << '\n';
    }
}