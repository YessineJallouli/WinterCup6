#include "testlib.h"

// inf - stream with the testdata.
// ouf - stream with the contestant output.
// ans - stream with the jury answer.

int main(int argc, char * argv[]) {
    registerTestlibCmd(argc, argv);

    int n = inf.readInt();
    int oufq = ouf.readInt();
    int ansq = ans.readInt();

    if (ansq > 2*n-4)
        quitf(_fail, "Limit is %d, but main solution have made %d queries", 2*n-4, ansq);

    if (oufq > 2*n-4)
        quitf(_wa, "Limit is %d, but solution have made %d queries", 2*n-4, oufq);

    quitf(_ok, "The permutation is guessed correctly using %d queries", oufq);

}
