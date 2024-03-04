// Mtaylor
#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define fi first
#define se second
#define mp make_pair
#define endl '\n';
#define sz(x) ((int)(x).size())
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
void dbg_out() { cerr << endl; }
template <typename Head, typename... Tail>
void dbg_out(Head H, Tail... T) {
    cerr << ' ' << H;
    dbg_out(T...);
}
#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
typedef long long ll;
const int N = 3e5 + 5;

int a[N];
int n, p[N];
void test_case() {
    cin >> n;
    memset(a, -1, sizeof(a));
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }

    if (p[0]) {
        cout << "No\n";
        return;
    }
    a[0] = 0;
    int cur = 1;
    for (int i = 1; i < n; i++) {
        if (p[i] >= i + 1) {
            cout << "No\n";
            return;
        }
        int len = p[i - 1];
        if (p[i] > p[i - 1] + 1) {
            cout << "No\n";
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
                cout << "No\n";
                return;
            }
        }
        if (len + 1 == p[i]) {
            a[i] = a[len];
            if (st.count(a[i])) {
                cout << "No\n";
                return;
            }
        }
    }
//    for (int i = 0; i < n; i++) {
//        cout << a[i] << " ";
//    }
//    cout << endl;
    cout << "Yes\n";
}

int main() {
    // freopen("input.txt", "r", stdin);
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T; cin >> T;
    while (T--) {
        test_case();
    }
    return 0;
}