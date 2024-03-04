#include <bits/stdc++.h>
using namespace std;

const int N = 3e5+7;
int a[N], p[N];

void test_case() {
    int n; cin >> n;
    memset(a, -1, sizeof(a));

    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }

    if (p[0]) {
        cout << "-1\n";
        return;
    }

    a[0] = 1;
    int cur = 2;
    for (int i = 1; i < n; i++) {
        if (p[i] >= i + 1) {
            cout << "-1\n";
            return;
        }
        int len = p[i - 1];
        if (p[i] > p[i - 1] + 1) {
            cout << "-1\n";
            return;
        }
        if (p[i] == 0) {
            a[i] = cur++;
            continue;
        }
        set<int> st;
        while (len && len + 1 != p[i]) {
            st.insert(a[len]);
            len = p[len - 1];
            if (len + 1 < p[i]) {
                cout << "-1\n";
                return;
            }
        }
        if (len + 1 == p[i]) {
            a[i] = a[len];
            if (st.count(a[i])) {
                cout << "-1\n";
                return;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        test_case();
    }
    return 0;
}
