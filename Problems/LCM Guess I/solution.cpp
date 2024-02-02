#include <bits/stdc++.h>
#define ll long long
using namespace std;
mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());

bool isPrime(int n) {
    if (n%2 == 0)
        return false;
    for (int i = 3; i*i <= n; i+=2) {
        if (n%i == 0)
            return false;
    }
    return true;
}

ll ask(int i, int j) {
    cout << "? " << i << ' ' << j << endl;
    ll x; cin >> x;
    return x;
}

int main() {
    int n; cin >> n;
    int biggestPrime = n;
    while (!isPrime(biggestPrime))
        biggestPrime--;

    int ans[n];
    ll q1[n];
    set<ll> s;
    ll g = 0;
    int idmn = 2;
    set<int> numbers;
    for (int i = 1; i <= n; i++) {
        numbers.insert(i);
    }
    bool all = true;
    for (int i = 2; i <= n; i++) {
        q1[i-1] = ask(1, i);
        g = __gcd(g, q1[i-1]);
        s.insert(q1[i-1]);
        if (q1[i-1]%biggestPrime == 0)
            idmn = i;
        else
            all = false;
    }
    if (all) {
        ans[0] = biggestPrime;
        for (int i = 1; i < n; i++)
            ans[i] = q1[i]/biggestPrime;
    }
    else {
        ans[0] = (int) g;
        ans[idmn-1] = biggestPrime;
        numbers.erase(ans[0]);
        numbers.erase(biggestPrime);
        for (int i = 2; i < n; i++) {
            if (idmn == n && i == n-1)
                break;
            if (i == idmn)
                continue;
            ans[i-1] = ask(idmn, i)/biggestPrime;
            numbers.erase(ans[i-1]);
        }
        if (idmn != n) {
            ans[n - 1] = *numbers.begin();
        }
        else {
            ans[n-2] = *numbers.begin();
        }
    }
    cout << "! ";
    for (int i = 0; i < n; i++)
        cout << ans[i] << ' ';
    cout << endl;
}

// 4 1 2 3