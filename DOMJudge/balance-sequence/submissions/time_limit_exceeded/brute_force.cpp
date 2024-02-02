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
        for (int j = i; j < n; j++) {
            reverse(s.begin()+i, s.begin()+j+1);
            if (isBalanced(s)) {
                cout << i+1 << ' ' << j+1 << '\n';
                return;
            }
            reverse(s.begin()+i, s.begin()+j+1);
        }
    }
}

int main() {
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        string s; cin >> s;
        solve(s);
    }
}
