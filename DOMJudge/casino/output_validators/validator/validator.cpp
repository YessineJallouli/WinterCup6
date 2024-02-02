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

void check_case(int run) {
	int nbools;
	assert(judge_in >> nbools);
	cout << nbools << endl;

	vector<int> bools(nbools);
	for (int pos = 0; pos < nbools; pos++) {
		assert(judge_in >> bools[pos]);
	}

	int nquery = 0;
	while (true) {
		nquery++;
		string operation;
		if (!(author_out >> operation)) {
			wrong_answer("testcase %d: Cannot parse operation in %dth query.\n", run, nquery);
		}
		int pos;
		if (!(author_out >> pos)) {
			wrong_answer("testcase %d: Cannot parse position in %dth query.\n", run, nquery);
		}
		if (pos < 0 || pos >= nbools) {
			wrong_answer("testcase %d: position %d out of range\n", run, pos);
		}
		if (operation == "OUTPUT") {
			if (pos >= nbools - 1) {
				wrong_answer("testcase %d: position %d out of range for OUTPUT\n", run, pos);
			}
			if (!bools[pos] || bools[pos + 1]) {
				wrong_answer("testcase %d: wrong output, position %d,%d = %d,%d\n", run, pos, pos+1, bools[pos], bools[pos+1]);
			}
			// The team made the correct guess, moving on to the new case.
			break;
		} else if (operation == "READ") {
			// Simulate slow operation by sleeping for 0.1ms
			usleep(100);
			cout << (bools[pos] ? "true" : "false") << endl;
		} else {
			wrong_answer("testcase %d: Unknown instruction '%s'.\n", run, operation.c_str());
		}
	}
}

using real=long double;
using integer=std::int64_t;

struct GameResult
{
    double S1,S2;
    GameResult(double S1,double S2):S1(S1),S2(S2){}
    GameResult() = default;
};

std::vector<GameResult> play_casino(int L,int G)
{
    std::vector<GameResult> games;
    games.reserve(G);
    integer score=0;
    std::vector<real> Z(L);
    for(int i=0;i<L;i++)
        judge_in >> Z[i];
    int a=0,b=L-1;
    std::cout << G << std::endl;
    for(int i=0;i<G;i++)
    {
        std::cout << "G" << std::endl;
        real S1=0,S2=0;
        int x;
        while(((author_out >> x),x) && S1 <= 1)
            std::cout << 'S'  << ' ' << (S1+=Z[a++]) << std::endl;
        while(S1 <= 1 && S2 < S1)
            S2+=Z[b--];
        if(S1 <= 1 && S2 > 1)
            std::cout << "W" << std::endl;
        else
            std::cout << "L" << std::endl;
        games.emplace_back(S1,S2);
    }
    return games;
}

constexpr real p = 0.42;

int main(int argc, char **argv) {
	init_io(argc, argv);

	int L,G;
	assert(judge_in >> L >> G);
	auto results=play_casino(L,G);
    integer score=0;
    for(auto [S1,S2]:results) if(S1 <= 1 && (S2 <= S1 || S2 > 1))
        score++;
    real scoreRatio=score/(real)G;
    if(G<100000)
        accept();
    else if(scoreRatio < p)
        wrong_answer("Score ratio is too low, %Lf",scoreRatio);
    else
        accept();
    /*
        // Check for trailing output.
        string trash;
        if (author_out >> trash) {
            wrong_answer("Trailing output: '%s'\n", trash.c_str());
        }

        // Yay!
        accept();
    */
}