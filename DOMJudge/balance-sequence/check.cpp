#include "testlib.h"
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


int main(int argc, char** argv) {
    registerTestlibCmd(argc, argv);
    int T;
    T= inf.readInt(1,1000,"T");
    int nb = 1;
    int sumN = 0;
    int h;
    while (T--) {
        setTestCase(nb++);
        int n = inf.readInt(1,5000,"n");
        sumN+= n;
        inf.readLine();
        string s = inf.readString();
        h = s.size();
        int a = ouf.readInt(1,n, "a");
        int b = ouf.readInt(1,n, "b");
        if (a > b) {
            quitf(_wa, "a should be greater than b");
        }
        reverse(s.begin()+a-1, s.begin()+b);
        if (! isBalanced(s)) {
            quitf(_wa, "The new sequence is not balanced");
        }
    }
    quitf(_ok, "All answers are correct good job");
}
