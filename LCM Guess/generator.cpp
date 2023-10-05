#include <bits/stdc++.h>
using namespace std;
mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    freopen("n50_2.txt", "w", stdout);
    int n = 50;
    cout << n << endl;
    int p[n];
    for (int i = 1; i <= n; i++)
        p[i-1] = i;

    shuffle(p, p+n, mt);
    for (int i = 0; i < n; i++)
        cout << p[i] << ' ';
}