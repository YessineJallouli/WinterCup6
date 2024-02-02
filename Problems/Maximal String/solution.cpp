#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        string s; cin >> s;
        string ch;
        for (int i = 0; i < n; i++) {
            if (s[i] == '0') {
                if (i != n-1 && s[i+1] == '0') {
                    ch+= '1';
                    i++;
                }
                else
                    ch+= '0';
            }
            else
                ch+= '1';
        }
        n = (int) ch.size();
        string ans;
        for (int i = n-1; i >= 0; i--) {
            int id = (int) ch.size()-1;
            if (id >= 2 && ch[id] == '1' && ch[id-1] == '1' && ch[id-2] == '0') {
                ch.pop_back();
                ch.pop_back();
                ch.pop_back();
                ch+= '1';
            }
            else if (! ch.empty()) {
                ans+= ch.back();
                ch.pop_back();
            }
        }
        reverse(ans.begin(), ans.end());
        cout << ans << '\n';
    }
}
