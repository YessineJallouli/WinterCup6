//
// Created by ramizouari on 29/09/23.
//
#include <iostream>
#include <vector>
#include <array>
#include <set>
#include <algorithm>
#include <unordered_set>

enum class Op
{
    Add,Sub,Mul,Div
};

struct Action
{
    int i,j;
    Op op;
};

template<size_t N>
std::pair<int,std::vector<Action>> closestTarget(std::array<int,N> V,int x)
{
    auto CMP=[x](int a,int b){return std::abs(a-x)<std::abs(b-x);};
    if constexpr (N==1)
        return {V[0],{}};
    else
    {
        std::vector<Action> S;
        int y=*std::min_element(V.begin(),V.end(),CMP);
        for(int i=0;i<N;i++) for(int j=0;j<N;j++) if(i!=j)
        {
            std::array<int,N-1> W;
            for(int l=0,k=0;l<N;l++) if(l!=i && l!=j)
                W[k++]=V[l];
            if(i<j)
            {
                {
                    W[N-2]=V[i]+V[j];
                    auto [z,sequence]=closestTarget<N-1>(W,x);
                    if(CMP(z,y))
                    {
                        sequence.push_back({i, j, Op::Add});
                        y=z;
                        S=std::move(sequence);
                    }
                }
                {
                    W[N-2]=V[i]*V[j];
                    auto [z,sequence]=closestTarget<N-1>(W,x);
                    if(CMP(z,y))
                    {
                        sequence.push_back({i, j, Op::Mul});
                        y=z;
                        S=std::move(sequence);
                    }
                }
            }
            if(V[i]>=V[j])
            {
                W[N-2]=V[i]-V[j];
                auto [z,sequence]=closestTarget<N-1>(W,x);
                if(CMP(z,y))
                {
                    y=z;
                    sequence.push_back({i, j, Op::Sub});
                    S=std::move(sequence);
                }
            }
            if(V[j] && V[i]%V[j]==0)
            {
                W[N-2]=V[i]/V[j];
                auto [z,sequence]=closestTarget<N-1>(W,x);
                if(CMP(z,y))
                {
                    sequence.push_back({i, j, Op::Div});
                    y=z;
                    S=std::move(sequence);
                }
            }
        }
        return {y,S};
    }
}


using equation=std::tuple<int,int,Op,int>;
std::ostream & operator<<(std::ostream &O,const equation &E)
{
    auto [i,j,op,y]=E;
    O<<i << ' ';
    switch(op)
    {
        case Op::Add:
            O<<'+';
            break;
        case Op::Sub:
            O<<'-';
            break;
        case Op::Mul:
            O<<'*';
            break;
        case Op::Div:
            O<<'/';
            break;
    }
    O << ' ' << j << ' ' << '=' << ' ' << y;
    return O;
}

int main()
{
    int T;
    std::cin>>T;
    while(T--)
    {
        std::array<int,6> V;
        for(auto &i:V) std::cin>>i;
        int x;
        std::cin>>x;
        auto [y,S]=closestTarget(V,x);
        std::cout<<y<<std::endl;
        std::vector<int> R(V.size());
        std::copy(V.begin(),V.end(),R.begin());
        std::cout << S.size() << '\n';
        while(S.size())
        {
            auto [i,j,op]=S.back();
            S.pop_back();
            equation E;
            int z=0;
            switch(op)
            {
                case Op::Add:
                    E={R[i],R[j],Op::Add,R[i]+R[j]};
                    R[i]+=R[j];
                    break;
                case Op::Sub:
                    E={R[i],R[j],Op::Sub,R[i]-R[j]};
                    R[i]-=R[j];
                    break;
                case Op::Mul:
                    E= {R[i],R[j],Op::Mul,R[i]*R[j]};
                    R[i]*=R[j];
                    break;
                case Op::Div:
                    E={R[i],R[j],Op::Div,R[i]/R[j]};
                    R[i]/=R[j];
                    break;
            }
            z=R[i];
            for(int k=0,l=0;k<R.size();k++) if(k!=i && k!=j)
                    R[l++]=R[k];
            std::cout<<E<< '\n';
            R.pop_back();
            R.back()=z;
        }
        std::cout << '\n';
    }

}