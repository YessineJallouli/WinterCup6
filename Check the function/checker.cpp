#include "testlib.h"
using namespace std;

bool solve(vector<int> p) {
    int n = (int) p.size();

    if (p[0]) {
        return false;
    }

    vector<int> a(n);
    a[0] = 1;
    int cur = 2;
    for (int i = 1; i < n; i++) {
        if (p[i] >= i + 1) {
            return false;
        }
        int len = p[i - 1];
        if (p[i] > p[i - 1] + 1) {
            return false;
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
                return false;
            }
        }
        if (len + 1 == p[i]) {
            a[i] = a[len];
            if (st.count(a[i])) {
                return false;
            }
        }
    }
    return true;
}

vector<int> prefix(vector<int> s) {
    int n = (int) s.size();
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

int main(int argc, char * argv[]) {
    registerTestlibCmd(argc, argv);
    int T = inf.readInt();
    int nb = 1;
    while (T--) {
        setTestCase(nb++);
        int n = inf.readInt();
        vector<int> p(n);
        for (int i = 0; i < n; i++) {
            p[i] = inf.readInt();
        }
        vector<int> a(n);
        int f = ouf.readInt();
        a[0] = f;
        if (f != -1) {
            for (int i = 1; i < n; i++) {
                a[i] = ouf.readInt();
            }
        }
        bool solvable = solve(p);
        if (solvable && a[0] == -1) {
            quitf(_wa, "Solution exists, but contestant didn't find it -1");
        }
        else if (! solvable && a[0] != -1) {
            bool ok = true;
            vector<int> pr = prefix(a);
            for (int i = 0; i < n; i++) {
                if (pr[i] != p[i])
                    ok = false;
            }
            if (ok) {
                quitf(_fail, "Judge didn't find a solution, but contestant gives a correct solution");
            }
            else {
                quitf(_wa, "Solution doesn't exist, but contestant gives a wrong solution");
            }
        }
        if (! solvable && a[0] == -1)
            continue;
        vector<int> pr = prefix(a);
        for (int i = 0; i < n; i++) {
            if (pr[i] != p[i])
                quitf(_wa, "Wrong prefix function");
        }
    }
    quitf(_ok, "All arrays are correct");
}