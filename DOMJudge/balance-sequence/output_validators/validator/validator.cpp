#include <utility>
#include <string>
#include <sstream>
#include <cassert>
#include <cstring>
#include <cmath>
#include <unistd.h>
#include <vector>
#include "validator.h"
#include <cstdint>

using namespace std;

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
    init_io(argc, argv);
    int T;
    judge_in >> T;
    int nb = 1;
    int sumN = 0;
    int h;
    while (T--) {
        int n ;
        if(!(judge_in >> n))
            wrong_answer("Failed to read n");
        sumN+= n;
        string s;
        if(!(judge_in >> s))
            wrong_answer("Failed to read s");
        h = s.size();
        int a,b;
        if(!(author_out >> a >> b))
            wrong_answer("Failed to read a and b");
        if (a > b) {
            wrong_answer("a should be greater than b");
        }
        reverse(s.begin()+a-1, s.begin()+b);
        if (! isBalanced(s)) {
            wrong_answer("The new sequence is not balanced");
        }
    }
    accept();
}
