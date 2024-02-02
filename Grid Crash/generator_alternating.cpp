#include <iostream>

int main(int argc,char **argv)
{
    int n=std::stoi(argv[1]);
    int m=std::stoi(argv[2]);
    std::cout << n << ' ' << m << '\n';
    char c_row='W',c=c_row;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            std::cout << c;
            c=(c=='W'?'B':'W');
        }
        c_row=(c_row=='W'?'B':'W');
        c=c_row;
        std::cout << '\n';
    }
}