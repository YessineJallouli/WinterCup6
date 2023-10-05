//
// Created by ramizouari on 05/10/23.
//
#include "testlib.h"

using real=long double;
constexpr real eps=1e-4;

int main(int argc, char * argv[]) {
    registerTestlibCmd(argc, argv);

    std::string walkEstimate=ouf.readToken();
    std::string answer=ans.readToken();
    if(answer=="inf" && walkEstimate!="inf")
        quitf(_wa,"The answer is inf but the contestant output is %s",walkEstimate.c_str());
    else if(answer!="inf" && walkEstimate=="inf")
        quitf(_wa,"The answer is %s but the contestant output is inf",answer.c_str());
    else if(answer!="inf" && walkEstimate!="inf")
    {
        real value=std::stold(answer);
        real estimate=std::stold(walkEstimate);
        if(std::abs(value-estimate)>eps)
            quitf(_wa,"The answer is %s but the contestant output is %s",answer.c_str(),walkEstimate.c_str());
        else
            quitf(_ok,"The answer is %s and the contestant output is %s",answer.c_str(),walkEstimate.c_str());
    }
    else
        quitf(_ok,"The answer is inf and the contestant output is inf");

}