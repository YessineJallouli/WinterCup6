#include <bits/stdc++.h>
using namespace std;

vector<int> prefix(vector<int> s) {
    int n = s.size();
    vector<int> pr(n);
    pr[0] = 0;
    for (int i = 1; i < n; i++) {
        int j = pr[i - 1];
        while (j > 0 && s[j] != s[i])
            j = pr[j - 1];
        if (s[i] == s[j])
            j++;
        pr[i] = j;
    }
    return pr;
}

int N;

vector<vector<int>> allVectors;

void generateArrays(int n, vector<int> &v) {
    if (n == 0) {
        allVectors.push_back(v);
        return;
    }
    for (int i = 0; i < N; i++) {
        v.push_back(i);
        generateArrays(n-1, v);
        v.pop_back();
    }
}

set<vector<int>> prefixes;

int main()
{
    N = 9;
    vector<int> k;
    generateArrays(N,k);
    for (auto i : allVectors) {
        prefixes.insert(prefix(i));
    }
    cout << prefixes.size() << '\n';
//    for (auto i : prefixes) {
//        for (int j : i)
//            cout << j << ' ';
//        cout << '\n';
//    }

}
