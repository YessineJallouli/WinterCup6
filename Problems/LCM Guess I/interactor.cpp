#include "testlib.h"
using namespace std;

// inf - stream with the testdata.
// ouf - stream with the contestant output.
// ans - stream with the jury answer.

const int N = 1e5;

int main(int argc, char** argv) {
    registerInteraction(argc, argv);
    int n = inf.readInt(3,N);
    cout << n << endl;
    int p[n];
    for (int i = 0; i < n; i++)
        p[i] = inf.readInt();

    int nbQuery = 0;
    int mxQuery = 2*n-3;

    while (true) {
        nbQuery++;
        if (nbQuery > mxQuery) {
            quitf(_wa, "Too many operations !");
        }
        char c = ouf.readChar();
        if (c == '?') {
            int i = ouf.readInt(1, n), j = ouf.readInt(1, n);
            i--; j--;
            ouf.eoln();
            if (i == j)
                quitf(_wa, "Wrong format, you should choose different positions !");
            cout << 1LL * p[i] * p[j] / __gcd(p[i], p[j]) << endl;
        } else if (c == '!') {
            for (int i = 0; i < n; i++) {
                int k = ouf.readInt(1, n);
                if (k != p[i])
                    quitf(_wa, "Wrong permutation, expected %d at position %d, found %d", p[i], i + 1, k);
            }
            tout << nbQuery-1 << endl;
            quitf(_ok, "Permutation is guessed correctly using %d queries", nbQuery);
        } else {
            quitf(_pe, "Invalid query !, found %c hhh", c);
        }
    }
}
