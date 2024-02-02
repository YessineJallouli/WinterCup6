#include <bits/stdc++.h>
using namespace std;

const int N = 1e5+7;
int furthest[N];

int main() {
    int n; cin >> n;
    set<int> leaves;

    for (int i = 1; i <= n; i++) {
        cin >> furthest[i];
        if (furthest[i] == i) {
            cout << "NO\n";
            return 0;
        }
        leaves.insert(furthest[i]);
    }

    if (leaves.size() == n)
        cout << "NO\n";
    else
        cout << "YES\n";
}
