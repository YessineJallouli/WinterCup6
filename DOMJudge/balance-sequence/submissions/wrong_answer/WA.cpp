#include <bits/stdc++.h>
using namespace std;

mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        string s; cin >> s;
        int pr[n+1]; pr[0] = 0;
        for (int i = 1; i <= n; i++) {
            pr[i] = pr[i-1] + ((s[i-1] == '(') ? 1 : -1);
        }
        int idmn = 0;
        int idmx = 0;
        for (int i = 0; i < n; i++)
            if (pr[i] < 0)
                idmx = i;
        for (int i = n - 1; i >= 0; i--)
            if (pr[i] < 0)
                idmn = i;
        idmn--; idmx--;
        cout << idmn+1 << ' ' << idmx+2 << '\n';
    }
}
