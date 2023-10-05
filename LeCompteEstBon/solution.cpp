#include <bits/stdc++.h>
#define ll long long
using namespace std;

mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());

int ans = 0;
string solution;
int target;

set<int> number;

set<vector<int>> visited;

void solve(vector<int> &v, string s) {
    vector<int> b;
    for (int i : v)
        b.push_back(i);

    sort(b.begin(), b.end());
    if (visited.count(b))
        return;
    visited.insert(b);

    for (int i : v) {
        if (abs(i-target) < abs(ans-target) && ! number.count(i)) {
            number.insert(i);
            ans = i;
            solution = s;
        }
        if (abs(i-target) == abs(ans-target)) {
            solution = min(solution,s);
        }
    }
    if ((int) v.size() == 1) {
        return;
    }
    // in one operation i will take two numbers
    int n = (int) v.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            vector<int> aux;
            for (int k = 0; k < n; k++) {
                if (i == k || j == k)
                    continue;
                aux.push_back(v[k]);
            }

            // addition
            if (v[i] != 0 && v[j] != 0 && i < j) {
                aux.push_back(v[i] + v[j]);
                solve(aux, s + to_string(v[i]) + " + " + to_string(v[j]) + " = " + to_string(v[i] + v[j]) + "\n");
                aux.pop_back();
            }

            // subtraction
            if (v[j] != 0 && v[i] > v[j]) {
                aux.push_back(v[i] - v[j]);
                solve(aux, s + to_string(v[i]) + " - " + to_string(v[j]) + " = " + to_string(v[i] - v[j]) + "\n");
                aux.pop_back();
            }

            // division
            if (v[j] != 1 && v[j] != 0 && v[i]%v[j] == 0) {
                aux.push_back(v[i]/v[j]);
                solve(aux, s + to_string(v[i]) + " / " + to_string(v[j]) + " = " + to_string(v[i]/v[j]) + "\n");
                aux.pop_back();
            }

            // multiplication
            if (v[i] != 1 && v[j] != 1 && i < j) {
                aux.push_back(v[i] * v[j]);
                solve(aux, s + to_string(v[i]) + " * " + to_string(v[j]) + " = " + to_string(v[i] * v[j]) + "\n");
                aux.pop_back();
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        visited.clear();
        number.clear();
        vector<int> v;
        int n = 6;
        for (int i = 0; i < n; i++) {
            int x;
            cin >> x;
            v.push_back(x);
        }
        cin >> target;
        string s = "";
        solve(v, s);
        int nb = 0;
        for (char c: solution) {
            if (c == '\n')
                nb++;
        }
        cout << nb << '\n' << solution << '\n';
    }
}
