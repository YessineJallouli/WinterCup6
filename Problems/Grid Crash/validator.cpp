#include "testlib.h"

int main(int argc, char** argv)
{
    registerValidation(argc, argv);
    int n,m;
    n=inf.readInt(1,5,"n");
    inf.readSpace();
    m=inf.readInt(1,5,"m");
    inf.readEoln();
    for(int i=0;i<n;i++)
    {
        for (int j = 0; j < m; j++)
        {
            auto c = inf.readChar();
            ensuref(c == 'W' || c == 'B', "Invalid character %c", c);
        }
        if(i<n-1) inf.readEoln();
    }
    inf.readEof();
}