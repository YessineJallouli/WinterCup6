#include "testlib.h"
#include <algorithm>
#include <numeric>
constexpr int N_max=100'000;
constexpr int K_max=1'000'000'000;
constexpr int L_max=1'000'000'000;

int main(int argc, char** argv)
{
    registerValidation(argc,argv);
    int N=inf.readInt(1,N_max,"n");
    inf.readSpace();
    int K=inf.readInt(1,K_max,"k");
    inf.readEoln();
    std::vector<int> A(N);
    for(int i=0;i<N;i++)
    {
        A[i]=inf.readInt(0,L_max,"a_i");
        if(i<N-1)
            inf.readSpace();
        else
            inf.readEoln();
    }
    ensuref(std::accumulate(A.begin(),A.end(),0LL , [K](auto x, auto y){
        return (x+y)%K;
    })==0,"sum of a_i should be divisible by k");
    inf.readEof();
}