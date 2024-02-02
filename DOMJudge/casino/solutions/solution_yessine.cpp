#include <bits/stdc++.h>
using namespace std;

double t = 0.57;

int main() {
    int nbR; cin >> nbR;
    while (nbR--) {
        string g; cin >> g;
        double score = 0;
        bool loose = false;
        while (score <= t) {
            cout << 1 << endl;
            string c; cin >> c;
            if (c == "L") {
                loose = true;
                break;
            }
            double r; cin >> r;
            score = r;
        }
        if (loose)
            continue;
        cout << 0 << endl;
        string c; cin >> c;
    }
}
