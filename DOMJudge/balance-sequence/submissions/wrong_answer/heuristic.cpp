#include <bits/stdc++.h>
using namespace std;

bool isBalanced(string &s) {
    int sm = 0;
    int n = (int) s.size();
    for (int i = 0; i < n; i++) {
        if (s[i] == '(')
            sm++;
        else
            sm--;
        if (sm < 0)
            return false;
    }
    return (sm == 0);
}

void solve(string &s) {
    int  n = s.size();
    for (int i = 0; i < n; i++) {
        reverse(s.begin(), s.begin()+i+1);
        if (isBalanced(s)) {
            cout << 1 << ' ' << i+1 << '\n';
            return;
        }
        reverse(s.begin(), s.begin()+i+1);
    }
    for (int i = 0; i < n; i++) {
        reverse(s.begin()+i, s.begin()+n);
        if (isBalanced(s)) {
            cout << i+1 << ' ' << n << '\n';
            return;
        }
        reverse(s.begin()+i, s.begin()+n);
    }
    cout << "NOANSWER\n";
}

int main() {
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        string s; cin >> s;
        solve(s);
    }
}
