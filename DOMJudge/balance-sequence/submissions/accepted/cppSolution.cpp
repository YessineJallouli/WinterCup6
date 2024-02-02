#include <bits/stdc++.h>
using namespace std;

int main() {
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        string s; cin >> s;
        int pr[n+1];
        pr[0] = 0;
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
        int L = 0, R = idmx;
        for (int i = 0; i <= idmn; i++) {
            if (pr[L] < pr[i]) {
                L = i;
            }
        }
        for (int i = idmx; i <= n; i++) {
            if (pr[R] < pr[i]) {
                R = i;
            }
        }
        int id = 0;
        for (int i = 0; i <= n; i++) {
            if (pr[i] > pr[id])
                id = i;
        }
        L++;
        cout << L << ' ' << R << '\n';
    }
}
