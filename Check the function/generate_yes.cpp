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
    freopen("random_3.txt", "w", stdout);
    int T = 2;
    cout << T << endl;
    while (T--) {
        int n = 100000;
        cout << n << endl;
        for (int i = 0; i < n; i++) {
            cout << mt()%n << ' ';
        }
        cout << endl;
    }
}