#include <iostream>
#include <vector>
#include "testlib.h"

constexpr int n_max=1'000'000'000,m_max=1'000'000'000;
constexpr int d_max=1'000'000'000,Q_max=100'000;
int main(int argc, char** argv)
{
    registerValidation(argc,argv);
    int Q=inf.readInt(1,Q_max,"Q");
    inf.readEoln();
    for(int i=0;i<Q;i++)
    {
        int n=inf.readInt(1,n_max,"n");
        inf.readSpace();
        int m=inf.readInt(1,m_max,"m");
        inf.readSpace();
        int x=inf.readInt(1,n,"x");
        inf.readSpace();
        int y=inf.readInt(1,m,"y");
        inf.readSpace();
        int d=inf.readInt(0,d_max,"d");
        inf.readEoln();
    }
    inf.readEof();
}